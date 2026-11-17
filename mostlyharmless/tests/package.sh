#!/bin/sh
# Build release story files and package them for distribution.
#   tests/package.sh        -> dist/mostlyharmless-<date>.zip (needs zip) and the story files in dist/
root=$(cd "$(dirname "$0")/.." && pwd)
dist=$root/dist
mkdir -p "$dist"
for g in mini full; do
  "$root/build.sh" $g > /dev/null 2>&1 || { echo "build failed: $g" >&2; exit 1; }
done
cp "$root/mini/mini.z5" "$dist/mostlyharmless-lastorders.z5"
cp "$root/full/full.z8" "$dist/mostlyharmless-longwayround.z8"
cp "$root/README.md" "$root/CREDITS.md" "$root/CHANGELOG.md" "$root/LICENSE" "$root/LICENSE-CONTENT" "$root/LICENSING.md" "$root/NOTICE" "$root/docs/WALKTHROUGH-full.md" "$root/docs/WALKTHROUGH-mini.md" "$dist/"
mkdir -p "$dist/third_party" && cp "$root"/third_party/*.txt "$dist/third_party/"
ls -l "$dist"
if command -v zip > /dev/null 2>&1; then
  stamp=$(date +%Y%m%d)
  (cd "$dist" && rm -f "mostlyharmless-$stamp.zip" && zip -q -r "mostlyharmless-$stamp.zip" *.z5 *.z8 *.md LICENSE LICENSE-CONTENT NOTICE third_party) && echo "wrote dist/mostlyharmless-$stamp.zip"
else
  echo "zip not found: story files and documents are in dist/"
fi
