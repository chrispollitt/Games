!  mishmash.h -- the Whole Sort of General Mish Mash rolls the dice.
!
!  Whenever a turn is completed by a command that contains the word "random"
!  (which is also the name of Arthur's daughter, so it turns up a good deal),
!  the Mish Mash rolls a twelve-sided die and something small may happen.
!  It is flavour only: it never changes puzzle state, score, trust, compliance
!  or an ending, it is rate-limited, it keeps quiet while a macro replays or
!  when the game says so (MishQuiet), and it can be switched off with /DICE OFF.
!  The roll uses random(), so it is repeatable under the debug SEED command.
!
!  The game supplies MishQuiet(): true when an event would be out of place.

! Called from BeforeParsing: does this line contain the word "random"?
[ MishScan  i nw;
    eg-->EG_RFLAG = 0;
    if (slash_on) return;
    nw = NumberWords();
    for (i = 1 : i <= nw : i++)
        if (WordValue(i) == 'random') { eg-->EG_RFLAG = 1; return; }
];

! Called once a turn, after the action (from the game's daemon)
[ MishRoll  r;
    if (eg-->EG_RFLAG == 0) return;
    eg-->EG_RFLAG = 0;
    if (eg-->EG_DICE ~= 0) return;                 ! switched off
    if (mv-->MV_STATE == 2) return;                ! a macro is replaying
    if (deadflag ~= 0) return;
    if (MishQuiet()) return;
    if (eg-->EG_RCOUNT > 0 && turns - eg-->EG_RLAST < 5) return;
    eg-->EG_RLAST = turns;
    eg-->EG_RCOUNT = eg-->EG_RCOUNT + 1;
    r = random(12);
    if (r == 8 || r == 11) print "^[The Mish Mash rolls an ", r, ".]^";
    else print "^[The Mish Mash rolls a ", r, ".]^";
    switch (r) {
      1:  "For a moment something in the room is a slightly different colour. Then it isn't.";
      2:  if (guide has on && IndirectlyContains(player, guide))
              "The cube in your pocket writes, in small neat capitals: ~THE GUIDE HAS NOTED THE WORD. THE GUIDE NOTES EVERYTHING.~";
          "Somewhere nearby, something that takes notes makes a note.";
      3:  if (Daughter in location)
              "Random glances up. ~Did somebody say something?~ she asks, and then, when nobody answers, looks faintly pleased to have been right.";
          "Somewhere, a girl feels, briefly and for no reason, that she has been talked about.";
      4:  "From a long way off comes the sound of a die landing on a very large table.";
      5:  "The word hangs in the air a moment longer than words do, and then gets on with its life.";
      6:  "The die lands on its edge, which seems rude of it. Nothing at all happens, extremely politely.";
      7:  "You have the passing impression that this is the sort of thing that would be funnier if the universe were slightly less tidy.";
      8:  "A thin smell of ozone and toast, and then not.";
      9:  "Your shadow, for an instant, is looking the wrong way. It thinks better of it.";
      10: "You remember, quite clearly, a lunch you did not eat. It was good. It is gone.";
      11: "[Out of the story: games like this never roll real dice. A call to a random-number routine picks the next number from a sequence that only looks haphazard; start it from the same seed and it gives the same rolls every time, which is how this game is tested.]";
      12: "Half a heartbeat of something else: a version of this room in which the roof is on fire, the walls are painted yellow, and everyone you have ever met is standing in it, talking. It is not like that. It never was.";
    }
    rtrue;
];

[ DiceSub;
    if (eg-->EG_DICE ~= 0) "The dice are off. /DICE ON brings them back.";
    "The Mish Mash rolls a die whenever you type the word random. /DICE OFF stops it.";
];

[ DiceOnSub;
    eg-->EG_DICE = 0;
    "(The dice are on.)";
];

[ DiceOffSub;
    eg-->EG_DICE = 1;
    "(The dice are off.)";
];
