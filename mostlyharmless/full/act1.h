!  act1.h -- Act I: Lamuella.  Eight rooms, ten puzzles, 60 points.

! ========================================================================
!  Rooms
! ========================================================================

Object Hut "Arthur's Hut"
  with description
         "A single round room of mud, thatch and optimism. Everything in it is exactly where you left it, because there is nowhere else for it to be. A shelf runs along one wall, and a bed, which is a bed on a generous day, takes up the other. The doorway opens east onto the village green.",
       e_to Green,
       out_to Green,
  has light;

Object bed "bed" Hut
  with name 'bed' 'blankets' 'pillow',
       description "It is a bed. It has done its best for you every night for years, and you have given it very little to work with.",
       before [;
         Enter, Climb: "It is far too early to go back to bed. There is a sandwich shop to open.";
       ],
  has static supporter enterable;

Object shelf "wooden shelf" Hut
  with name 'wooden' 'shelf',
       description "A plank on two pegs, adequate for the three things you own that are not clothes.",
  has static supporter;

Object sketch "pencil sketch" shelf
  with name 'pencil' 'sketch' 'drawing' 'picture' 'portrait',
       description [;
         Award(A_SKETCH, 2);
         "It is a drawing, in your own clumsy pencil, of a girl with a way of tilting her head as if the world were a question she meant to answer later. Fenchurch. You have drawn her at least forty times and never once got the tilt right. She vanished in the middle of a hyperspace jump, which is a thing that happens, and which nobody has ever satisfactorily explained to you.";
       ];

Object jar "glass jar" shelf
  with name 'jar' 'jam',
       description "A jam jar with a punctured lid. Something small and yellow turns lazily inside it.",
       after [;
         Take: Award(A_FISH, 2);
       ],
  has container open transparent;

Object fish "Babel fish" jar
  with name 'babel' 'fish' 'yellow' 'creature',
       description "A small yellow fish with the expression of something that has seen a great many conversations and been tactful about all of them.",
       after [;
         Take: Award(A_FISH, 2);
       ];

Object dressing_gown "dressing gown"
  with name 'dressing' 'gown' 'robe' 'sleeves' 'pocket',
       description "Your dressing gown: thick, brown, and older than the village. The sleeves are heavy enough to stop a small animal, or a large bramble.",
  has clothing;

Object basket "wicker basket" Hut
  with name 'wicker' 'basket',
       initial "A wicker basket sits by the bed, doing nothing in particular. ",
       description "A sturdy basket woven from river-reed.",
  has container open;

Object licence "sandwich licence" Hut
  with name 'licence' 'license' 'certificate' 'wall',
       description "Framed on the wall, in a hand that has seen a lot of forms: SANDWICH MAKER, LAMUELLA. Underneath, smaller: ~to supply lunch as required, without complaint.~ You have never found that last part difficult.",
  has scenery;

! ------------------------------------------------------------------------

Object Green "The Village Green"
  with description [;
         print "A trampled circle of grass ringed by a dozen houses like your own. Smoke rises, unhurried, from one or two chimneys. Your house is to the west, the sandwich hut stands to the east, and a path runs north to the meadow where the occasional ship puts down. A track drops away south towards the orchard slope, and two small paths wander off to the southwest (the Reeve's lodge) and the southeast (the smokehouse).";
         if (parcel in self) print " Someone has left a brown parcel in the middle of the grass, as if it were an offering.";
         new_line;
         rtrue;
       ],
       w_to Hut, e_to SandHut, n_to Meadow, s_to Orchard,
       sw_to Lodge, se_to Smokehouse,
  has light;

Object well "well" Green
  with name 'well' 'bucket' 'rope',
       description "A stone ring with a bucket on a rope. Nothing about it has ever gone wrong, which makes it unusual.",
  has scenery;

Object SandHut "The Sandwich Hut"
  with description
         "A low hut with a wide shutter that folds down into a counter. This is where your life happens. Shelves and a cool pit hold the ingredients, a chopping board sits in the middle of the counter, and the knife hangs from its nail. The village green is back to the west.",
       w_to Green, out_to Green,
  has light;

Object counter "counter" SandHut
  with name 'counter' 'shutter',
       description "A folding shutter, propped open on its hinges. Years of crumbs have been rubbed into it until they have become a finish.",
  has static supporter scenery;

