!  sandwich.h -- the sandwich engine for THE LONG WAY ROUND.
!
!  Ingredients are objects of class Ingredient; each has a bit (BIT_...), a
!  `home` where it goes back to after use, and a `missing` line explaining
!  where to find more.  The chopping board holds the loaf and the ingredients;
!  MAKE SANDWICH combines them.  Customers are NPCs with `wants` (the bits
!  they will accept), `sorry` (what they say about a wrong sandwich) and
!  `eaten` (a routine run when they are given the right one).

Constant BIT_MEAT   = 1;
Constant BIT_BERRY  = 2;
Constant BIT_PICKLE = 4;
Constant BIT_HONEY  = 8;
Constant BIT_CURD   = 16;

Global sand_flags = 0;        ! what the current sandwich is made of

Class Ingredient
  with ibit 0,
       home 0,
       missing "You haven't any of that to hand.",
       article "some",
       before [;
         Drop:
           if (location ~= SandHut) {
               SendHome(self);
               "You can hardly leave the larder lying about; you tuck it back where it belongs.";
           }
       ];

! What is on the chopping board, as a bit pattern
[ BoardFlags o f;
    f = 0;
    objectloop (o in board)
        if (o ofclass Ingredient) f = f + o.ibit;
    return f;
];

! Send an ingredient back where it comes from
[ SendHome o;
    move o to o.home;
];


! "meat, sourberries and pickled roots"
[ SandwichDesc lc n i;
    if (lc) print "a sandwich of "; else print "A sandwich of ";
    n = 0;
    if (sand_flags & BIT_MEAT)   n++;
    if (sand_flags & BIT_BERRY)  n++;
    if (sand_flags & BIT_PICKLE) n++;
    if (sand_flags & BIT_HONEY)  n++;
    if (sand_flags & BIT_CURD)   n++;
    if (n == 0) { print "nothing in particular, between two thick slabs of Lamuellan bread"; return; }
    i = 0;
    if (sand_flags & BIT_MEAT)   { i++; SandwichItem("meat", n, i); }
    if (sand_flags & BIT_BERRY)  { i++; SandwichItem("sourberries", n, i); }
    if (sand_flags & BIT_PICKLE) { i++; SandwichItem("pickled roots", n, i); }
    if (sand_flags & BIT_HONEY)  { i++; SandwichItem("honey", n, i); }
    if (sand_flags & BIT_CURD)   { i++; SandwichItem("herb curd", n, i); }
    print ", between two thick slabs of Lamuellan bread";
];

[ SandwichItem s n i;
    if (i > 1) { if (i == n) print " and "; else print ", "; }
    print (string) s;
];

[ MakeSub f;
    if (location ~= SandHut)
        "Sandwiches are made at the sandwich hut; that is rather the point of it.";
    StraySandwich();
    if (parent(sandwich) ~= nothing)
        "There is a sandwich already on the go. Finish with that one first.";
    if (loaf notin board)
        "You will want the loaf on the chopping board first.";
    if (knife notin player)
        "A sandwich maker without his knife is a man with a loaf and a theory. Pick it up.";
    f = BoardFlags();
    if (f == 0)
        "Bread on bread is a very honest sandwich, but it is not what anyone is waiting for. Put something else on the board with the loaf.";
    sand_flags = f;
    ClearBoard();
    move loaf to counter;
    move sandwich to board;
    print "You slice, stack and press, with the unhurried competence of long practice. The result is ";
    SandwichDesc(1);
    print ".^";
    rtrue;
];

! Is this ingredient somewhere the sandwich maker can reach?
[ IngAvailable o;
    if (o in board || o in cool_pit || o in pantry) rtrue;
    if (IndirectlyContains(player, o)) rtrue;
    rfalse;
];

! Give a sandwich to a customer; called from their `before`
[ ServeSandwich c;
    if ((c provides wantany && (sand_flags & c.wantany) == 0) || (c provides wants && sand_flags ~= c.wants)) {
        print_ret (The) c, " ", (string) c.sorry;
    }
    remove sandwich;
    c.eaten();
    rtrue;
];

