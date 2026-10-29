#!/bin/sh
# Serve maze4 in a browser tab through ttyd (xterm.js).
# Read-only (no -W: viewers cannot type) and bound to one interface, "lo" by
# default, so put an SSH tunnel or a reverse proxy in front of it.
#
#   ./ttyd/run-ttyd.sh                  # http://127.0.0.1:7681/
#   PORT=8081 MAZE4_ARGS="-g80 -t4" ./ttyd/run-ttyd.sh
#
# Each browser tab starts its own game.  A tab smaller than maze4's minimum
# size ends the game at once; reload with a bigger window.
here=$(cd "$(dirname "$0")" && pwd)
exec "${TTYD:-ttyd}" -i "${IFACE:-lo}" -p "${PORT:-7681}" -m "${MAX_CLIENTS:-10}" \
  -T xterm-256color ${BASE:+-b "$BASE"} \
  "$here/../src/maze4" ${MAZE4_ARGS:--g50}