Object board "chopping board" SandHut
  with name 'chopping' 'board',
       description "A thick slab of wood, deeply scored. It is where bread becomes lunch.",
  has static supporter scenery;

Object loaf "loaf of bread" counter
  with name 'loaf' 'bread' 'lamuellan',
       description "A round loaf, brown, with a crust that has opinions. There is always one.";

Object knife "sandwich knife" counter
  with name 'knife' 'blade' 'carving',
       description "A long thin blade with a worn wooden handle. It has cut more bread than anyone has counted.",
       before [;
         Drop: if (location == SandHut) "(You hang it on its nail instead.)";
       ];

Object cool_pit "cool pit" SandHut
  with name 'cool' 'pit' 'cellar',
       description "A square hole in the floor with a lid and a rim of cold air. It keeps things.",
  has static container open scenery;

Object pantry "pantry shelf" SandHut
  with name 'pantry' 'shelf' 'shelves',
       description "A plank shelf of jars and crocks, with a smell of dried herbs.",
  has static supporter scenery;

Ingredient i_pickles "pickled roots" cool_pit
  with name 'pickled' 'pickles' 'pickle' 'root' 'roots',
       description "Pale roots in brine, sharp enough to wake a stone.",
       ibit BIT_PICKLE, home cool_pit,
       missing "There are no pickled roots to hand; they live in the cool pit."
  has pluralname;

Ingredient i_honey "honey" pantry
  with name 'honey' 'honeypot' 'pot',
       description "Dark amber honey in a clay pot, from the bees on the orchard slope.",
       ibit BIT_HONEY, home pantry,
       missing "There is no honey to hand; it lives on the pantry shelf.";

Ingredient i_curd "herb curd" pantry
  with name 'herb' 'curd' 'herbs' 'crock',
       description "A soft green curd, flecked with herbs, kept in a crock. It is soft enough to eat with no teeth at all.",
       ibit BIT_CURD, home pantry,
       missing "There is no herb curd to hand; it lives on the pantry shelf.";

Ingredient i_meat "cured meat"
  with name 'meat' 'cured' 'ham' 'jerky' 'haunch',
       description "A slab of dark, smoky meat, cured by someone who knew what she was doing.",
       ibit BIT_MEAT, home cool_pit,
       missing "There is no cured meat in the cool pit at the moment. Dunna smokes the village's, and she is not handing it over yet.";

Ingredient i_berries "sourberries"
  with name 'berry' 'berries' 'sourberry' 'sourberries' 'sour' 'purple',
       description "Small, tight, purple berries that make your eyes water and your tongue sit up.",
       ibit BIT_BERRY, home Orchard,
       initial "Sourberries cling to the bushes in thorny profusion. ",
       before [;
         Take:
           if (self in Orchard) {
               if (dressing_gown in player && dressing_gown hasnt worn && difficulty == DIFF_EASY) {
                   give dressing_gown worn;
                   print "(first putting on the dressing gown)^";
               }
               if (dressing_gown hasnt worn || dressing_gown notin player)
                   "The bushes have thorns like an accountant's smile; you would leave half your hands behind. Something thick over your arms would help.";
               if (basket notin player)
                   "You pick a handful and immediately have nowhere to put them. A basket would help.";
               move self to basket;
               Award(A_BERRIES, 3);
               "You pick the berries with your sleeves held out of the thorns' way, and tip them into the basket. The bees allow it, after a short debate.";
           }
       ]
  has pluralname;

Object sandwich "sandwich"
  with name 'sandwich' 'sarnie' 'lunch',
       description [;
         print "You made it yourself. It is ";
         SandwichDesc(1);
         ".";
       ];

! ------------------------------------------------------------------------

Object Orchard "The Orchard Slope"
  with description
         "A gentle slope of sourberry bushes, planted by someone with ambition and tended by someone with a scarf. The bushes are low and thorny and heavy with small purple berries; bees move among them like a committee. The village is back to the north.",
       n_to Green, out_to Green,
  has light;

Object bushes "sourberry bushes" Orchard
  with name 'bush' 'bushes' 'thorns' 'thorn' 'shrubs' 'plants',
       description "Dense, low and bristling. The berries are all on the far side of the thorns, which is how they like it.",
  has scenery;

Object bees "bees" Orchard
  with name 'bee' 'bees' 'committee' 'hive',
       description "Fat, unhurried bees who have somewhere to be and do not intend to be seen to hurry.",
       before [;
         Attack: "That would be an argument with the entire hive, and you are not at your best in arguments.";
         Take: "You would sooner take the berries.";
       ],
  has scenery pluralname;

