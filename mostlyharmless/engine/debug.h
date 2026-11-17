!  debug.h -- verbs for testers.  Compiled only with the debug build (-D).
!
!  COMPLY n     set the cube's compliance counter
!  PUZZLES      list every Puzzle with its hint rung and state

#ifdef DEBUG;

Verb meta 'comply'
    * number                                -> SetComply;

Verb meta 'compliance'
    *                                       -> ShowComply;

Verb meta 'seed'
    * number                                -> SetSeed;

Verb meta 'egg'
    * number                                -> SetEgg;

Verb meta 'puzzles'
    *                                       -> ShowPuzzles;

[ SetComplySub;
    compliance = noun;
    if (compliance > 6) compliance = 6;
    print "Compliance is now ", compliance, ".^";
    rtrue;
];

[ SetSeedSub;
    random(-noun);                         ! make random() repeatable for tests (use 1000 or more: smaller seeds make a counter)
    print "Random numbers seeded with ", noun, ".^";
    rtrue;
];

[ SetEggSub;
    eg-->EG_FORCE = noun;
    print "The next XYZZY will give outcome ", noun, ".^";
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
