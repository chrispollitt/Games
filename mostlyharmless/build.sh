#!/bin/sh
# Build one of the MOSTLY HARMLESS games.
#   ./build.sh mini           release build   -> mini/mini.z5
#   ./build.sh mini debug     debug build     -> mini/mini_debug.z5   (-D: debug verbs)
#   ./build.sh full [debug]   the full game   -> full/full.z8 (Z-machine version 8)
# The shared engine/ folder is on the include path for every game.
game=${1:-mini}
mode=${2:-release}
root=$(cd "$(dirname "$0")" && pwd)
cd "$root/$game" || exit 1
case "$game" in
  full) ext=z8; vflag=-v8 ;;
  *)    ext=z5; vflag=-v5 ;;
esac
case "$mode" in
  debug)   flags="-D $vflag"; out=${game}_debug.$ext ;;
  release) flags="$vflag";    out=${game}.$ext ;;
  *) echo "usage: $0 game [debug|release]" >&2; exit 2 ;;
esac
exec inform6 $flags +include_path=/usr/local/share/inform6/lib,"$root/engine",./ "$game.inf" "$out"