Object Smokehouse "The Smokehouse"
  with description
         "A squat hut of black timber, whose single room is a perpetual early evening. Racks of curing meat hang in the smoke. A chopping block stands by the door with a cleaver on it. The smoke finds your eyes in about a second and a half. A path leads back northwest to the village green.",
       nw_to Green, out_to Green,
  has light;

Object racks "racks of meat" Smokehouse
  with name 'racks' 'rack' 'hanging' 'meat' 'smoke',
       description "Dark sides of meat, curing in a smoke so thick you could set a spoon in it.",
       before [;
         Take: "Dunna would notice. Dunna notices everything that happens in this room, and most of what happens outside it.";
       ],
  has scenery;

Object cleaver "cleaver" Smokehouse
  with name 'cleaver' 'blade' 'axe',
       sharp 0,
       description [;
         if (self.sharp) "The cleaver's edge now looks like a promise.";
         "A heavy cleaver with a blade as blunt as a rumour. It has not cut anything cleanly in years.";
       ],
       before [;
         Take: "It is Dunna's, and she is standing right there.";
       ],
  has static scenery;

Object Lodge "The Reeve's Lodge"
  with description
         "The biggest building in the village by some margin, which is not saying much: a round hall with a floor of beaten earth, a ring of low seats, and in the middle a great smooth boulder that everyone calls the Hearing Stone, because that is where the village comes to hear itself talk. The village green is back to the northeast.",
       ne_to Green, out_to Green,
  has light;

Object hearing_stone "Hearing Stone" Lodge
  with name 'hearing' 'boulder' 'seats' 'ring',
       description "A great round stone, worn so smooth by elbows that it shines. The village insists it listens. It has never said anything back, which is the quality they admire in it.",
  has scenery;

Object Meadow "The Landing Meadow"
  with description [;
         print "A wide, flat meadow of soft grass where visitors, on the rare occasions that Lamuella has any, are invited to park. The sky above it is very large and has the look of somewhere things can arrive from. The village green is back to the south, and a trodden path leads east towards the sound of water.";
         new_line;
         rtrue;
       ],
       s_to Green, e_to River,
  has light;

Object River "The River Bank"
  with description [;
         print "A slow brown river that has never been in a hurry. Flat stones make a path of sorts across the shallows, and reeds stand along the bank. Near your feet a big flat rock sits on the mud as if it had been put there to be looked under. The meadow is back to the west.";
         new_line;
         rtrue;
       ],
       w_to Meadow,
  has light;

Object reeds "reeds" River
  with name 'reed' 'reeds',
       description "Tall, green and whispering. They lean in to listen to everything you say, and then lean away.",
  has scenery pluralname;

Object rock "flat rock" River
  with name 'rock' 'flat' 'boulder',
       lifted 0,
       description [;
         if (self.lifted) "The rock lies where you turned it, with a damp pale patch beneath.";
         "A big flat rock, bedded in the mud. It looks as though something might be underneath, and as though someone has been careful not to say so.";
       ],
       before [;
         Take, Push, Pull, Search, LookUnder, Turn:
           if (self.lifted) "You have already turned it over.";
           self.lifted = 1;
           move whetstone to River;
           "You heave the rock aside. Underneath, in the damp, lies a smooth grey whetstone, the sort a sensible person would have kept somewhere drier.";
       ],
  has static scenery;

Object whetstone "whetstone"
  with name 'whetstone' 'stone' 'sharpening' 'whet',
       description "A flat grey stone, finely gritted on one face. Someone used it for years and then lost it, or hid it.",
       after [;
         Take: Award(A_WHET, 2);
       ];

! ========================================================================
!  The Mk II parcel
! ========================================================================

Object parcel "brown parcel"
  with name 'brown' 'parcel' 'package' 'box',
       description "A brown paper parcel tied up with a stiff cord. Your name is on it, in Ford's handwriting, underlined twice.",
       before [;
         Open:
           if (cord in Green) "It is tied with a stiff cord, knotted by someone who meant it. You would need to cut it.";
       ],
       after [;
         Open: Award(A_PARCEL, 3);
       ],
  has container openable;

Object cord "stiff cord"
  with name 'cord' 'string' 'twine' 'knot' 'knots',
       description "A tight, brown cord, knotted several times for good measure.",
       before [;
         Cut: return CutCord();
         Take: "It is part of the parcel.";
       ],
  has scenery;

