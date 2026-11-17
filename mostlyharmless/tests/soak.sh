#!/bin/bash
# Random-input soak test: throws random commands at a game and looks for
# interpreter or library errors ("Programming error", "runtime", ...).
#   tests/soak.sh <game> <act> [runs] [commands-per-run]
# For the full game each run starts with a debug "ACT n" jump; for the mini
# game <act> is ignored.  Uses the debug build (build it first).
root=$(cd "$(dirname "$0")/.." && pwd)
game=${1:-full}
act=${2:-1}
runs=${3:-30}
len=${4:-60}
case "$game" in full) ext=z8 ;; *) ext=z5 ;; esac
story=$root/$game/${game}_debug.$ext
dfrotz=${DFROTZ:-$(command -v dfrotz)}
out=$root/tests/out/soak
mkdir -p "$out"

verbs=(take drop examine open close wear search read push pull turn enter wait look inventory
       eat smell touch climb ask tell give put show consult make fill stamp type dial compose
       draw order sit toast sharpen cut talk hello compliment apologise warn duck)
nouns=(fish jar basket gown knife loaf bread meat berries pickles honey curd sandwich board counter pit
       hob tem dunna pilot skiff trillian random ford tricia marvin fenchurch parcel cord note guide cube
       rock stone whetstone cleaver coin ticket towel form pen stamp machine clerk attendant gruba kiosk
       badge terminal kess pim lift lifts zendra orlo desk drawer key ledger memo file stacks boardroom core
       credential van officer cup wrapper stub map dee diary earring photo sketch pencil laser card matchbook
       man barman grebulons glass door table window)
dirs=(n s e w ne nw se sw u d in out)
topics=("lunch" "the guide" "random" "the cube" "ford" "the vogons" "food" "the towel" "the party" "herself"
        "her mother" "the button" "the chart" "the plan" "norway" "the worst" "diodes" "the sketch")

pick() { local a=("$@"); echo "${a[$((RANDOM % ${#a[@]}))]}"; }

bad=0
for ((r = 1; r <= runs; r++)); do
  RANDOM=$((act * 1000 + r))
  cmd=$out/${game}_a${act}_r$r.cmd
  : > "$cmd"
  if [ "$game" = full ] && [ "$act" -ge 2 ]; then echo "act $act" >> "$cmd"; fi
  # start from a random point part-way through the act's spine, so that
  # the random commands meet puzzles in mid-flight
  spine=$root/tests/$game/act$act.cmd
  [ "$game" = full ] && [ "$act" = 5 ] && spine=$root/tests/full/act5_booka.cmd
  if [ -f "$spine" ]; then
    n=$(grep -vc '^act ' "$spine")
    k=$((RANDOM % (n + 1)))
    grep -v '^act ' "$spine" | head -n $k >> "$cmd"
  fi
  for ((i = 0; i < len; i++)); do
    case $((RANDOM % 10)) in
      0|1) echo "$(pick "${dirs[@]}")" ;;
      2|3) echo "$(pick take drop examine open wear search) $(pick "${nouns[@]}")" ;;
      4)   echo "ask $(pick "${nouns[@]}") about $(pick "${topics[@]}")" ;;
      5)   echo "give $(pick "${nouns[@]}") to $(pick "${nouns[@]}")" ;;
      6)   echo "put $(pick "${nouns[@]}") $(pick in on) $(pick "${nouns[@]}")" ;;
      7)   echo "$(pick "${verbs[@]}") $(pick "${nouns[@]}")" ;;
      8|9) echo "$(pick xyzzy xyzzy plugh plover grue panic sing)" ;;
    esac
  done >> "$cmd"
  echo "/hint" >> "$cmd"
  res=$out/${game}_a${act}_r$r.txt
  timeout 30 "$dfrotz" -q -m -w 80 "$story" < "$cmd" > "$res" 2>&1
  pat='Programming error|runtime error|\[\*\*|tried to (read|write|call|use)|not a valid'
  if grep -qE "$pat" "$res"; then
    echo "PROBLEM  $game act $act run $r: $(grep -E "$pat" "$res" | head -2 | tr '\n' ' ')"
    bad=1
  fi
done
[ $bad -eq 0 ] && echo "soak OK: $game act $act, $runs runs of $len commands"
exit $bad
