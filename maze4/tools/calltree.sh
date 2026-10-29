#!/bin/bash
#
# Generate function call tree by static code analysis
# Requires: https://github.com/linux-sir/calltree
#

SRC="maze4.cc"
FUNCS=$(perl -ne '/^\w+ (\w+).*\) \{/ and print "$1|"' $SRC)
FUNCS="\\b(${FUNCS%?})\\b"
/opt/schily/bin/calltree -m $SRC 2>/dev/null | grep -P "$FUNCS" > calltree.txt
less -iS calltree.txt
