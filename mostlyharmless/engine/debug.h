!  debug.h -- verbs for testers.  Compiled only with the debug build (-D).
!
!  COMPLY n     set the cube's compliance counter
!  PUZZLES      list every Puzzle with its hint rung and state

#ifdef DEBUG;

Verb meta 'comply'
    * number                                -> SetComply;

Verb meta 'compliance'
    *                                       -> ShowComply;

Verb meta 'puzzles'
    *                                       -> ShowPuzzles;

[ SetComplySub;
    compliance = noun;
    if (compliance > 6) compliance = 6;
    print "Compliance is now ", compliance, ".^";
    rtrue;
];

[ ShowComplySub;
    print "Compliance is ", compliance, ".^";
    rtrue;
];

[ ShowPuzzlesSub p;
    objectloop (p ofclass Puzzle) {
        print (name) p, ": rung ", p.rung;
        if (p.active ~= 0 && p.active()) print ", active";
        if (p.done ~= 0 && p.done()) print ", done";
        new_line;
    }
    rtrue;
];

#endif;