! Free-form orders:  MAKE A SANDWICH WITH MEAT AND BERRIES, MAKE HOB THE USUAL,
! MAKE ME SOMETHING WITH PICKLES BUT NO MEAT ...
[ MakeFreeSub w neg have ex bits who i sawsand;
    wn = consult_from;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 'sandwich' or 'sarnie' or 'lunch' or 'snack' or 'meal' or 'usual' or 'regular')
            sawsand = 1;
        if (w == 'no' or 'without' or 'hold' or 'not' or 'except' or 'minus'
                or 'skip' or 'omit' or 'nothing' or 'lose' or 'dont')
            neg = 1;
        else if (w == 'with' or 'but' or 'plus' or 'add' or 'including' or 'extra' or 'then')
            neg = 0;
        if (w == 'meat' or 'ham' or 'cured' or 'jerky') {
            if (neg) ex = ex | BIT_MEAT; else have = have | BIT_MEAT;
        }
        if (w == 'berry' or 'berries' or 'sourberry' or 'sourberries' or 'sour' or 'purple') {
            if (neg) ex = ex | BIT_BERRY; else have = have | BIT_BERRY;
        }
        if (w == 'pickle' or 'pickles' or 'pickled' or 'root' or 'roots') {
            if (neg) ex = ex | BIT_PICKLE; else have = have | BIT_PICKLE;
        }
        if (w == 'honey' or 'sweet') {
            if (neg) ex = ex | BIT_HONEY; else have = have | BIT_HONEY;
        }
        if (w == 'curd' or 'herb' or 'herbs') {
            if (neg) ex = ex | BIT_CURD; else have = have | BIT_CURD;
        }
        if (w == 'usual' or 'regular') have = have | BIT_MEAT | BIT_BERRY;
        if (w == 'works' or 'everything') have = BIT_MEAT | BIT_BERRY | BIT_PICKLE;
        if (w == 'hob' or 'hobs') who = hob;
        if (w == 'pilot' or 'courier' or 'pilots') who = pilot;
        if (w == 'random' or 'girl' or 'daughter' or 'randoms') who = Daughter;
        if (w == 'tem' or 'reeve' or 'tems') who = Tem;
        if (w == 'ford' or 'fords') who = Ford;
    }
    bits = have & ~ex;
    if (sawsand == 0 && bits == 0)
        "Make what? Sandwiches are the house speciality. Try MAKE A SANDWICH WITH MEAT AND SOURBERRIES.";
    if (location ~= SandHut)
        "Sandwiches are made at the sandwich hut; that is rather the point of it.";
    if (who ~= 0 && who notin location)
        "There's nobody here to make it for.";
    StraySandwich();
    if (parent(sandwich) ~= nothing)
        "There is a sandwich already on the go. Finish with that one first.";
    if (bits == 0 && BoardFlags() == 0)
        "What goes in it? Have a look at what is in the hut and what the customer would like, then say so: for example MAKE A SANDWICH WITH MEAT AND SOURBERRIES.";
    if (bits ~= 0) {
        ! check the larder before laying anything out
        if ((bits & BIT_MEAT) && IngAvailable(i_meat) == 0)     { print_ret (string) i_meat.missing; }
        if ((bits & BIT_BERRY) && IngAvailable(i_berries) == 0) { print_ret (string) i_berries.missing; }
        if ((bits & BIT_PICKLE) && IngAvailable(i_pickles) == 0){ print_ret (string) i_pickles.missing; }
        if ((bits & BIT_HONEY) && IngAvailable(i_honey) == 0)   { print_ret (string) i_honey.missing; }
        if ((bits & BIT_CURD) && IngAvailable(i_curd) == 0)     { print_ret (string) i_curd.missing; }
        ClearBoard();
        if (bits & BIT_MEAT)   move i_meat to board;
        if (bits & BIT_BERRY)  move i_berries to board;
        if (bits & BIT_PICKLE) move i_pickles to board;
        if (bits & BIT_HONEY)  move i_honey to board;
        if (bits & BIT_CURD)   move i_curd to board;
    }
    if (knife notin player) {
        move knife to player;
        print "(first taking the knife)^";
    }
    if (loaf notin board) move loaf to board;
    MakeSub();
    if (who ~= 0 && parent(sandwich) == board) {
        move sandwich to player;
        <Give sandwich who>;
    }
    rtrue;
];

[ ClearBoard o nxt;
    o = child(board);
    while (o ~= 0) {
        nxt = sibling(o);
        if (o ofclass Ingredient) SendHome(o);
        o = nxt;
    }
];

! One line of "On the counter is a loaf of bread." for the hut's description
[ HutShelf o prefix;
    if (child(o) == 0) return;
    print (string) prefix, " ";
    WriteListFrom(child(o), ENGLISH_BIT + ISARE_BIT);
    print ".^";
];

! A forgotten sandwich lying in some other room is quietly cleared away, so
! that it can never block the next one.
[ StraySandwich;
    if (parent(sandwich) == nothing) return;
    if (parent(sandwich) == board || parent(sandwich) == location) return;
    if (IndirectlyContains(player, sandwich)) return;
    remove sandwich;
];
