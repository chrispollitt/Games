!  full_actions.h -- verbs and hooks specific to THE LONG WAY ROUND.

! SHARPEN <thing> WITH <stone>
[ SharpenSub;
    if (second ~= whetstone) "You'd need a proper stone for that.";
    if (noun == knife) "Your knife is already keen enough to embarrass a loaf.";
    if (noun ~= cleaver) "That isn't something that holds an edge.";
    if (cleaver.sharp) "You have already put an edge on it you could shave with.";
    if (location ~= Smokehouse) "The cleaver is in the smokehouse.";
    if (whetstone notin player) "You haven't got the stone in your hand.";
    cleaver.sharp = 1;
    move i_meat to cool_pit;
    Award(A_TRADE, 4);
    "You work the cleaver along the stone with long, patient strokes until it sings. Dunna comes over, tests the edge against a hair from her own arm, and is silent for a respectful moment. ~That'll do,~ she says. She whistles for her boy, who runs off with a side of cured meat. ~It's in your pit. Don't say I never gave you anything.~";
];

[ FlySub;
    if (location == Meadow && skiff in Meadow) {
        <Enter skiff>;
        rtrue;
    }
    "There is nothing here to fly.";
];

[ AboutSub;
    print "^MOSTLY HARMLESS: THE LONG WAY ROUND^^An unofficial interactive fan adaptation of the fifth Hitchhiker's Guide novel, in six acts. The story follows the main events of the book; all prose and puzzles are original. The Hitchhiker's Guide to the Galaxy and its characters belong to the estate of Douglas Adams. This is a non-commercial tribute.^^This is an early version: only Act I (Lamuella) is written so far.^";
    rtrue;
];

[ WalkToSub dest;
    dest = 0;
    if (TopicAny('sandwich', 'shop', 'stall', 'counter', 'work', 'kitchen')) dest = SandHut;
    else if (TopicAny('home', 'house', 'bed', 'bedroom', 'own', 'my')) dest = Hut;
    else if (TopicAny('meadow', 'landing', 'field', 'pad')) dest = Meadow;
    else if (TopicAny('river', 'bank', 'water', 'reeds')) dest = River;
    else if (TopicAny('orchard', 'slope', 'bushes', 'berries')) dest = Orchard;
    else if (TopicAny('smokehouse', 'smoker', 'dunna', 'smoke')) dest = Smokehouse;
    else if (TopicAny('lodge', 'reeve', 'tem', 'stone', 'hall')) dest = Lodge;
    else if (TopicAny('green', 'village', 'square', 'centre', 'center')) dest = Green;
    else if (TopicAny('skiff', 'ship')) {
        if (skiff in location) { <Enter skiff>; rtrue; }
        "There's no ship here to go to.";
    }
    else if (TopicAny('hut'))
        "Which hut: your own, or the sandwich hut?";
    if (dest == 0)
        "Go where? Try a place name, like GO TO THE MEADOW, or a compass direction.";
    if (dest == location)
        "You're already there.";
    if (dest ~= Green && location ~= Green
            && ~~(dest == River && location == Meadow) && ~~(dest == Meadow && location == River))
        PlayerTo(Green, 1);
    PlayerTo(dest);
    rtrue;
];

[ DeathMessage;
    switch (ending) {
      1: print "The Earth has been demolished. Again";
      2: print "The Earth has been demolished, with the books balanced";
      3: print "Rescued, improbably, by fish";
      default: print "The story has ended";
    }
];

[ PrintRank;
    print ", earning you the rank of ";
    if (score < 50) print "Passenger Without a Towel";
    else if (score < 120) print "Sandwich Maker";
    else if (score < 220) print "Reluctant Hitchhiker";
    else if (score < 320) print "Seasoned Hitchhiker";
    else if (score < 400) print "Mostly Harmless";
    else print "Remarkably Harmless";
    print ".^";
];
