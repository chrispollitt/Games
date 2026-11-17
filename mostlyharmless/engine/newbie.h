!  newbie.h -- two short overviews for newcomers, both out of the story:
!
!  /INTRO   what interactive fiction is and how to play it
!  /HHGG    a spoiler-free account of The Hitchhiker's Guide to the Galaxy
!
!  The game supplies GameBlurb(), a paragraph on where it fits in the franchise.

[ IntroSub;
    print "^[Out of the story: A SHORT INTRODUCTION TO INTERACTIVE FICTION^^";
    print "This is interactive fiction: a story told in text, which you steer by typing. You read a description of where you are, then type what you want to do, and the story answers. There are no graphics and no clock; take as long as you like.^^";
    print "How to play^";
    print "  Look around: LOOK (or L). Study something: EXAMINE SKETCH (or X SKETCH).^";
    print "  Move: NORTH, SOUTH, EAST, WEST (or N, S, E, W), sometimes UP, DOWN, IN or OUT; or GO TO THE MEADOW.^";
    print "  Pick things up and use them: TAKE KNIFE, DROP IT, GIVE SANDWICH TO HOB, PUT LOAF ON BOARD. INVENTORY (or I) lists what you carry.^";
    print "  Talk: ASK HOB ABOUT LUNCH, or just ask a question such as WHO IS FORD PREFECT?^";
    print "  Let time go by: WAIT.^^";
    print "Commands can be long or short. This game copes with plain English, filler words, punctuation, small typos, and commands joined by AND or THEN (TAKE PARCEL AND EXAMINE IT).^^";
    print "Commands about the game itself start with a slash: /HINT (a nudge that gets clearer each time you ask), /DIFFICULTY (how much help you get), /SAVE and /RESTORE, /UNDO (take back your last move), /MACRO (record a routine you repeat), and /HELP for the rest.^^";
    print "Good habits: read descriptions closely, because clues hide in them; examine anything with a name; talk to everyone about everything; try the obvious thing and then the silly one. If you are stuck, wander, look again, or ask for a hint. Nothing you try can make this story unwinnable; the worst that happens is a sadder ending.]^";
    rtrue;
];

[ HhggSub;
    print "^[Out of the story: THE HITCHHIKER'S GUIDE TO THE GALAXY, FOR NEWCOMERS^^";
    print "Douglas Adams (1952-2001) began The Hitchhiker's Guide to the Galaxy as a BBC Radio 4 comedy that first went out in March 1978. Its hero is Arthur Dent, an ordinary Englishman who is whisked off the Earth, moments before it is demolished to make way for a hyperspace bypass, by his friend Ford Prefect, who turns out to be an alien researcher for the Guide: a best-selling electronic travel book with the words DON'T PANIC on its cover.^^";
    print "It grew into what its author called a trilogy in five parts: five novels (1979, 1980, 1982, 1984 and 1992), a television series (1981), Infocom's text adventure (1984), stage shows, comics, a feature film (2005) and radio adaptations of the later books (2004-2005). A sixth novel, And Another Thing..., by Eoin Colfer, appeared in 2009 with the Adams estate's approval. Every version retells the story a little differently, and fans take that as part of the joke, so an adaptation like this one is in good company.^^";
    print "A few things worth knowing: the Babel fish (a small yellow fish that, in your ear, translates every language you hear); the towel (the most useful thing a hitchhiker can carry); the Vogons (the Galaxy's bureaucrats, whose poetry is said to be the third worst in the Universe); and the number 42. The people you will meet include Ford Prefect, Trillian, Zaphod Beeblebrox and Marvin the Paranoid Android.^^";
    GameBlurb();
    print "^You do not need to have read any of it; the game tells you what you need. This is an unofficial fan work, with entirely original text.]^";
    rtrue;
];
