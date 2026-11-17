!  act6.h -- Act VI: The Restaurant at the End of the Universe.
!  Four rooms, four puzzles, 40 points.  Reached only by the radio ending.
!
!              Terrace
!                 |
!   Foyer ---- Dining Hall ---- Kitchen        (Foyer is south of the Dining Hall)

Object pencil "pencil"
  with name 'pencil' 'order' 'pad' 'stub',
       description "A short, much-chewed pencil that a waiter left on the table, ordinarily for taking orders, extraordinarily for taking any chance at all.";

Object DiningHall "The Dining Hall"
  with description [;
         print "An enormous domed hall of white linen and polished brass, under a ceiling that has been painted with the night sky and then, at some point, switched on. Tables run off into a golden distance. On a small stage at the far end, a band is playing something faintly apologetic. Your own table, round and large and beautifully laid, stands in the middle. The foyer is south, the kitchen east through a swing door, and glass doors to the north open onto a terrace.";
         new_line;
         rtrue;
       ],
       s_to Foyer, e_to Kitchen, n_to Terrace,
  has light;

Object tables "tables" DiningHall
  with name 'table' 'tables' 'linen' 'brass' 'chairs' 'cutlery',
       description "A great many tables laid for a great many people who will, in due course, be there for the end of everything. Yours has a small card on it: RESERVED, ENTIRELY.",
  has scenery pluralname;

Object band "band" DiningHall
  with name 'band' 'stage' 'music' 'musicians',
       description "Four very good musicians playing a very sad waltz. They are, you realise, playing the exact tune of the evening. The singer has the look of someone who has been told the ending and is being kind about it.",
  has scenery;

Object Marvin "Marvin" DiningHall
  with name 'marvin' 'robot' 'android' 'depressed',
       description "A small, battered, deeply dejected robot with a tray he evidently picked up out of habit. His head is tilted slightly downward at an angle that has taken about a million years to acquire.",
       initial "Marvin stands beside your chair with a tray, in the way of someone who has been waiting a very long time for no one to ask. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('diode', 'diodes', 'pain', 'left', 'side', 'health')) {
               Award(A_MARVIN, 6);
               marvin_follows = 1;
               "~The diodes,~ says Marvin, as though you had finally said the magic word. ~Down my left side. Since you ask, which nobody has. They have been aching since before the planet you're thinking of had a name.~ A pause, in which he seems to enjoy it. ~Thank you. I feel no better. But it is something to have said.~ He lifts his tray a quarter-inch, which is how he indicates that he will come along, naturally, and that no one will notice.";
           }
           if (TopicAny('brain', 'mind', 'clever', 'intelligence', 'planet'))
               "~I have a brain,~ says Marvin, ~the size of a planet, and I have used about a hundredth of it to carry this tray. The rest is waiting for something to think about. It is waiting with great patience.~";
           if (TopicAny('life', 'meaning', 'point', 'universe', 'end'))
               "~The universe ends tonight,~ says Marvin, ~in about forty minutes. It does this every night. I have seen it eleven thousand times. It is, I am sorry to say, always magnificent. That is the worst of it.~";
           if (TopicAny('fenchurch', 'terrace', 'woman', 'corner'))
               "~She is on the terrace,~ says Marvin. ~She has been there some time. She told me she was waiting for someone and I said, naturally. Then she smiled. I did not know what to do with that.~";
           if (TopicAny('random', 'girl'))
               "~The young one,~ says Marvin. ~She asked me whether I would be content. I said no. She said, good. We got on rather well.~";
           if (TopicAny('arthur', 'you', 'dent'))
               "~You,~ says Marvin. ~Arthur. I have been waiting at the table where you were due. I brought a tray. I would very much like it if someone took something off it.~";
           if (TopicAny('ford', 'prefect'))
               "~Mr Prefect,~ says Marvin. ~He tipped me once. It was a coin I did not recognise. I have kept it ever since.~";
           if (TopicAny('dinner', 'food', 'order', 'menu', 'waiter'))
               "~Dinner,~ says Marvin. ~There is a dish of the day. It would like to be eaten. I am afraid it is quite sincere. I have given up trying to talk anyone out of anything.~";
           "~I would help,~ says Marvin, ~but there is the matter of my own personality.~";
       ],
  has animate male proper;

