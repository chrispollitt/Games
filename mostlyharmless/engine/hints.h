!  hints.h -- progressive hints, difficulty, and the cube's compliance counter.
!
!  A game declares one Puzzle object per puzzle, in the order it expects the
!  player to meet them:
!
!      Puzzle p_order "Hob's lunch"
!        with hint1 "A nudge.",
!             hint2 "A clearer pointer.",
!             hint3 "Nearly the answer.",
!             hint4 "The answer.",
!             active [; return phase == 0; ],     ! is the player up to this?
!             done   [; return hob_fed; ];        ! optional: already solved?
!
!  /HINT finds the first active, unsolved puzzle (or the one with the highest
!  `priority`) and gives its next rung.  /HINT BACK re-reads the last rung.
!  Hints are out-of-story and never cost anything.
!
!  /DIFFICULTY EASY | AVERAGE | FAITHFUL
!    easy      unlimited hints, and a nudge offered after a long stall
!    average   /HINT on request, four rungs per puzzle
!    faithful  one rung per puzzle
!
!  Compliance (0-6) counts the "cube shortcuts" the player has accepted.  A
!  game calls Comply(1) when the player accepts one.  At 3 the cube changes
!  tone; at 4 or more the game should close the radio ending (see Comply).



Include "hintvars";

Class Puzzle
  with hint1 0, hint2 0, hint3 0, hint4 0,
       rung 0,                ! how many rungs have been shown
       active 0,              ! routine: is the player up to this puzzle?
       done 0,                ! routine (optional): is it solved?
       priority 0;            ! higher wins when several are active

! ------------------------------------------------------------------------
!  Hints
! ------------------------------------------------------------------------

[ HintMax;
    if (difficulty == DIFF_FAITHFUL) return 1;
    return 4;
];

[ HintText p r;
    switch (r) {
      1: print (string) p.hint1;
      2: print (string) p.hint2;
      3: print (string) p.hint3;
      4: print (string) p.hint4;
    }
];

! The puzzle the player is most likely stuck on (0 if none)
[ HintFind p best bp;
    best = 0;
    bp = -32000;
    objectloop (p ofclass Puzzle) {
        if (p.active == 0) continue;
        if (p.done ~= 0 && p.done()) continue;
        if (p.active() == 0) continue;
        if (p.priority > bp) { best = p; bp = p.priority; }
    }
    return best;
];

[ HintSub p r m;
    p = HintFind();
    if (p == 0) "There is nothing more to suggest.";
    m = HintMax();
    if (p.rung < m) p.rung++;
    r = p.rung;
    if (r > m) r = m;
    print "[Hint ", r, " of ", m, "] ";
    HintText(p, r);
    new_line;
    if (r == m) {
        if (difficulty == DIFF_FAITHFUL)
            print "[Faithful mode gives one hint per puzzle. /DIFFICULTY AVERAGE gives the full set.]^";
        else
            print "[That was the last hint for this one.]^";
    }
    rtrue;
];

[ HintBackSub p r;
    p = HintFind();
    if (p == 0) "There is nothing more to suggest.";
    r = p.rung;
    if (r == 0) "You haven't asked for a hint about this yet. /HINT gives the first.";
    if (r > HintMax()) r = HintMax();
    if (r > 1) r--;
    print "[Hint ", r, " of ", HintMax(), "] ";
    HintText(p, r);
    new_line;
    rtrue;
];

! Easy mode: offer a nudge after a long stall.  Call once a turn.
[ HintTick;
    if (score ~= hint_lastscore) { hint_lastscore = score; hint_idle = 0; return; }
    hint_idle++;
    if (difficulty == DIFF_EASY && hint_idle >= 20) {
        hint_idle = 0;
        print "^[Stuck? /HINT will give you a nudge.]^";
    }
];

! ------------------------------------------------------------------------
!  Difficulty
! ------------------------------------------------------------------------

[ DifficultyName;
    switch (difficulty) {
      1: print "EASY";
      2: print "AVERAGE";
      3: print "FAITHFUL";
    }
];

[ DifficultySub;
    print "Difficulty is ";
    DifficultyName();
    print ". /DIFFICULTY EASY, AVERAGE or FAITHFUL changes it.^";
    rtrue;
];

[ DifficultySetSub d;
    d = 0;
    if (TopicAny('easy', 'simple', 'gentle', 'casual', 'kind')) d = 1;
    else if (TopicAny('average', 'normal', 'medium', 'standard', 'default', 'ordinary')) d = DIFF_AVERAGE;
    else if (TopicAny('faithful', 'hard', 'original', 'infocom', 'difficult', 'tough', 'cruel')) d = 3;
    if (d == 0) "I didn't catch that. /DIFFICULTY EASY, AVERAGE or FAITHFUL.";
    difficulty = d;
    switch (d) {
      1: "Easy: hints are unlimited, and the game will offer a nudge if you have been stuck a while.";
      2: "Average: /HINT gives up to four steps for each puzzle, whenever you ask.";
      3: "Faithful: one hint per puzzle, and the game keeps quiet unless you ask.";
    }
];

! ------------------------------------------------------------------------
!  The cube's compliance
! ------------------------------------------------------------------------

! The player accepted n cube shortcuts
[ Comply n;
    compliance = compliance + n;
    if (compliance > 6) compliance = 6;
    if (n > 0) CubeTone();
];

[ CubeTone;
    if (compliance == 3)
        print "^The cube makes a small, satisfied sound. ~Good.~^";
    else if (compliance >= 4 && cube_warned == 0) {
        cube_warned = 1;
        print "^~Leave it with me,~ says the cube, kindly. It does not sound as though this was an offer.^";
    }
];

! True while the radio ending is still open as far as the cube is concerned
[ CubeAllowsRadio;
    if (compliance >= 4) rfalse;
    rtrue;
];
