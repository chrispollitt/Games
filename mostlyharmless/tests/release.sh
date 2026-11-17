#!/bin/sh
# Build both games in release mode (no debug verbs), report sizes, and replay
# the walkthroughs that use no debug verbs against the release story files.
root=$(cd "$(dirname "$0")/.." && pwd)
fail=0
for g in mini full; do
  "$root/build.sh" $g > /dev/null 2>&1 || { echo "FAIL build $g"; fail=1; }
done
ls -l "$root/mini/mini.z5" "$root/full/full.z8" | awk '{print $5, $NF}'
check() {  # game walkthrough story
  "$root/tests/walk.sh" "$1" "$2" "$3" || { echo "FAIL run $1/$2"; fail=1; return; }
  if grep -v 'Serial number' "$root/tests/out/$1/$2.scr" | diff -q "$root/tests/$1/golden/$2.scr" - > /dev/null; then
    echo "PASS release $1/$2"
  else
    echo "FAIL release $1/$2 (differs from golden)"; fail=1
  fi
}
check mini radio     "$root/mini/mini.z5"
check mini book1     "$root/mini/mini.z5"
check full act1      "$root/full/full.z8"
check full parser2   "$root/full/full.z8"
check full complete  "$root/full/full.z8"
check mini complete  "$root/mini/mini.z5"
exit $fail