Object Waiter "waiter" DiningHall
  with name 'waiter' 'maitre' 'server' 'man',
       description "A waiter in a white jacket, with the poise of a man who has served a great many people their last supper, and been tactful in every language.",
       initial "A waiter hovers at your elbow with a pad, perfectly patient. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('dish', 'special', 'day', 'meat', 'beef', 'steak', 'roast'))
               "~Today's dish,~ says the waiter, ~is the shoulder of a very fine animal who has put himself forward for the purpose. He will tell you so himself. Most guests find it an affecting conversation.~";
           if (TopicAny('order', 'menu', 'dinner', 'food', 'eat', 'hungry'))
               "~Whatever you like,~ says the waiter, ~except perhaps the vegetarian option, which people feel awkward about, and the dish of the day, which people feel awkward about for other reasons.~";
           "~If you need anything,~ says the waiter, ~I'll be nearby for the remainder of the Universe.~";
       ],
  has animate male scenery;

Object dish "Dish of the Day" DiningHall
  with name 'dish' 'animal' 'beast' 'special' 'creature' 'cow',
       description "A large, amiable four-legged creature in a bow tie, standing beside the waiter with the dignity of someone who has found his purpose and is happy about it.",
       initial "A large, amiable animal in a bow tie waits patiently beside the waiter. ",
       life [;
         Ask, Answer, Tell:
           "~Do have me,~ says the Dish of the Day, warmly. ~I've been looking forward to it since the beginning of time. I'm very good with mustard.~";
       ],
  has animate male scenery;

Object Foyer "The Foyer"
  with description
         "A curved entrance hall of marble and discreet gold, with a long reception desk, a cloakroom and a queue that is entirely seated. A host at the desk looks up as you enter with the expression of a man who is delighted you could come. The dining hall is north.",
       n_to DiningHall,
  has light;

Object Host "host" Foyer
  with name 'host' 'greeter' 'desk' 'reception' 'man',
       description "A tall man in a tailcoat, with the air of someone who has never been surprised and has been looking forward to it for centuries.",
       initial "The host stands behind the desk, ready to be delighted. ",
       life [;
         Ask, Answer, Tell:
           "~Your table is waiting,~ says the host warmly. ~Where it always was. Where it always will be. Don't hurry, but do not be late.~";
       ],
  has animate male scenery;

Object Kitchen "The Kitchen"
  with description
         "A white, roaring kitchen full of copper and steam, where a great many cooks hand things to one another without looking, like a dance rehearsed for ten thousand years. A chef in the middle of it all stands very still, in the way of a man who has found the eye of the storm and sits in it. The dining hall is west through the swing door.",
       w_to DiningHall,
  has light;

Object Chef "chef" Kitchen
  with name 'chef' 'cook' 'cooks' 'man',
       description "A big, quiet man in a tall hat, who looks as though he has the whole kitchen in his head and the rest in his hands.",
       initial "The chef stands in the middle of the kitchen, absolutely still. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('sandwich', 'sandwiches', 'bread', 'lunch'))
               "The chef's eyes brighten. ~Bread,~ he says. ~Something honest in the middle of all this. I would give a great deal for a quiet hour and a good sandwich. Someone told me there's a man who makes them.~";
           "~I'm cooking the end of the world,~ says the chef, with enormous modesty. ~It's mostly a matter of timing.~";
       ],
  has animate male scenery;

Object Terrace "The Observation Terrace"
  with description [;
         print "A long glass terrace running along the whole curve of the building, open to a sky that is not a sky: the Universe, in its last hours, spilling slowly from the end of the dining hall like a great slow fire. The corner table is set for six. The dining hall is south.";
         new_line;
         rtrue;
       ],
       s_to DiningHall,
  has light;