[ CutCord;
    if (knife notin player && cleaver notin player)
        "You would need something with an edge, and you are not carrying one.";
    remove cord;
    "You slide the knife under the cord and it parts with a sound like a shy cough. The parcel can be opened now.";
];

Object note "handwritten note" parcel
  with name 'handwritten' 'note' 'letter' 'paper',
       description [;
         Award(A_NOTE, 2);
         "In Ford's scrawl: ~Arthur. Mind this for me. Don't switch it on, don't believe a word it says and don't let it near anyone who wants to be helped. Will collect. Soon-ish. F.~";
       ];

! ========================================================================
!  People
! ========================================================================

Object hob "Hob" SandHut
  with name 'hob' 'man' 'regular' 'customer',
       wants (BIT_MEAT + BIT_BERRY),
       sorry "turns the sandwich over once and gives it back. ~That's a sandwich,~ he says. ~It just isn't mine.~",
       eaten [;
         Award(A_HOB, 10);
         hob_fed = 1;
         remove hob;
         move coin to player;
         "Hob takes it in both hands, weighs it like a newborn and bites. For a while he is quite unavailable for conversation. ~That,~ he says at last, ~is why we keep you.~ He pays in a coin of no known denomination, shakes your hand, and strolls off into the morning, chewing. You slip the coin into your dressing-gown pocket; you have never seen its like.";
       ],
       description "A broad, patient man with sawdust in his beard, and the air of someone who has been waiting a very long time for lunch and hasn't minded.",
       initial "Hob leans on the counter with the air of a man who has been waiting since before the sun. ",
       life [;
         Give: if (noun == sandwich) return ServeSandwich(self);
         Ask, Answer, Tell:
           if ((TopicAny('order', 'lunch', 'sandwich', 'usual', 'want', 'wants') || TopicAny('food', 'eat')))
               "~Same as always,~ says Hob. ~Meat and sourberry, thick as you like. And no pickle. I have views about pickle.~";
           if (TopicAny('pickle', 'pickles', 'roots'))
               "~Wrecks a sandwich,~ says Hob. ~Pickle belongs in a jar, where it can think about what it's done.~";
           if (TopicAny('meat', 'dunna', 'smoker'))
               "~Dunna's the one for meat,~ says Hob. ~She's particular about her blade. Whatever you do, don't mention it unless you're prepared to help.~";
           if (TopicAny('berry', 'berries', 'orchard', 'thorns', 'bees'))
               "~Wear something thick,~ says Hob. ~The bushes have teeth.~";
           if (TopicAny('village', 'lamuella', 'work', 'job'))
               "~Quiet place,~ says Hob. ~Nobody's ever asked it to be anything else.~";
           if (TopicAny('fenchurch', 'sketch', 'girl'))
               "Hob studies his boots. ~I'm sorry,~ he says. ~I don't know. But I'd say she wasn't gone for good. People mostly aren't.~";
           if (TopicAny('ford', 'guide', 'parcel', 'cube'))
               "~Never trust anything that comes by post,~ says Hob. ~It's been somewhere.~";
           "~Can't say,~ says Hob. ~Ask me when I've eaten.~";
       ],
  has animate male proper;

Object Dunna "Dunna" Smokehouse
  with name 'dunna' 'smoker' 'woman' 'butcher',
       description "A wiry woman with eyes permanently narrowed against the smoke, and forearms that look as though they have been cured.",
       initial "Dunna stands at the racks, watching the meat as though it might try something. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('meat', 'trade', 'buy', 'sell', 'price', 'sandwich')) {
               if (cleaver.sharp == 0)
                   "~Meat's not for coin,~ says Dunna. ~And I'll not give it for love, either. My cleaver wouldn't cut butter. Sort me an edge on it and we'll talk.~";
               "~We've a deal,~ says Dunna. ~Meat's in your pit. Go and make something with it.~";
           }
           if (TopicAny('cleaver', 'blade', 'edge', 'sharp', 'knife'))
               "~Blunt as a tax,~ says Dunna. ~It wants a good stone. The river has one, if you can find it. Under something heavy, I'd have said.~";
           if (TopicAny('smoke', 'smokehouse', 'racks'))
               "~The smoke's the secret,~ says Dunna. ~That, and not caring how it looks.~";
           if (TopicAny('hob', 'order', 'usual'))
               "~Hob? Meat and sourberries and a long face,~ says Dunna. ~He'll wait. He's good at it.~";
           if (TopicAny('village', 'lamuella', 'reeve', 'tem'))
               "~The Reeve's got a bad tooth,~ says Dunna. ~Ought to eat something soft. Honey, maybe. He won't.~";
           "Dunna's reply is mostly smoke.";
       ],
  has animate female proper;

