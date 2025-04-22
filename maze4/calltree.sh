#!/bin/bash

SRC="maze4.cc"
SRC2="${SRC%.cc}-tmp.cc"
perl -lpe 's/errno == EINTR/1/; s/#include <cerrno>//;' $SRC > ${SRC%.cc}-tmp.cc
FUNCS=$(perl -ne '/^\w+ (\w+).*\) \{/ and print "$1|"' $SRC)
FUNCS="\\b(${FUNCS%?})\\b"
/opt/schily/bin/calltree -m $SRC2 2>/dev/null | grep -P "$FUNCS"
