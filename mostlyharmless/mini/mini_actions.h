!  mini_actions.h -- mini-game verbs: fly, free-form sandwiches, walking to places.

[ FlySub;
    if (location == Meadow && skiff in Meadow) {
        <Enter skiff>;
        rtrue;
    }
    "There is nothing here to fly.";
];

! Free-form sandwich orders:  MAKE A SANDWICH WITH MEAT AND BERRIES,
! MAKE HOB THE USUAL, MAKE ME SOMETHING WITH PICKLES BUT NO MEAT ...
[ MakeFreeSub w neg have ex bits who i sawsand;
    wn = consult_from;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 'sandwich' or 'sarnie' or 'lunch'
                or 'snack' or 'meal' or 'usual' or 'regular')
            sawsand = 1;
        if (w == 'no' or 'without' or 'hold' or 'not' or 'except' or 'minus'
                or 'skip' or 'omit' or 'nothing' or 'lose' or 'dont')
            neg = 1;
        else if (w == 'with' or 'but' or 'plus' or 'add' or 'including'
                or 'extra' or 'then')
            neg = 0;
        if (w == 'meat' or 'ham' or 'cured') {
            if (neg) ex = ex | BIT_MEAT; else have = have | BIT_MEAT;
        }
        if (w == 'berry' or 'berries' or 'sourberry' or 'sourberries'
                or 'sour' or 'purple') {
            if (neg) ex = ex | BIT_BERRY; else have = have | BIT_BERRY;
        }
        if (w == 'pickle' or 'pickles' or 'pickled' or 'root' or 'roots') {
            if (neg) ex = ex | BIT_PICKLE; else have = have | BIT_PICKLE;
        }
        if (w == 'usual' or 'regular') have = have | BIT_MEAT | BIT_BERRY;
        if (w == 'veggie' or 'vegetarian') have = have | BIT_BERRY | BIT_PICKLE;
        if (w == 'works' or 'everything') have = BIT_MEAT | BIT_BERRY | BIT_PICKLE;
        if (w == 'hob' or 'hobs') who = hob;
        if (w == 'pilot' or 'courier' or 'pilots') who = pilot;
        if (w == 'random' or 'girl' or 'daughter' or 'randoms') who = Daughter;
        if (w == 'ford' or 'fords') who = Ford;
    }
    bits = have & ~ex;
    if (sawsand == 0 && bits == 0)
        "Make what? Sandwiches are the house speciality. Try MAKE A SANDWICH WITH MEAT AND SOURBERRIES.";
    if (location ~= SandHut)
        "Sandwiches are made at the sandwich hut; that is rather the point of it.";
    if (who ~= 0 && who notin location)
        "There's nobody here to make it for.";
    if (parent(sandwich) ~= nothing)
        "There is a sandwich already on the go. Finish with that one first.";
    if (bits == 0 && meat notin board && berries notin board && pickles notin board)
        "What goes in it? You have meat, sourberries and pickled roots, in any combination. For example: MAKE A SANDWICH WITH SOURBERRIES AND PICKLED ROOTS.";
    if (knife notin player) {
        move knife to player;
        print "(first taking the knife)^";
    }
    if (bits ~= 0) {
        ! start from a clean board, then lay out exactly what was asked for
        if (meat in board) move meat to cold_pit;
        if (berries in board) move berries to pantry;
        if (pickles in board) move pickles to pantry;
        if (bits & BIT_MEAT) move meat to board;
        if (bits & BIT_BERRY) move berries to board;
        if (bits & BIT_PICKLE) move pickles to board;
    }
    if (loaf notin board) move loaf to board;
    MakeSub();
    if (who ~= 0 && parent(sandwich) == board) {
        move sandwich to player;
        <Give sandwich who>;
    }
    rtrue;
];

[ RegionOf r;
    if (r == Hut or Green or SandHut or Meadow) return 1;
    if (r == Site or Bar) return 2;
    return 0;
];

[ WalkToSub dest;
    dest = 0;
    if (TopicAny('sandwich', 'shop', 'stall', 'counter', 'work', 'kitchen')) dest = SandHut;
    else if (TopicAny('home', 'house', 'bed', 'bedroom', 'own', 'my')) dest = Hut;
    else if (TopicAny('meadow', 'landing', 'field', 'pad')) dest = Meadow;
    else if (TopicAny('green', 'village', 'square', 'centre', 'center')) dest = Green;
    else if (TopicAny('pub', 'bar', 'gnu', 'dancing', 'drink')) dest = Bar;
    else if (TopicAny('park', 'lawn', 'site', 'outside', 'cordon', 'tape')) dest = Site;
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
    if (RegionOf(dest) ~= RegionOf(location))
        "That isn't somewhere you can walk to from here.";
    if (RegionOf(dest) == 1 && dest ~= Green && location ~= Green)
        PlayerTo(Green, 1);
    PlayerTo(dest);
    rtrue;
];