Object Tem "Tem" Lodge
  with name 'tem' 'reeve' 'elder' 'old' 'man',
       wants (BIT_HONEY + BIT_CURD),
       sorry "puts a hand to his jaw and sets the sandwich down, as if it had hurt him already. ~Kind of you,~ he says, with great dignity. ~Something softer, maybe.~",
       eaten [;
         Award(A_TEM, 5);
         tem_fed = 1;
         "Tem eats in small, careful bites, and his whole face loosens. ~Better,~ he says. ~Much better. The Stone's been saying there's a ship coming. Rare for it to be useful.~ He winks, which on him looks like a minor earthquake.";
       ],
       description "A slow old man with a face like a folded map, and a way of holding his jaw as if it were a secret.",
       initial "Tem, the Reeve, sits on the stone's rim with one hand to his jaw. ",
       life [;
         Give: if (noun == sandwich) return ServeSandwich(self);
         Ask, Answer, Tell:
           if ((TopicAny('tooth', 'jaw', 'hurt', 'pain', 'food', 'eat') || TopicAny('lunch', 'order', 'sandwich', 'want')))
               "~Can't chew,~ says Tem. ~Hard crust, hard meat, hard luck. Something soft would be a kindness: honey, that herb curd. Nothing that fights back.~";
           if (TopicAny('stone', 'hearing', 'listens'))
               "~It hears all of us,~ says Tem. ~It doesn't repeat any of it. Very restful.~";
           if (TopicAny('village', 'lamuella', 'work'))
               "~Quiet place,~ says Tem. ~We like quiet. It lets us hear the Stone.~";
           if (TopicAny('parcel', 'ship', 'ford', 'guide', 'cube', 'meadow'))
               "~Something's coming,~ says Tem. ~The Stone has been in a mood since dawn.~";
           if (TopicAny('fenchurch', 'sketch', 'girl'))
               "Tem is quiet for a long moment. ~The Stone says nothing about her,~ he says. ~Which is not the same as nothing.~";
           if (TopicAny('hob', 'dunna', 'tooth'))
               "~Everyone has views,~ says Tem. ~I try to keep mine soft.~";
           "Tem considers it for some time and then shakes his head.";
       ],
  has animate male proper;

Object pilot "mail pilot"
  with name 'pilot' 'courier' 'man' 'cap',
       wants BIT_PICKLE,
       sorry "lifts his cap a quarter of an inch, looks at it, and sets it down again. ~Not what I asked,~ he says. ~Break is a break.~",
       eaten [;
         Award(A_PILOT, 8);
         pilot_fed = 1;
         "The pilot eats it in four bites, eyes closed, like a man receiving a sacrament. ~I'd fly you to the edge of the Galaxy for that,~ he says. ~Well. Not the edge. But somewhere.~ He taps the skiff's flank. ~Whenever you're ready.~";
       ],
       description "A man in a leather cap, tipped over his eyes, leaning on the skiff with the comfortable air of someone who has been told it is his break.",
       initial [;
         if (location == SkiffCabin) { print "The pilot hunches over the controls, cap over his eyes, humming. ^"; rtrue; }
         print "The pilot dozes against the skiff's flank, cap over his eyes. ^";
         rtrue;
       ],
       life [;
         Give: if (noun == sandwich) return ServeSandwich(self);
         Ask, Answer, Tell:
           if (TopicAny('skiff', 'fly', 'ship', 'earth', 'trip', 'break'))
               "~Break,~ says the pilot, without opening his eyes, ~is a break.~";
           if ((TopicAny('lunch', 'food', 'eat', 'hungry', 'sandwich', 'want') || TopicAny('order', 'pickle', 'pickles')))
               "~Pickled roots,~ says the pilot dreamily. ~On bread. Nothing else on it. A man has a stomach, and he has views.~";
           if (TopicAny('random', 'girl', 'ford'))
               "~Not my business,~ says the pilot. ~I'm on my break.~";
           "~I'm on my break,~ says the pilot.";
       ],
  has animate male;

