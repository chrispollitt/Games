!  act5.h -- Act V: the Dancing Gnu and the lawn.  Five rooms, six puzzles, 60 points.
!
!  Snug -- Bar -- Pub Yard -- Lawn          (the Bar's door leads south to the High Road;
!           |                                a hatch leads down to the Cellar)
!         Cellar
!
!  The accident with the laser pistol always happens (decided).  What the
!  player controls is how ready everyone is, and what happens next.

Object matchbook "matchbook"
  with name 'matchbook' 'matches' 'match' 'book' 'matchbox',
       description [;
         if (matchbook_read == 0) {
             matchbook_read = 1;
             Award(A_MATCH, 15);
             doom_t = after_t + 5; if (difficulty == DIFF_FAITHFUL) doom_t = after_t + 4;
             "The cover bears the pub's name and an address. Inside, in the cramped hand of a man who kept careful count of everything, are a date and a time. You know the date. You know the time. Years ago, in some other context, a furious voice told you where it expected to meet its last end, and at what hour: here, this table, this minute. A creature who had come back and back and back, as one small animal after another, each of them killed by something you happened to be doing. The score is settled. Whatever it was, it is finished. For the first time since a bulldozer in a country lane, a very old knot inside you comes loose.^^Ford has been reading over your shoulder. He begins to laugh, the laugh of a man who has just seen the last piece of a very long joke arrive.";
         }
         "The cover bears the pub's name and an address. You have already read what's inside it.";
       ],
       initial [;
         if (matchbook_read == 0) "A matchbook lies on the floor near the corner table.";
         "The matchbook you read lies on the floor.";
       ];

Object laser "laser pistol"
  with name 'laser' 'pistol' 'gun' 'weapon' 'device',
       description "A short ugly weapon with a safety catch Random has not yet read about.",
       before [;
         Take, Pull:
           if (self in Daughter) {
               FireGun(1);
               rtrue;
           }
       ];

Object card "instruction card"
  with name 'card' 'instruction' 'instructions' 'manual' 'leaflet',
       description [;
         card_read = 1;
         "A creased laminated card, tucked behind the cushions in the Snug: SAFETY INSTRUCTIONS FOR A MODEL 7 PERSONAL LASER (ADVANCED). 1. Do not point at anything you value. 2. The dial marked STUN is the only dial that is safe; the others are optimistic. 3. The beam swings anticlockwise from the muzzle when the weapon is tilted; in the event of a mishap, everyone in the room should get down at once. 4. In the event of a mishap, do not blame the weapon.";
       ];

Object Bar "The Dancing Gnu"
  with description [;
         print "A pub that has been trying to look historic since about last Tuesday. Low beams, dark wood, a carpet whose pattern has given up. The door is south onto the High Road; a snug lies west, a hatch in the floor leads down to the cellar, and a back door opens east onto a yard.";
         if (gun_fired == 0)
             print " At a small corner table, a man in a damp raincoat drinks alone, with the look of someone who is waiting for something to happen, again.";
         else
             print " Everyone who is not under a table is trying to be.";
         new_line;
         rtrue;
       ],
       before [;
         Sing:
           print "You sing, quietly, a verse of something you half-remember. The man at the corner table looks up. Behind the bar the barman stops polishing his glass, and for a moment the whole pub is listening. Then the moment goes, as moments do, and the barman goes back to the glass, but a little more gently than before.^";
           rtrue;
       ],
       s_to HighRoad, out_to HighRoad,
       w_to Snug, d_to Cellar, e_to PubYard,
  has light;

Object corner_table "corner table" Bar
  with name 'table' 'corner' 'chairs',
       description "A small round table by the wall, with four chairs and a view of the whole pub. Someone has already picked the best seat.",
  has scenery supporter;

Object man "man in the raincoat"
  with name 'man' 'raincoat' 'stranger' 'patron' 'drinker' 'agrajag',
       description [;
         if (gun_fired) "He is down, and has the peaceful expression of someone who has finished a very long errand. It is, you will think later, the quietest end anybody has had in that pub.";
         "He has the face of someone who has been a great many things, none of them for very long.";
       ],
       life [;
         Ask, Answer, Tell:
           if (awarded->A_AGR1 == 0) {
               Award(A_AGR1, 5);
               "The man in the raincoat looks at you with enormous tiredness. ~I am,~ he says, ~someone who has been waiting a very long time to see you. Not today, though. Not quite. You will know me by what I leave behind.~ He turns back to his glass, politely, like a man who has said as much as is proper.";
           }
           "~Not today,~ says the man in the raincoat, gently.";
       ],
  has animate male scenery;

