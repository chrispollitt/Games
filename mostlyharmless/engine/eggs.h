!  eggs.h -- hidden surprises, never required and never harmful.
!
!  XYZZY rolls a die (about half good, half mildly annoying, none permanent);
!  PLUGH, PLOVER, GRUE, ZORK and PANIC get a sly line each.  The game supplies
!  two hooks:
!      EggRoomOK(r)  true if r is a room XYZZY may send the player back to
!      EggKeep(o)    true if o must never be dropped by XYZZY

[ EggSub w;
    w = verb_word;
    switch (w) {
      'xyzzy':  return Xyzzy();
      'plugh':  "A hollow voice says ~Plugh.~ You feel faintly outflanked.";
      'plover': "An emerald glow flickers at the edge of your vision, considers the situation, and thinks better of it.";
      'zork':   "You say it with feeling. Somewhere, a long way beneath your feet, something large and well-meaning stirs, and decides that it is not, on balance, a good time.";
      'grue':   "It is not dark here, and in any case the nearest grue is a very long way off and, by all accounts, has taken up vegetarianism. You are in no danger of being eaten.";
    }
    rfalse;
];

[ PanicSub;
    "Quite right. Don't.";
];

[ Xyzzy r n i k;
    if (eg-->EG_FORCE == 0 && (turns < 2 || (eg-->EG_COUNT > 0 && turns - eg-->EG_LAST < 10)))
        "Nothing happens. (The magic seems to need a moment.)";
    eg-->EG_LAST = turns;
    eg-->EG_COUNT = eg-->EG_COUNT + 1;
    r = random(12);
    if (eg-->EG_FORCE ~= 0) { r = eg-->EG_FORCE; eg-->EG_FORCE = 0; }
    switch (r) {
      1:  "A hollow voice says ~Fool.~";
      2:  "Nothing happens.";
      3:  print "A hollow voice, quite kindly, whispers:^";
          HintSub();
          rtrue;
      4:  n = 0;
          objectloop (i has visited) if (i ~= location && EggRoomOK(i)) n++;
          if (n == 0) "Nothing happens.";
          k = random(n);
          objectloop (i has visited) {
              if (i ~= location && EggRoomOK(i)) { k--; if (k == 0) break; }
          }
          print "The world makes a small, apologetic noise and rearranges itself.^";
          PlayerTo(i);
          rtrue;
      5:  n = 0;
          objectloop (i in player) if (i hasnt worn && EggKeep(i) == 0) n++;
          if (n == 0) "Nothing happens.";
          k = random(n);
          objectloop (i in player) {
              if (i hasnt worn && EggKeep(i) == 0) { k--; if (k == 0) break; }
          }
          move i to location;
          print "A sudden, unexplained weakness takes your fingers, and ", (the) i, " slips and lands at your feet.^";
          rtrue;
      6:  "In your pocket you find a small boiled sweet you do not remember buying. It tastes of nostalgia and is gone before you have decided whether to like it.";
      7:  "[Out of the story: XYZZY is a magic word from Colossal Cave Adventure, the first text adventure, begun by Will Crowther and extended by Don Woods in 1976-77. A great many later games have kept it as a tip of the hat.]";
      8:  "Your parser would like it noted that this was not in its manual, and that it is doing its best.";
      9:  if (guide has on && IndirectlyContains(player, guide))
              "The cube, in your pocket, writes on its face: ~I HEARD THAT. I FIND IT ENDEARING.~";
          "Somewhere nearby, something that takes notes makes a note.";
      10: "A pleasant voice, from nowhere in particular, says: ~Well done.~ It does not say for what.";
      11: "Somewhere a very long way off, someone small sighs, and thinks no more of it.";
      12: "You find a coin in your hand that was not there a moment ago. It is of no known denomination, and by the time you look again it is gone.";
    }
    rtrue;
];