Object skiff "mail skiff"
  with name 'skiff' 'mail' 'ship' 'hopper' 'craft' 'boat',
       description "A battered little mail skiff with a dent for every planet it has been to. It looks as though it has been told to expect worse.",
       initial "A mail skiff squats in the grass, its engine ticking as it cools. ",
       before [;
         Enter:
           if (pilot_fed == 0)
               "The pilot lifts his hat an inch. ~Break,~ he says, ~is a break.~ Perhaps he might be persuaded with something to eat.";
           ActOneEnd();
           rtrue;
       ],
  has static enterable;

Object Trillian "Trillian"
  with name 'trillian' 'woman',
       description "Poised, tired and impeccable, the way people are who have been to a great many places and been charming at all of them.",
       initial "Trillian stands in the middle of things, composed. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('random', 'girl', 'daughter', 'her', 'child')) {
               Award(A_TRILL1, 1);
               "~Clever, furious and far too like both of us,~ says Trillian. ~She was an accident that took a great deal of paperwork.~";
           }
           if (TopicAny('father', 'arthur', 'you', 'donor', 'how'))
               "~Don't ask about the method,~ says Trillian. ~It involved a hospital, a donor form, and nobody's idea of romance. But it's your side of the family she's got her chin from.~";
           if (TopicAny('ford', 'guide', 'parcel', 'cube')) {
               Award(A_TRILL2, 1);
               "Trillian's mouth tightens. ~I've heard Ford's name near a very large amount of trouble lately. Don't be any use to him.~";
           }
           if (TopicAny('earth', 'tricia', 'london', 'home')) {
               Award(A_TRILL3, 1);
               "~There is an Earth where I never left,~ says Trillian. ~I look in on it sometimes. I'm a good deal less happy there than you'd think.~";
           }
           if (TopicAny('fenchurch', 'sketch'))
               "~I'm sorry about her,~ says Trillian. ~Truly. People do go missing in jumps. They go somewhere.~";
           if (TopicAny('zaphod'))
               "~Not lately,~ says Trillian, with enormous restraint.";
           "Trillian gives you the look of someone who has an appointment with a galaxy.";
       ],
  has animate female proper;

Object Daughter "Random"
  with name 'random' 'girl' 'daughter' 'dent' 'teenager',
       wants (BIT_BERRY + BIT_PICKLE),
       sorry "looks at it, then at you. ~You're supposed to be the grown-up,~ she says. ~Pay attention to what I actually eat.~",
       eaten [;
         Award(A_RANDOMFED, 10);
         random_fed = 1;
         pt = 0;
         TrustUp(1);
         "Random inspects it with deep suspicion, bites, and holds very still. ~Okay,~ she says, with her mouth full, in a tone of grudging scientific honesty. ~That's actually good.~ It is the first thing you've given her that she has not immediately turned into an insult.";
       ],
       description "About fourteen, give or take a parallel universe: thin, watchful, with her hands in her pockets and a chin you recognise, to your slight alarm, from the shaving mirror.",
       initial "Random lounges against the wall with her arms folded, looking at everything the way a cat looks at a bath. ",
       life [;
         Give: return RandomGive();
         Ask, Answer, Tell:
           if (act >= 4 && RandomTalkLate()) rtrue;
           if ((TopicAny('food', 'eat', 'hungry', 'lunch') || TopicAny('sandwich', 'meal', 'dinner', 'order'))) {
               if (random_fed) "~I told you. It was fine.~ She looks away. ~Maybe another.~";
               "~I'm not hungry,~ says Random, in the voice of a girl who is. You notice what she picked from the pickle jar earlier, and what she left. Sharp and sour things; not the meat.";
           }
           if (TopicAny('mother', 'trillian', 'mum', 'mom'))
               "~She's busy,~ says Random. ~She's always busy. It's the second most reliable thing about her.~";
           if (TopicAny('father', 'arthur', 'you', 'dad', 'dent'))
               "~I'm told you make sandwiches,~ says Random. She does not say anything else, and manages to make that sound like a judgement.";
           if (TopicAny('guide', 'cube', 'parcel', 'mk'))
               "Random's eyes go to your pocket and then come away with considerable effort. ~Is it any good?~ she says. ~I mean, is it clever?~";
           if (TopicAny('ship', 'ford', 'earth'))
               "~Everything ends up on Earth,~ says Random. ~Or near it. I want to see one.~";
           if (TopicAny('herself', 'me', 'myself', 'self'))
               "Random shrugs. ~I'm good at maths,~ she says. ~And leaving.~";
           if (TopicAny('fish', 'babel'))
               "~You keep a fish in a jar,~ says Random, with real interest. ~That's the first interesting thing I've learned about you.~";
           "Random shrugs, which is how she says almost everything.";
       ],
  has animate female proper;

