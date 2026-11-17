#!/bin/sh
# Play one command file through a game and save a clean transcript.
#   tests/walk.sh <game> <name> [story-file]
# Reads tests/<game>/<name>.cmd, writes tests/out/<game>/<name>.scr using the
# game's own /SCRIPT command (so the transcript has no status line).
# Needs dfrotz (dumb frotz): set DFROTZ or have it on PATH (see README.md).
root=$(cd "$(dirname "$0")/.." && pwd)
game=$1; name=$2
case "$game" in full) ext=z8 ;; *) ext=z5 ;; esac
z5=${3:-$root/$game/${game}_debug.$ext}
dfrotz=${DFROTZ:-$(command -v dfrotz)}
out=$root/tests/out/$game
mkdir -p "$out"
tr=$out/$name.scr      # dfrotz adds ".scr" to the name we give it
rm -f "$tr"
{ echo /script; echo "$out/$name"; cat "$root/tests/$game/$name.cmd"
  echo quit; echo /quit; echo y; } | timeout 120 "$dfrotz" -q -m -w 80 "$z5" >/dev/null 2>&1
test -s "$tr" || { echo "no transcript for $name" >&2; exit 1; }