Object glass_wall "glass wall" Terrace
  with name 'glass' 'wall' 'window' 'sky' 'universe' 'view',
       description "Beyond the glass the stars are slowing down, one at a time, with the dignity of old clocks. Nobody is looking, which, you think, is a shame, and also the point.",
  has scenery;

Object Fenchurch "Fenchurch" Terrace
  with name 'fenchurch' 'woman' 'girl',
       description [;
         tilt_seen = 1;
         "She sits at the corner table with her chin on her hand, looking at the end of the Universe with the look of someone who has come a long way to be looking at it with someone. Her head tilts, just so: to the left, and slightly back, and with a small pause at the top as if the world were a question she meant to answer later. You have drawn that tilt forty times. It is nothing like the drawing.";
       ],
       initial "Fenchurch sits at the corner table, alone, looking out at the end of the Universe. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('sketch', 'drawing', 'tilt', 'head', 'picture')) {
               tilt_seen = 1;
               "~You drew me,~ says Fenchurch, with her head on one side. ~Forty times, Random says. Each time you got the tilt wrong. I'd like to see.~ She does not say it unkindly. She watches your hands.";
           }
           if (TopicAny('jump', 'vanished', 'gone', 'lost', 'missing', 'left'))
               "~I fell out of the jump,~ says Fenchurch, ~and I didn't know how to find you. So I waited somewhere every road eventually passes.~ She looks at the stars. ~It's very nice, isn't it. It doesn't last, but that's the whole of it.~";
           if (TopicAny('arthur', 'you', 'dent'))
               "~You look tired,~ says Fenchurch, and moves her chair a foot to make room, which is the kindest thing anyone has said to you in years.";
           if (TopicAny('random', 'girl', 'daughter'))
               "~She's marvellous,~ says Fenchurch. ~She has your chin. She has someone else's rage. She hasn't decided what to do with either.~";
           if (TopicAny('ford', 'prefect', 'tricia', 'marvin'))
               "~Everyone's here,~ says Fenchurch, looking round the table with the smile of a woman who has been waiting to say so. ~That's the right way to end.~";
           "Fenchurch smiles, tilts her head the way she does, and says nothing, which is also an answer.";
       ],
  has animate female proper;

Object terrace_table "corner table" Terrace
  with name 'table' 'corner' 'chairs',
       description "A round table in the corner with six chairs, a white cloth, and a small candle that has been burning for exactly as long as the evening.",
  has scenery supporter;

! ========================================================================
!  Routines for Act VI
! ========================================================================

[ DineSub w i veg dishy teaq;
    if (location ~= DiningHall) "Nobody here is taking orders.";
    if (awarded->A_ORDER) "You have already ordered, and very well.";
    wn = consult_from;
    veg = 0; dishy = 0;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 'salad' or 'vegetables' or 'vegetable' or 'veg' or 'vegetarian' or 'soup' or 'sandwich' or 'bread'
                or 'pasta' or 'risotto' or 'omelette' or 'cheese' or 'anything' or 'something') veg = 1;
        if (w == 'dish' or 'meat' or 'steak' or 'beef' or 'roast' or 'shoulder' or 'special' or 'day') dishy = 1;
        if (w == 'tea') teaq = 1;
    }
    if (teaq)
        "~Tea,~ says the waiter, and looks at you with sympathy. ~We do have a drink that is a long way from tea, and a very good friend of it. Most guests find that it settles the nerves.~ He brings it. It is warm, brown, and in no way tea. It is, you find, quite pleasant.";
    if (dishy)
        "You open your mouth to order the dish of the day, and the animal in the bow tie turns to you with such shining, uncomplicated eagerness that the word will not come. ~Do have me,~ it says. ~I'd be honoured.~ You find you simply cannot, and you say so, as gently as you can. ~Never mind,~ says the Dish of the Day, ~someone will.~ The waiter raises an eyebrow, as if to say that he sees this a good deal.";
    if (veg) {
        Award(A_ORDER, 8);
        "~An excellent choice,~ says the waiter, in a tone that suggests it is the first he has heard in some centuries. He writes it down with enormous ceremony. The Dish of the Day nods approvingly and wanders off to wait for someone less principled. ~You are,~ says Ford, with feeling, ~a better man than I am.~";
    }
    "~And to follow?~ says the waiter, patiently, pencil poised. You could try ordering something that did not volunteer.";
];