Object Ford "Ford Prefect"
  with name 'ford' 'prefect' 'friend',
       description "Lean, wild-eyed, and carrying himself like a man who has just remembered something important; nothing about him suggests it is the thing you hoped. A towel is slung over one shoulder.",
       initial "Ford is here, restless, taking in the place as though pricing it. ",
       life [;
         Ask, Answer, Tell:
           if (act >= 2 && FordTalkLate()) rtrue;
           Award(A_FORDHI, 1);
           if (TopicAny('guide', 'cube', 'mk', 'ii', 'parcel')) {
               Award(A_FORDGUIDE, 1);
               "~It isn't finished,~ says Ford, ~which is the only reason it's still dangerous. The Mk II. It reads you while you read it. I mailed it to you because nobody in the history of the Galaxy has ever looked for anything important in a sandwich hut. No offence.~";
           }
           if (TopicAny('vogon', 'vogons', 'infinidim', 'owners', 'company')) {
               Award(A_FORDVOGON, 1);
               "~They bought the Guide,~ says Ford. ~Through a company with six names and one address. I walked up to the address. Vogons, Arthur. Vogons in suits, which is only Vogons with better excuses. They want Earth gone, all of it, in all its versions, and this new edition is how they plan to be sure.~";
           }
           if (TopicAny('random', 'girl', 'daughter'))
               "~Charming girl,~ says Ford. ~I think she's going to be an enormous problem.~";
           if (TopicAny('towel'))
               "~Never be without one,~ says Ford. ~Yours is probably in lost property somewhere. They always are.~";
           if (TopicAny('earth', 'london'))
               "~The one you remember is gone,~ says Ford. ~There are others. Some of them are very nearly the same.~";
           if (TopicAny('plan', 'next', 'do'))
               "~Rescue the girl,~ says Ford. ~Rescue the Guide. Possibly rescue ourselves, if there's time, which is a hard sentence to say without laughing.~";
           if (TopicAny('fenchurch', 'sketch'))
               "Ford's expression changes, just once. ~I'm sorry about her. I really am. People do go missing in jumps. They go somewhere. That's the thing: they go somewhere.~";
           "~Later,~ says Ford. ~Things are about to go wrong. I'd like us to be ready.~";
       ],
  has animate male proper;

Object fordship "Ford's ship"
  with name 'fordship' 'battered' 'little',
       description "A dented, battered little ship that has been landed by someone in a hurry.",
       initial "Ford's ship sits crookedly on the grass, still ticking. ",
  has static;

! ========================================================================
!  Story clock
! ========================================================================


Object Director
  with daemon [;
         FollowStep();
         HintTick();
         if (act >= 2) { ActTick(); return; }
         switch (phase) {
           0:
             pt++;
             if (hob_fed) {
                 phase = 1;
                 pt = 0;
                 CourierArrives();
             }
             else if (pt == 40 && location == SandHut)
                 print "^Hob coughs, gently, in the manner of a man who is making a point.^";
           1:
             pt++;
             if (parcel has open && guide_revealed == 0) { guide_revealed = 1; pt = 0; }
             if (guide_revealed) {
                 if (pt >= 4) TrillianArrives();
             } else if (pt == 25)
                 print "^You find yourself wondering about the brown parcel on the village green.^";
           2:
             pt++;
             if (pt == 8) TrillianLeaves();
           3:
             pt++;
             if (random_fed && pt >= 2) FordArrives();
             else if (pt % 14 == 0 && random_fed == 0)
                 print "^Random, who has been looking at the sandwich hut with great indifference for some time, says to nobody in particular: ~I'm not hungry.~^";
           4:
             pt++;
             if (pt == 9) RandomFlees();
           5:
             pt++;
             if (pt % 12 == 0 && pilot_fed == 0 && location ~= Meadow && location ~= SandHut)
                 print "^~The pilot, Arthur,~ says Ford. ~Pickled roots on bread, nothing else. Hurry.~^";
         }
       ];

