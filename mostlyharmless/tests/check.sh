#!/bin/sh
# Run every walkthrough of a game and compare with the golden transcripts.
#   tests/check.sh <game>            compare
#   tests/check.sh <game> --update   (re)write the golden transcripts
root=$(cd "$(dirname "$0")/.." && pwd)
game=$1
fail=0
mkdir -p "$root/tests/$game/golden"
for cmd in "$root"/tests/$game/*.cmd; do
  name=$(basename "$cmd" .cmd)
  "$root/tests/walk.sh" "$game" "$name" || { fail=1; continue; }
  new=$root/tests/out/$game/$name.scr
  gold=$root/tests/$game/golden/$name.scr
  # the serial number is the build date, so ignore it
  norm() { grep -v 'Serial number' "$1"; }
  if [ "$2" = "--update" ]; then
    norm "$new" > "$gold"; echo "updated $name"
  elif norm "$new" | diff -u "$gold" - > "$root/tests/out/$game/$name.diff"; then
    echo "PASS $name"
  else
    echo "FAIL $name  (see tests/out/$game/$name.diff)"; fail=1
  fi
done
exit $fail
