!  fulldebug.h -- tester verbs for the full game (debug builds only).
!
!  ACT n     jump to the start of act n (2 to 5) with everything the earlier
!            acts would have left you

#ifdef DEBUG;

Verb meta 'act'
    * number                                -> JumpAct;

! Common state: as if Act I had been completed
[ JumpBase;
    hob_fed = 1; tem_fed = 1; random_fed = 1; pilot_fed = 1;
    phase = 5;
    remove cord;
    give parcel open;
    move guide to player;
    guide_revealed = 1;
    give guide on;
    move knife to player;
    move basket to player;
    move coin to player;
    move jar to player;
    move fish to jar;
    trust = 1;
];

[ JumpActSub;
    if (noun == 2) {
        JumpBase();
        score = 60;
        StartAct2();
        rtrue;
    }
    if (noun >= 3) {
        JumpBase();
        remove coin;
        move towel to player;
        form_state = 3; cleared = 1;
        score = 110;
        if (noun == 3) { StartAct3(); rtrue; }
        remove jar;
        move jar to player;
        score = 210;
        lift_ok = 1; orlo_moved = 1; heard_meeting = 1; have_coords = 1; desk_open = 1;
        if (noun == 4) { StartAct4(); rtrue; }
        ! Act 5: as if Act IV were completed
        score = 300;
        trust = 4;
        Tricia.convinced = 1;
        random_gone = 1; random_caught = 1;
        random_follows = 1; tricia_follows = 1; ford_follows = 1;
        act = 4;
        move Ford to Bar; move Tricia to Bar; move Daughter to Bar;
        if (noun == 6) { score = 360; StartAct6(); rtrue; }
        StartAct5();
        move player to Bar;
        location = Bar;
        PlayerTo(Bar);
        rtrue;
    }
    "ACT 2, 3, 4, 5 or 6.";
];

#endif;
