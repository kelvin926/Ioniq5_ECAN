#!/usr/bin/env bash
set -euo pipefail

PANDA_COMMIT="dd8a5b3df77706337a11555377e7180c5adc8726"
OPENDBC_COMMIT="b72c1fd55ae7e84763e40912bbe06b8f533cb66b"

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
prepare_only=false
if [[ "${1:-}" == "--prepare-only" ]]; then
  prepare_only=true
  shift
fi
if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
  printf 'Usage: %s [--prepare-only] [build-root]\n' "$0"
  printf 'Prepare-only downloads pinned sources and applies patches without package installation, build or device access.\n'
  exit 0
fi
if (( $# > 1 )) || [[ "${1:-}" == --* ]]; then
  printf 'Usage: %s [--prepare-only] [build-root]\n' "$0" >&2
  exit 2
fi
split_arm_patch="${repo_root}/patches/opendbc-hyundai-canfd-split-arm.patch"
command_session_patch="${repo_root}/patches/opendbc-command-session.patch"
command_session_startup_patch="${repo_root}/patches/opendbc-command-session-startup.patch"
panda_version_patch="${repo_root}/patches/panda-builder-env.patch"
panda_ecan_patch="${repo_root}/patches/panda-ecan-only.patch"

cache_root="${1:-${repo_root}/.firmware-build}"

required_commands=(git)
if ! $prepare_only; then required_commands+=(uv); fi
for command_name in "${required_commands[@]}"; do
  if ! command -v "${command_name}" >/dev/null 2>&1; then
    printf 'Required command is missing: %s\n' "${command_name}" >&2
    exit 1
  fi
done

clone_pinned() {
  local url="$1"
  local commit="$2"
  local destination="$3"
  if [[ ! -d "${destination}/.git" ]]; then
    git clone --filter=blob:none --no-checkout "${url}" "${destination}"
  fi
  git -C "${destination}" fetch --depth=1 origin "${commit}"
  git -C "${destination}" checkout --detach "${commit}"
}

mkdir -p "${cache_root}"
cache_root="$(cd -- "$cache_root" && pwd)"
panda_dir="${cache_root}/panda"
opendbc_dir="${cache_root}/opendbc"
venv_dir="${cache_root}/venv"
export UV_CACHE_DIR="${cache_root}/cache"
export UV_PYTHON_INSTALL_DIR="${cache_root}/python"
export XDG_CACHE_HOME="${cache_root}/cache"
export TMPDIR="${cache_root}/tmp"
export PYTHONDONTWRITEBYTECODE=1
mkdir -p "$TMPDIR"
clone_pinned https://github.com/commaai/opendbc.git "${OPENDBC_COMMIT}" "${opendbc_dir}"
clone_pinned https://github.com/commaai/panda.git "${PANDA_COMMIT}" "${panda_dir}"

apply_patch_once() {
  local destination="$1"
  local patch_file="$2"
  local description="$3"
  if git -C "${destination}" apply --reverse --check "${patch_file}" >/dev/null 2>&1; then
    printf '%s already applied.\n' "${description}"
  else
    git -C "${destination}" apply --check "${patch_file}"
    git -C "${destination}" apply "${patch_file}"
    printf 'Applied %s.\n' "${description}"
  fi
}

apply_patch_once "${opendbc_dir}" "${split_arm_patch}" \
  "opt-in LDA lateral / SET longitudinal Panda safety patch"
# The follow-up changes files created by the base patch, so its reverse check
# would fail on an already upgraded cache. Do not reapply the base there.
if git -C "${opendbc_dir}" apply --reverse --check "${command_session_startup_patch}" >/dev/null 2>&1; then
  printf 'Command-session base and startup fix already applied.\n'
else
  apply_patch_once "${opendbc_dir}" "${command_session_patch}" \
    "volatile command-gap button session and standby safety patch"
fi
apply_patch_once "${opendbc_dir}" "${command_session_startup_patch}" \
  "command-session initial RX versus safety-tick race fix"
apply_patch_once "${panda_dir}" "${panda_version_patch}" \
  "IONIQ5ECAN firmware builder marker patch"
apply_patch_once "${panda_dir}" "${panda_ecan_patch}" \
  "ECAN-only transceiver and harness-orientation patch"

if $prepare_only; then
  baseline_dir="${cache_root}/.safety-baseline"
  mkdir -p "$baseline_dir"
  record_baseline() {
    local destination="$1" name="$2" pin="$3"
    shift 3
    local index tree old_tree
    index="$(mktemp "${baseline_dir}/index.XXXXXX")"
    rm -f -- "$index"
    (
      trap 'rm -f -- "$index" "${index}.lock"' EXIT
      export GIT_INDEX_FILE="$index"
      git -C "$destination" read-tree "$pin"
      for patch in "$@"; do git -C "$destination" apply --cached "$patch"; done
      tree="$(git -C "$destination" write-tree)"
      if [[ -s "${baseline_dir}/${name}.tree" ]]; then
        old_tree="$(cat "${baseline_dir}/${name}.tree")"
        if [[ "$old_tree" != "$tree" ]]; then
          printf 'Prepared baseline changed; preserve existing tuning and review before replacing it.\n' >&2
          exit 1
        fi
      fi
      printf '%s\n' "$tree" > "${baseline_dir}/${name}.tree"
      printf '%s\n' "$pin" > "${baseline_dir}/${name}.commit"
      git -C "$destination" update-ref refs/ioniq5-safety/baseline "$tree"
    )
  }
  record_baseline "$opendbc_dir" opendbc "$OPENDBC_COMMIT" \
    "$split_arm_patch" "$command_session_patch" "$command_session_startup_patch"
  record_baseline "$panda_dir" panda "$PANDA_COMMIT" "$panda_version_patch" "$panda_ecan_patch"
fi

# Additional user edits are exported relative to the five project patches above.
# Empty/missing exports leave the existing firmware behavior unchanged.
for source_name in opendbc panda; do
  local_patch="${repo_root}/patches/${source_name}-local-safety.patch"
  if [[ -s "$local_patch" ]]; then
    apply_patch_once "${cache_root}/${source_name}" "$local_patch" "${source_name} local safety edits"
  fi
done

if $prepare_only; then
  printf 'Prepared editable sources: %s\n' "$cache_root"
  printf 'Values are unchanged unless an existing local-safety patch was supplied.\n'
  exit 0
fi

if [[ ! -d "${venv_dir}" ]]; then
  uv venv --python 3.11 "${venv_dir}"
fi
# shellcheck disable=SC1091
if [[ -f "${venv_dir}/bin/activate" ]]; then
  source "${venv_dir}/bin/activate"
else
  # Git Bash uses the native Windows uv layout.
  source "${venv_dir}/Scripts/activate"
fi
uv pip install -e "${opendbc_dir}"
uv pip install --no-deps -e "${panda_dir}"
uv pip install \
  cffi libusb1 libusb-package scons

# The comma compiler wheel does not support native Windows. Prefer a portable
# Arm GNU installation next to the cache when building from Git Bash.
portable_toolchain_bin="$(dirname "${cache_root}")/toolchains/arm-gnu-14.2/bin"
if [[ -x "${portable_toolchain_bin}/arm-none-eabi-gcc.exe" ]]; then
  export PATH="${portable_toolchain_bin}:${PATH}"
fi
if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  uv pip install \
    "gcc-arm-none-eabi @ git+https://github.com/commaai/dependencies.git@release-gcc-arm-none-eabi#subdirectory=gcc-arm-none-eabi"
fi

# Do not set RELEASE: the Hyundai longitudinal flag is compiled only with ALLOW_DEBUG.
# Also clear an ambient DEBUG variable; Panda uses it for additional debug-only code that is
# unrelated to the debug signing key and should not vary the pinned image accidentally.
(cd "${panda_dir}" && env -u RELEASE -u DEBUG PANDA_BUILDER=IONIQ5ECAN SETLEN=1 scons -j"$(nproc)" \
  board/obj/bootstub.panda_h7.bin board/obj/panda_h7.bin.signed)

firmware="${panda_dir}/board/obj/panda_h7.bin.signed"
bootstub="${panda_dir}/board/obj/bootstub.panda_h7.bin"
sha256sum "${bootstub}"
sha256sum "${firmware}"
sha256sum "${split_arm_patch}" "${command_session_patch}" "${panda_version_patch}" "${panda_ecan_patch}"
sha256sum "${command_session_startup_patch}"
printf 'Built pinned DEBUG bootstub: %s\n' "${bootstub}"
printf 'Built pinned DEBUG firmware: %s\n' "${firmware}"
printf 'Flash only after reading docs/panda_firmware.md.\n'