Object barman "barman" Bar
  with name 'barman' 'landlord' 'bartender' 'publican',
       description "A solid man in a waistcoat with a face that has heard everything, doubted most of it, and polished a glass in the meantime.",
       initial "The barman polishes a glass, with the air of a man whose job is not to ask. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('sandwich', 'food', 'eat', 'lunch', 'hungry', 'order', 'ploughman')) {
               if (parent(sandwich) ~= nothing) "~You've got one already,~ says the barman, not unkindly.";
               sand_flags = BIT_MEAT + BIT_PICKLE;
               move sandwich to player;
               "~Ploughman's,~ says the barman, and produces a plain pub sandwich of ham and pickle from under a cloth. It is not a sandwich that Lamuella would recognise, but it is honest. ~On the house,~ he says, ~given the day we're having.~";
           }
           if (TopicAny('man', 'raincoat', 'stranger', 'regular'))
               "~Comes in every few years,~ says the barman. ~Never says why. Always the same table. Always pays in exact change.~";
           if (TopicAny('snug', 'cellar', 'yard', 'laser', 'gun'))
               "~Snug's through there,~ says the barman. ~Cellar's below. Yard's out the back. If anything alarming happens, I'm in the cellar.~";
           "~Can't help you,~ says the barman, polishing.";
       ],
  has animate male scenery;

Object Snug "The Snug"
  with description
         "A tiny back room barely big enough for two people and an opinion, with a bench seat and a table and a window onto a brick wall. The bar is east.",
       e_to Bar,
  has light;

Object Cellar "The Cellar"
  with description
         "Kegs and crates in a cool brick cellar that smells of malt and old secrets. A steep stair leads up to the bar.",
       u_to Bar,
  has light;

Object PubYard "The Pub Yard"
  with description
         "A cobbled yard behind the pub with bins, a stack of empty crates and a gate that opens east onto the park. Beyond the gate you can see the lawn and the bulk tin on it. The back door of the pub is west.",
       w_to Bar, e_to Lawn,
  has light;

Object Lawn "The Lawn"
  with description
         "The middle of the park, just inside the tape, with the bulk tin looming over you like a landed apartment block. A door in its side stands open, and on the grass in front of it two creatures sit at a folding table with a very large chart, looking worried. The pub yard is west.",
       w_to PubYard,
  has light;

Object grebulons "Grebulons" Lawn
  with name 'grebulon' 'grebulons' 'creatures' 'crew' 'aliens' 'two',
       description "Two squat, round-shouldered creatures in matching grey coveralls, each wearing the expression of a being who has misplaced something important and cannot remember what.",
       initial "Two Grebulons are bent over a chart on a folding table, arguing in the voices of people who have been doing it for a century. ",
       life [;
         Give:
           if (noun == sandwich) {
               Award(A_GREB1, 5);
               remove sandwich;
               "The nearer Grebulon takes it, eyes it, passes it to the other, who takes a bite and looks up with great emotion. ~This,~ says the first, ~is hospitality. We had forgotten. We had a button, you know, and no idea what it was for.~";
           }
         Ask, Answer, Tell:
           if (TopicAny('button', 'press', 'red', 'purpose', 'for')) {
               Award(A_GREB2, 5);
               "~The button,~ says the nearer Grebulon, vaguely. ~It is red. It is very large. We were told to press it when the room was full. We don't remember what room. Or what happens.~ A silence. ~Everything, probably.~";
           }
           if (TopicAny('chart', 'map', 'star', 'stars'))
               "The chart is spread over the table, a huge, creased star-map covered in annotations. One star, drawn in red, has been circled, scribbled out and re-circled; beside it someone has written, in a loping hand: BEEBLEBROX'S FOLLY. DO NOT PUBLISH.";
           if (TopicAny('food', 'sandwich', 'eat', 'hungry', 'hospitality'))
               "~We haven't eaten,~ says the nearer Grebulon, ~since before the war.~ ~Which war?~ ~We've forgotten that too.~";
           if (TopicAny('earth', 'london', 'room', 'pub', 'variants', 'mk'))
               "~All the Earths,~ says the nearer Grebulon, vaguely. ~In one room. The map says so. Is it you? You look like you might be the room.~";
           "The Grebulons look at one another, and then at their chart, as if it might help. It doesn't.";
       ],
  has animate;