[ CourierArrives;
    move parcel to Green;
    move cord to Green;
    move skiff to Meadow;
    move pilot to Meadow;
    print "^Far overhead, a thin whine builds, as of a mosquito with ambitions. Some minutes later a child skids in to shout that the mail hopper has been, and has left a parcel on the village green with the sandwich maker's name on it. Nobody on Lamuella has ever received a parcel before. The hopper itself, you gather, has landed on the meadow and is not yet sure it will leave.^";
];

[ TrillianArrives;
    phase = 2; pt = 0;
    move Trillian to location;
    move Daughter to location;
    random_follows = 1;
    print "^A sound like tearing silk comes down out of the sky, and a sleek white shuttle puts itself carefully down in the meadow. Presently two people come looking for you: a woman, brisk and tired, and a girl who walks several paces behind with her hands in her pockets as if she had been delivered against her will.^^~Hello, Arthur,~ says the woman. You know her at once. Of all the people you did not expect to see on Lamuella, she is the first. ~This is Random. She's your daughter.~^^She says it the way one reports a delay on a railway. The girl says nothing, which she does with great force.^";
    if (guide has on)
        print "^The cube, in your pocket, produces a tiny hum. A line of text writes itself on its face where only you will see it: ~A DAUGHTER. HOW NICE FOR YOU. THE GUIDE HAS UPDATED YOUR ITINERARY.~^";
];

[ TrillianLeaves;
    phase = 3; pt = 0;
    remove Trillian;
    print "^Trillian consults something on her wrist that is not a watch. ~I have to go,~ she says. ~There is a story in another galaxy, and it won't wait. Look after her. She is excellent at looking after herself, which is not the same thing.~ She kisses her daughter on the top of the head, which Random permits as one permits weather, and walks away up the meadow. A moment later the white shuttle goes up like a breath being let out.^^Random, who now has a father whom she has met for ten minutes, regards the hut roof with extreme attention.^";
];

[ FordArrives;
    phase = 4; pt = 0;
    move fordship to Meadow;
    move Ford to location;
    ford_follows = 1;
    print "^A rattling, wheezing noise announces that someone has landed a ship on the meadow without much regard for its feelings. A little while later Ford Prefect strides in, hair on end, towel over his shoulder.^^~Arthur!~ he says. ~Good. You got it. Please tell me you didn't switch it on.~ He glances at Random, then at you. ~Ah. A great deal has happened since I last checked. Sit down. Or don't.~^";
];

[ RandomFlees;
    phase = 5; pt = 0;
    remove Daughter;
    random_follows = 0;
    remove fordship;
    print "^While you were listening to Ford, Random has done three things quietly: looked at the ship on the meadow, worked out who it belongs to, and noticed that its owner is distracted. By the time you notice, there is a noise from the meadow like a large animal clearing its throat. Through the doorway you see a battered ship wobble upward, pick up speed, and go.^^Ford stares after it. ~Of course she took my ship,~ he says. ~Of course she did.~ Then he is practical, which is the most alarming thing about him. ~The mail skiff. It's on the meadow with a pilot who is, as I understand it, hungry. Make him something he wants and he'll fly us anywhere within reason. She'll head for Earth, or what she thinks is Earth. We'll have to find out where that is.~^";
    if (guide has on)
        print "^~I COULD TELL YOU,~ writes the cube, helpfully, on its face. ~BUT YOU DO NOT SEEM TO WANT TO KNOW YET.~^";
];

[ FollowStep;
    if (ford_follows && Ford notin location) {
        move Ford to location;
        print "^Ford ambles in after you, towel over one shoulder.^";
    }
    if (tricia_follows && Tricia notin location) {
        move Tricia to location;
        print "^Tricia follows, taking in the place with a journalist's eye.^";
    }
    if (random_follows && Daughter notin location) {
        move Daughter to location;
        print "^Random drifts in after you at a distance she clearly considers dignified.^";
    }
];

[ ActOneEnd;
    Award(A_SKIFF, 3);
    act_ended = 1;
    print "^The pilot checks nothing in particular, says a word to the instrument panel, and the world drops out from under you. There is a sensation like missing a step in the dark, followed by another like finding it. For one long instant in the middle of the jump you think of a girl who stepped into one of these and was not on the other side; you grip the armrest until the feeling passes.^^~Not long now,~ says Ford, mostly to himself.^^";
    StartAct2();
];
