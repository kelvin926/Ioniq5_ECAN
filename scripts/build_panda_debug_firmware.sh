#!/usr/bin/env bash
set -euo pipefail

PANDA_COMMIT="dd8a5b3df77706337a11555377e7180c5adc8726"
OPENDBC_COMMIT="b72c1fd55ae7e84763e40912bbe06b8f533cb66b"

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
split_arm_patch="${repo_root}/patches/opendbc-hyundai-canfd-split-arm.patch"
panda_version_patch="${repo_root}/patches/panda-builder-env.patch"

cache_root="${1:-${XDG_CACHE_HOME:-$HOME/.cache}/ioniq5_ecan/upstream}"
panda_dir="${cache_root}/panda"
opendbc_dir="${cache_root}/opendbc"
venv_dir="${cache_root}/venv"

for command_name in git uv; do
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
apply_patch_once "${panda_dir}" "${panda_version_patch}" \
  "IONIQ5 firmware builder marker patch"

uv venv --python 3.11 --clear "${venv_dir}"
# shellcheck disable=SC1091
source "${venv_dir}/bin/activate"
uv pip install -e "${opendbc_dir}"
uv pip install --no-deps -e "${panda_dir}"
uv pip install \
  cffi libusb1 libusb-package scons \
  "gcc-arm-none-eabi @ git+https://github.com/commaai/dependencies.git@release-gcc-arm-none-eabi#subdirectory=gcc-arm-none-eabi"

# Do not set RELEASE: the Hyundai longitudinal flag is compiled only with ALLOW_DEBUG.
# Also clear an ambient DEBUG variable; Panda uses it for additional debug-only code that is
# unrelated to the debug signing key and should not vary the pinned image accidentally.
(cd "${panda_dir}" && env -u RELEASE -u DEBUG PANDA_BUILDER=IONIQ5 scons -j"$(nproc)" \
  board/obj/bootstub.panda_h7.bin board/obj/panda_h7.bin.signed)

firmware="${panda_dir}/board/obj/panda_h7.bin.signed"
bootstub="${panda_dir}/board/obj/bootstub.panda_h7.bin"
sha256sum "${bootstub}"
sha256sum "${firmware}"
sha256sum "${split_arm_patch}" "${panda_version_patch}"
printf 'Built pinned DEBUG bootstub: %s\n' "${bootstub}"
printf 'Built pinned DEBUG firmware: %s\n' "${firmware}"
printf 'Flash only after reading docs/panda_firmware.md.\n'