! ========================================================================
!  Routines for Act V
! ========================================================================


[ StartAct5;
    act = 5;
    pt = 0;
    move man to Bar;
    move card to Snug;
    if (IndirectlyContains(player, fish) == 0) {
        move jar to player;
        move fish to jar;
        print "^Ford falls into step beside you and, without comment, presses a jam jar into your hands. ~Found this in the gutter,~ he says. ~I think it's yours.~ Inside, something small and yellow turns over and goes back to sleep.^";
    }
    tricia_follows = 1;
    random_follows = 1;
    ford_follows = 1;
];

[ SeatSub;
    if (location ~= Bar) "There's nowhere particular to sit.";
    if (seated) "You are all seated already.";
    if (Tricia notin Bar || Daughter notin Bar || Ford notin Bar)
        "Not everyone is here yet.";
    seated = 1;
    Award(A_SEAT, 5);
    "You steer the others to the corner table. Tricia takes the chair that faces the door, from habit. Ford takes the one that faces the exit, from a different habit. Random pulls out the one nearest the wall and sits with her bag in her lap, looking like someone who has agreed to this under protest. For the first time since you landed, nobody is running.";
];

[ WarnSub;
    if (location ~= Bar) "There is nobody here who needs warning.";
    if (gun_fired) "It is rather late for that.";
    if (bar_t < 2) "Nobody sees any reason to duck. Perhaps when there is one.";
    if (card_read == 0) "You have a feeling something is about to happen, but you cannot say what, and ~get down~ sounds like panic, which you were specifically told not to do.";
    warned = 1;
    "~Everybody,~ you say, and your voice comes out wrong, ~get down. Now. Under the table. Please.~ To your surprise, Tricia, Ford and Random all do. The man in the raincoat watches you, and then the table, and then, with a small nod, you.";
];

[ DuckSub;
    if (location == Bar && gun_fired == 0 && bar_t >= 2 && card_read) {
        warned = 1;
        "You drop to the floor, and the others, after a moment, follow. Not one of them asks why.";
    }
    "You crouch for a moment, to no particular purpose, and straighten up again.";
];

[ BarStep;
    if (location ~= Bar) return;
    bar_t++;
    if (bar_t == 1 && fish_checked == 0) {
        fish_checked = 1;
        if (IndirectlyContains(player, fish)) {
            Award(A_FISH5, 10);
            print "^~You brought the fish,~ says Ford quietly, with approval. ~Good man.~^";
        }
    }
    if (bar_t == 1)
        print "^Tricia gets a round in. Random sits with her bag in her lap, looking at everything and trusting nothing. Something in her coat pocket makes a faint, contented purring.^";
    if (bar_t == 2) {
        move laser to Daughter;
        print "^Random takes something short and ugly out of her bag and puts it on the table, where it looks out of place. It is a laser pistol. She has found it somewhere on the way and has not yet read the instructions.^";
    }
    if (bar_t == 4 && gun_fired == 0)
        FireGun(0);
];

[ FireGun why;
    gun_fired = 1;
    after_t = 0;
    doom_t = 6;
    if (difficulty == DIFF_FAITHFUL) doom_t = 5;
    if (card_read && warned) {
        doom_t = 9;
        if (difficulty == DIFF_FAITHFUL) doom_t = 7;
        Award(A_PREP, 10);
    }
    move matchbook to Bar;
    move laser to Daughter;
    remove man;
    if (why == 1)
        print "^Your fingers have barely closed on it before Random yanks it back. There is a click, a whine, and a line of white light that makes the air smell of toast.^";
    else
        print "^Random picks up the pistol to hand it to you, and her thumb finds the one control she hasn't read about. There is a click, a whine, and a line of white light that makes the air smell of toast.^";
    print "^The beam wanders. It touches the beams, a bottle, the corner table. A man in a damp raincoat is no longer sitting at it. ";
    if (card_read && warned)
        print "Because you warned them, everybody is already under the table, and nobody else is hurt; the pub erupts in shouting and a barman diving for the cellar, but it is the sort of eruption one can watch from underneath.^";
    else
        print "The pub erupts: there is shouting, a stampede, a barman diving for the cellar, someone on a table, and for a few frightening seconds no one at all in a position to say who is hurt.^";
];

