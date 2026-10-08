#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
  printf 'Usage: %s [build-root]\n' "$0"
  printf 'Exports source edits after the existing project patches; never accesses Panda hardware.\n'
  exit 0
fi
if (( $# > 1 )) || [[ "${1:-}" == --* ]]; then
  printf 'Usage: %s [build-root]\n' "$0" >&2
  exit 2
fi
cache_root="${1:-${repo_root}/.firmware-build}"
if [[ ! -d "$cache_root" ]]; then
  printf 'Run build_panda_debug_firmware.sh --prepare-only first.\n' >&2
  exit 1
fi
cache_root="$(cd -- "$cache_root" && pwd)"
baseline_dir="${cache_root}/.safety-baseline"

# Check both baselines before writing either output.
for name in opendbc panda; do
  if [[ ! -s "${baseline_dir}/${name}.tree" || ! -s "${baseline_dir}/${name}.commit" ]]; then
    printf 'Run build_panda_debug_firmware.sh --prepare-only first.\n' >&2
    exit 1
  fi
  expected="$(cat "${baseline_dir}/${name}.commit")"
  actual="$(git -C "${cache_root}/${name}" rev-parse HEAD)"
  [[ "$actual" == "$expected" ]] || { printf 'Unexpected %s source commit; refusing export.\n' "$name" >&2; exit 1; }
  git -C "${cache_root}/${name}" cat-file -e "$(cat "${baseline_dir}/${name}.tree")^{tree}"
done

for name in opendbc panda; do
  (
    source_dir="${cache_root}/${name}"
    index="$(mktemp "${baseline_dir}/export-index.XXXXXX")"
    rm -f -- "$index"
    output="${repo_root}/patches/${name}-local-safety.patch"
    temporary_patch="$(mktemp "${baseline_dir}/export-patch.XXXXXX")"
    trap 'rm -f -- "$index" "${index}.lock" "$temporary_patch"' EXIT
    export GIT_INDEX_FILE="$index"
    git -C "$source_dir" read-tree HEAD
    git -C "$source_dir" add -A -- .
    current="$(git -C "$source_dir" write-tree)"
    baseline="$(cat "${baseline_dir}/${name}.tree")"
    git -C "$source_dir" diff --binary --full-index "$baseline" "$current" -- > "$temporary_patch"
    if [[ -s "$temporary_patch" ]]; then
      mv -f -- "$temporary_patch" "$output"
      printf 'Exported additional %s edits: %s\n' "$name" "$output"
    else
      # These two generated paths are owned by this exporter. Reverting all source
      # edits also removes the old export so a subsequent build cannot reapply it.
      rm -f -- "$output"
      printf 'No additional %s edits; no tuning patch.\n' "$name"
    fi
  )
done