[ DrawSub;
    if (location ~= Terrace) "There is no one here to draw, and you have nothing to draw on.";
    if (awarded->A_TILT) "The new drawing is finished, and better than the old one, and you do not touch it again.";
    if (sketch notin player) "You haven't got the sketch.";
    if (pencil notin player && pencil notin location) "You'll need something to draw with.";
    if (tilt_seen == 0) "You haven't looked at her properly. Draw what is there, not what you remember.";
    Award(A_TILT, 12);
    "You smooth the sketch on the corner table and look at her, properly, as you have not let yourself look in a very long time. Then you draw: not what you remember, not what you wished, but the angle of her head, the small pause at the top, and the smile that follows it by exactly the time it takes to decide to. It takes a minute. When you look up, Fenchurch is watching your hands. You turn the sketch round. She looks at it for a long time. ~There,~ she says, quietly. ~That's it. That's me.~";
];

[ ToastSub n;
    if (location ~= Terrace) "There is no occasion here for a toast; wait until the whole table is together.";
    n = 0;
    if (Ford in location) { Award(A_TOAST1, 2); n++; }
    if (Daughter in location) { Award(A_TOAST2, 3); n++; }
    if (Tricia in location) { Award(A_TOAST3, 3); n++; }
    if (Marvin in location) { Award(A_TOAST4, 3); n++; }
    Award(A_TOAST5, 3);
    print "You stand, which is not usual, and pick up a glass, which is. The table looks up: Fenchurch, with her head on one side, Random, with her chin; Ford, in the middle of an anecdote he abandons; ";
    if (Tricia in location) print "Tricia, with a journalist's raised eyebrow; ";
    if (Marvin in location) print "Marvin, still holding his tray, waiting for the point; ";
    print "and you find you have nothing prepared.^^";
    print "~To the end of the Universe,~ you say, ~which I am told does this every night, and to everyone who turned up for it.~ You look at Random, who looks at her plate. ~To the people who were late. And to the ones who were never going to be anywhere else.~^^";
    print "The glasses touch. Beyond the glass, the Universe finishes, magnificently, the way it does every night; and at your table nobody is looking at it, because everyone is looking at each other.^^";
    print "The cube is nowhere. The Vogons will have to find some other way. For now the Earth is a little further away and entirely, for the moment, intact; Random has her father's chin and she is, with an effort, willing to be seen with it. The universe ends, over the soup, and you find that you do not mind. It is, on the whole, mostly harmless.^";
    ending = 3;
    deadflag = 3;
    rtrue;
];

[ Act6Tick;
    if (marvin_follows && Marvin notin location) {
        move Marvin to location;
        print "^Marvin shuffles in after you with his tray.^";
    }
];

[ StartAct6;
    act = 6;
    pt = 0;
    ford_follows = 1;
    random_follows = 1;
    tricia_follows = 1;
    remove fish;
    remove guide;
    if (sketch notin player) {
        move sketch to player;
        sketch_given = 1;
    }
    move Ford to DiningHall;
    move Daughter to DiningHall;
    move Tricia to DiningHall;
    move pencil to terrace_table;
    FloydKitchen();
    PlayerTo(DiningHall);
    if (sketch_given)
        print "^Random, who has not said a word since the pop, reaches into her coat and hands you something folded very small. ~I took it from your hut,~ she says, not looking at you. ~On the first day. I wanted to know what you looked at.~ It is the pencil sketch. She has smoothed it flat.^";
];