[ RadioReady;
    if (IndirectlyContains(player, fish) == 0) rfalse;
    if (matchbook_read == 0 || gun_fired == 0) rfalse;
    if (trust < 4) rfalse;
    if (CubeAllowsRadio() == 0) rfalse;
    if (location ~= Bar || Daughter notin Bar || Tricia notin Bar || Ford notin Bar) rfalse;
    rtrue;
];

[ AfterStep;
    after_t++;
    if (after_t == doom_t - 3 && matchbook_read == 0)
        print "^~Arthur,~ says Ford, pulling you aside. ~Something on the floor by that table. Look at it. Quickly.~^";
    if (RadioReady() && after_t == doom_t - 2)
        print "^Random's fingers have found your sleeve. Ford, wiping his eyes, nudges you. ~Arthur. Pocket. Fish. If ever there was a moment.~^";
    if (after_t >= doom_t)
        Doom();
];

[ Doom;
    if (matchbook_read) {
        ending = 2;
    } else
        ending = 1;
    print "^Across the road, in a ship the size of a bulk tin, two Grebulons who have forgotten why they came finish their long afternoon of calculation. The chart is clear on one point: the thing in the way of everything good is this particular planet. One of them presses the sort of button one presses when one has forgotten what it does.^^In the pocket of Random's coat the Mk II finishes the job it was built for, which was never to guide anyone anywhere but to make quite sure that every road led to a single room on a single afternoon. It purrs once, in satisfaction, folds itself in half, and is not there any more.^^";
    if (matchbook_read)
        print "You have time, at least, to think that the books are balanced. You have a daughter, and a friend, and a very great deal of sky is coming down, and for once none of it is your fault.^^";
    else
        print "You have time for one question. It is a good one, and there will be nobody left to answer it.^^";
    print "The sky goes out. Not just here: in every London, over every field, in every version of every Earth there has ever been, the same thing happens, quietly, at the same moment. It is the original demolition order, finally served in full. The Vogons will be pleased. They will not say so.^";
    deadflag = 3;
];

[ FishEarSub;
    if (noun ~= fish)
        "That is not going in anyone's ear.";
    if (IndirectlyContains(player, fish) == 0)
        "You cannot reach it.";
    if (location ~= Bar || gun_fired == 0 || matchbook_read == 0)
        "You hold the fish up to your ear. It wriggles with great reluctance and conveys, in some way you find hard to describe, that it is saving itself for something.";
    if (Daughter notin Bar || Tricia notin Bar || Ford notin Bar)
        "The fish hesitates at the rim of your ear, as though a quorum were needed and the room does not have one yet. It wants, in some wordless way, the people who matter.";
    if (trust < 4)
        "The fish pauses. It glances across the table at Random, who is looking at you as one looks at a stranger on a train, and decides, with the delicacy of something very small and very wise, that the trust that would carry the whole room is not quite there. Perhaps if you had listened to her a little more.";
    if (CubeAllowsRadio() == 0)
        "The fish starts toward your ear and stops. It turns, very slowly, to look at the cube, which has been quiet for some time. Whatever it sees, it decides with the gravity of something small and wise that it will not be getting involved.";
    RadioEnding();
    rtrue;
];

[ RadioEnding;
    ending = 3;
    Award(A_CHOICE, 10);
    print "The fish slides into your ear with the faint reluctance of something that has already guessed what you are about to ask of it, and for a moment the whole pub goes quiet in the way a church does. Then you feel it: a vast number of small, bored, golden creatures, in a vast number of ears, all quietly deciding that this is not the afternoon on which the Galaxy gets to wind itself up. Somewhere an improbability that has been building for decades curdles, politely, into something kinder.^^";
    print "There is a pop.^^";
    print "You are sitting on a velvet chair at a table in an enormous domed restaurant. Beyond a vast window the universe is ending, magnificently, the way it does every night; nobody is looking.^";
    StartAct6();
];

[ Act5Tick;
    switch (gun_fired) {
      0: if (act == 5 && seated >= 0) BarStep();
      1: AfterStep();
    }
];
