!  act2.h -- Act II: Hub Station.  Six rooms, seven puzzles, 50 points.
!
!  Skiff Cabin - Arrivals - Customs Hall - Concourse - Departure Gate
!                                              |
!                                        Lost Property

Object coin "coin of no known denomination"
  with name 'coin' 'denomination' 'money',
       description "A small, heavy, yellowish coin with a worn figure on one side and what might be a joke on the other. Nobody has ever been able to say where it is legal tender, which, you have noticed, is how it gets accepted everywhere.",
       before [;
         Insert:
           if (second == vending) {
               remove coin;
               move ticket to player;
               Award(A_COIN, 6);
               "The machine considers the coin for a long time, rejects nothing, and with a shudder produces a thin grey platform ticket. ~Thank you,~ it says, to nobody, and sounds as though it means it.";
           }
       ];

Object towel "your old towel"
  with name 'towel' 'old' 'stain' 'norway',
       description "A faded towel with a long, ragged stain across one corner that, from the right angle, has an unmistakable coastline. Fjords, in particular. Someone designed it, and not on purpose.",
  has clothing proper;

Object ticket "platform ticket"
  with name 'platform' 'ticket' 'pass',
       description "A thin grey slip stamped DEPARTURES: GATE ONE. It has a hole punched in the shape of a small, disappointed face.";

! ------------------------------------------------------------------------

Object SkiffCabin "The Skiff Cabin"
  with description
         "The inside of the mail skiff, which is mostly instrument panel, with seats added later as an afterthought. Beyond the glass there is the particular grey that means a jump is in progress. You are not meant to look at it, and you do.",
       out_to [;
         if (cabin_t < 3) "You are in the middle of a jump. Leaving now would not be a good idea.";
         return Arrivals;
       ],
  has light;

Object panel "instrument panel" SkiffCabin
  with name 'instrument' 'panel' 'dials' 'controls',
       description "Dials, switches, and one handwritten label that says DO NOT. Nothing says what.",
  has scenery;

Object Arrivals "Hub Station: Arrivals"
  with description
         "A long, low arrivals hall under a ceiling of strip lights, with the sort of carpet that was chosen to hide things. A kiosk stands in the middle of the floor; a plant, a very old plant, waits in a pot beside it. A sign says QUEUE HERE and points at the floor. Customs is to the east.",
       e_to Customs,
       w_to [; "The skiff has already gone. The pilot left the second you stepped off, and the dock is a blank wall."; ],
  has light;

Object kiosk "information kiosk" Arrivals
  with name 'kiosk' 'information' 'machine' 'screen',
       description "A tall kiosk with a screen that glows with the calm of something that has never been wrong, and is not about to start.",
       life [;
         Ask, Answer, Tell:
           "~Customs is to your west,~ says the kiosk, with great confidence. It is not. ~Lost Property is closed,~ it adds. It is not. ~Have a nice day,~ it concludes, and the screen dims.";
       ],
  has animate scenery;

Object plant "plant" Arrivals
  with name 'plant' 'pot',
       description "A large, grey-green plant whose label says: WAITING SINCE ARRIVAL. It looks at ease.",
  has scenery;

Object Customs "Customs Hall"
  with description [;
         print "A pale green hall divided by a counter and a pane of glass, behind which Clerk Gruba sits like a very large moth in a very small cupboard. A rack of forms stands by the glass, with a pen on a chain beside it; an enormous sign says FORM 27B/6 OR NO ONE PASSES. There is a bin marked ITEMS TO DECLARE (it is full of nothing). Arrivals is to the west and the concourse lies east, past Gruba.";
         new_line;
         rtrue;
       ],
       w_to Arrivals,
       e_to [;
         if (form_state == 3 && form in player) {
             print "Gruba looks at the stamped form for slightly longer than necessary. ~Next,~ he says, in the voice of a man who has decided to let it go.^";
             return Concourse;
         }
         "Gruba does not look up. ~Form 27B/6,~ he says. ~Completed, signed, stamped.~";
       ],
  has light;

Object glass "pane of glass" Customs
  with name 'glass' 'pane' 'counter' 'window',
       description "Thick, scratched glass with a speaking-slot at the bottom. On the other side, Gruba's desk holds a rubber stamp on a stand.",
  has scenery;

Object rack "rack of forms" Customs
  with name 'rack' 'forms' 'stack',
       description "A rack of identical forms, each headed FORM 27B/6 and each requiring an astonishing amount of detail.",
       before [;
         Take, Search: <<Take form>>;
       ],
  has scenery;

Object form "Form 27B/6" Customs
  with name 'form' 'paperwork' 'paper',
       description [;
         switch (form_state) {
           0, 1: "A tall, pale form with boxes for everything, including the box where you put the other boxes. It is blank.";
           2: "The form, completed in your own hand. Name: Arthur Dent. Purpose of visit: personal. It is waiting for a stamp.";
           3: "The form, completed and stamped. The stamp has a small smug crest.";
         }
       ],
       before [;
         Fill: return FillFormSub();
       ],
       after [;
         Take: if (form_state == 0) form_state = 1;
       ];

Object stamp_ob "rubber stamp" Customs
  with name 'stamp' 'rubber' 'stand',
       description "A rubber stamp on a stand, behind the glass, at Gruba's elbow.",
       before [;
         Take: "It is behind the glass, at Gruba's elbow. Not for the likes of you.";
       ],
  has scenery;

Object bin "ITEMS TO DECLARE bin" Customs
  with name 'bin' 'declare' 'items',
       description "A bin, entirely empty. Its lid is sealed. Someone has labelled it, in a small hand: ALSO EMPTY.",
  has scenery;

Object Gruba "Clerk Gruba" Customs
  with name 'gruba' 'clerk' 'moth' 'man',
       description "A large, pale clerk wedged behind the glass, with a face like an unironed shirt and fingers permanently curled round a pen.",
       initial [;
         print "Clerk Gruba sits behind the glass, framed like a portrait of someone who has never been surprised. ";
         if (gruba_away) print "At the moment he is staring out of the window at a queue that is not there.";
         new_line;
         rtrue;
       ],
       life [;
         Ask, Answer, Tell:
           if (TopicAny('form', 'paper', 'paperwork', '27b', 'stamp', 'sign'))
               "~Form 27B/6,~ says Gruba. ~Completed, signed, stamped. The stamp is behind the glass. You are not.~";
           if (TopicAny('queue', 'queues', 'line', 'wait', 'theory'))
               "Gruba's eyes light up for a moment, like a candle in a draught. ~Queueing,~ he says. ~Now there is a field.~ He seems to remember himself.";
           if (TopicAny('ford', 'prefect'))
               "~Mr Prefect,~ says Gruba, with the weariness of a man who has met Ford before. ~Please do not start talking again.~";
           if (TopicAny('fish', 'towel', 'cube', 'guide'))
               "~Nothing to declare?~ says Gruba, who has not looked up from the paperwork. ~Excellent. The bin is for nothing.~";
           "~Form 27B/6,~ says Gruba.";
       ],
  has animate male proper static;

! ------------------------------------------------------------------------

Object Concourse "The Concourse"
  with description
         "A wide, bright concourse under a roof like a very clean sky. Benches face nothing in particular; shop windows display the sort of goods that make you realise you've been away. A vending machine hums against one wall. Customs lies back to the west, a sign points east to the Departure Gate, and a small doorway to the south says LOST PROPERTY in letters that have given up.",
       w_to Customs,
       e_to Gate,
       s_to LostProp,
  has light;

Object vending "vending machine" Concourse
  with name 'vending' 'machine' 'slot' 'dispenser',
       description [;
         print "A tall machine with a slot, a flap, and a sign: EXACT CHANGE ONLY. The coin return has a handwritten note on it: ALSO NO.";
         if (difficulty == DIFF_EASY) print " In very small letters along the slot: ACCEPTS ANY COIN NOBODY RECOGNISES.";
         new_line;
         rtrue;
       ],
       before [;
         Take: "It is bolted to the floor, and to the building, and apparently to the idea of being a vending machine.";
       ],
  has static container open scenery;

Object benches "benches" Concourse
  with name 'bench' 'benches' 'seats',
       description "Wide, cold benches that face the shops. Each has a small brass plate: IN MEMORY OF A WAITING FRIEND.",
  has scenery pluralname;

Object windows "shop windows" Concourse
  with name 'window' 'windows' 'shops' 'shop' 'goods',
       description "Shops selling the sorts of things travellers buy so they can have had the trip.",
  has scenery pluralname;

Object speaker "announcement speaker" Concourse
  with name 'speaker' 'announcement' 'announcements' 'voice' 'loudspeaker',
       description "A grille in the ceiling. A tired voice comes out of it at intervals.",
  has scenery;

Object LostProp "Lost Property"
  with description
         "A cramped office stacked to the ceiling with lost things: umbrellas, a trumpet, one enormous boot, several thousand single gloves. The Attendant's counter is at the back. The concourse is back to the north.",
       n_to Concourse,
  has light;

Object lp_shelves "shelves of lost things" LostProp
  with name 'shelves' 'things' 'umbrellas' 'gloves' 'boot' 'trumpet' 'stack' 'heap' 'crate',
       tries 0,
       description "Row after row of someone's lost something. You get the feeling that nobody has ever come back for a single one.",
       before [;
         Search:
           if (eg-->EG_FLOYD ~= 0)
               "You have found all that the heap has to give, and one thing it did not know it had.";
           self.tries = self.tries + 1;
           if (self.tries == 1)
               "You poke about. A crate rattles, then thinks better of it. The Attendant, who knows where everything is, would find what you want in about a second; but you are not here for what you want.";
           if (self.tries == 2)
               "You work your way deeper, behind the trumpet and the enormous boot. Something in the crate whirs, very quietly, in the manner of a person trying not to giggle.";
           FloydFound();
           rtrue;
       ],
  has scenery pluralname;

Object pen "pen on a chain" Customs
  with name 'pen' 'chain' 'biro',
       description "A cheap pen on a very long chain, fixed to the counter. It writes.",
       before [;
         Take: "It is chained to the counter. You may write with it here, at the counter, if you've something to write on.";
       ],
  has scenery;

Object lp_sandwich "unclaimed sandwich" LostProp
  with name 'unclaimed' 'sandwich' 'sarnie' 'lunch',
       description "A sandwich in a paper wrapper, labelled UNCLAIMED. Inside, by the look of it, are bread and honey. It has been unclaimed for long enough that the label has faded.",
       before [;
         Take:
           if (parent(sandwich) ~= nothing) "You are already carrying a sandwich.";
           sand_flags = BIT_HONEY;
           move sandwich to player;
           remove self;
           "The Attendant waves a hand without looking: ~Take it. It has been there since before the Collapse of Something.~ You take it. It is, as far as you can tell, a honey sandwich.";
       ];

Object Attendant "Lost Property Attendant" LostProp
  with name 'attendant' 'wendell' 'man' 'clerk',
       description "A wiry, patient man in a brown coat, who looks as though he has been a part of the furniture for so long that the furniture has begun to look after him.",
       initial "The Attendant stands behind the counter, with the air of a man who can find anything and would prefer you hadn't lost it. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('towel', 'stain', 'norway', 'fjord', 'fjords', 'norwegian', 'coast')) {
               if (towel in player || towel has worn) "~You have it already,~ says the Attendant, not unkindly.";
               if (TopicAny('norway', 'fjord', 'fjords', 'norwegian', 'coast')) {
                   move towel to player;
                   Award(A_TOWEL, 8);
                   "The Attendant's eyes close for a moment. ~Brown, frayed, a stain shaped like the coast of Norway,~ he says. ~Slartibartfast's.~ He disappears among the shelves and returns with your old towel, as though he had known all along. ~Thanks for describing it,~ he says. ~They mostly can't.~";
               }
               "~Describe it,~ says the Attendant. ~Exactly. There's a stain. Which country is it the shape of?~";
           }
           if (TopicAny('sandwich', 'lunch', 'unclaimed', 'food'))
               "~Help yourself,~ says the Attendant. ~Nobody's claimed that in years.~";
           if (TopicAny('pen', 'chain', 'form'))
               "~You can write at the counter,~ says the Attendant. ~Not elsewhere. It's a long chain, but not that long.~";
           if (TopicAny('ford', 'cube', 'guide', 'fish'))
               "~I find things,~ says the Attendant. ~I don't ask what they are.~";
           "~Describe what you've lost,~ says the Attendant. ~Exactly. Then I'll fetch it.~";
       ],
  has animate male proper static;

Object Gate "Departure Gate"
  with description
         "A wide, empty gate hall with a single moving walkway that doesn't. A clearance desk stands at the end; behind it, through a large window, a ship waits, ticking. A probability gauge on the wall twitches uneasily. The concourse is back to the west.",
       w_to Concourse,
  has light;

Object gauge "probability gauge" Gate
  with name 'gauge' 'probability' 'dial' 'needle',
       description "A large dial with a needle that moves slightly, as if embarrassed. It points to PLAUSIBLE, but only just.",
  has scenery;

Object cl_desk "clearance desk" Gate
  with name 'desk' 'clearance' 'counter',
       description "A counter with a bell and a sign: PROBABILITY CLEARANCE: PLEASE BE PATIENT. The Clerk behind it looks a little hungry.",
  has scenery;

Object Clerk "clearance clerk" Gate
  with name 'clerk' 'clearance' 'woman',
       wantany BIT_HONEY,
       sorry "sniffs it. ~Not sweet,~ she says. ~Honey, if you've any. A person has standards.~",
       eaten [;
         Award(A_CLEAR, 10);
         cleared = 1;
         "The Clerk eats it slowly, with her eyes shut. ~Probability clearance,~ she says at last, stamping a card without looking at it. ~Granted. Go on. Don't tell the others.~";
       ],
       description "A tired woman in a grey uniform with a very small desk lamp. She looks like someone who has been waiting for a good lunch since the last board meeting.",
       initial "The Clerk sits behind the desk, with a look of long and practised hunger. ",
       life [;
         Give: if (noun == sandwich) return ServeSandwich(self);
         Ask, Answer, Tell:
           if (TopicAny('clearance', 'permit', 'permission', 'ship', 'probability'))
               "~Probability clearance,~ says the Clerk. ~It takes weeks. Or an afternoon, if the applicant is pleasant. A pleasant applicant brings something sweet.~";
           if (TopicAny('sandwich', 'honey', 'food', 'hungry', 'lunch'))
               "~A sandwich,~ says the Clerk wistfully. ~With honey. I'd say that was a proper form of application.~";
           if (TopicAny('ford', 'prefect', 'cube', 'guide'))
               "~Mr Prefect has a file,~ says the Clerk. ~I've not been asked to open it.~";
           "~Clearance,~ says the Clerk, vaguely, ~is a question of mood.~";
       ],
  has animate female static;

Object gate_ship "waiting ship" Gate
  with name 'ship' 'waiting' 'craft' 'vessel',
       description "A small grey shuttle with RESEARCH VESSEL stencilled on it. It is going to Ursa Minor. You have a bad feeling about this, and a good one about the Guide's offices.",
       before [;
         Enter:
           if (ticket notin player) "The gate beeps, politely, and doesn't open. You haven't a ticket.";
           if (cleared == 0) "The gate beeps, politely, and doesn't open. The probability gauge twitches. You haven't a clearance.";
           ActTwoEnd();
           rtrue;
       ],
  has static enterable scenery;

! ========================================================================
!  Verbs and routines for Act II
! ========================================================================

[ FillFormSub;
    if (noun ~= form) "That isn't something you can fill in.";
    if (form notin player) {
        if (form notin location) "You haven't got a form.";
        move form to player;
        form_state = 1;
        print "(first taking the form)^";
    }
    if (form_state >= 2) "You have filled it in already, to the best of your ability.";
    if (location ~= Customs) "You haven't anything to write with. A pen on a chain hangs by the forms in the Customs Hall.";
    form_state = 2;
    Award(A_FORM, 4);
    "You fill in the form at the counter, using the chained pen: Name, Arthur Dent. Address, the sandwich hut, Lamuella. Purpose of visit: personal. There is a box for Other, and you leave it blank, which seems to be correct.";
];

[ StampSub;
    if (location ~= Customs) "There's nothing to stamp here.";
    if (noun ~= 0 && noun ~= form) "That isn't something you should stamp.";
    if (form notin player) "You haven't got the form.";
    if (form_state == 3) "It is stamped already.";
    if (form_state < 2) "It is blank. Gruba would notice that, even without looking.";
    if (gruba_away == 0)
        "Gruba's eye is on you, the way a lighthouse is on a ship. Perhaps when he is looking elsewhere.";
    form_state = 3;
    Award(A_STAMP, 6);
    "While Gruba glares out of the window at the queue that isn't there, you reach round the end of the glass, find the stamp by feel, and bring it down on the form with the quiet efficiency of a man who has been to a great many offices. The stamp has a small, smug crest.";
];

[ ActTwoEnd;
    FloydParts();
    print "^You show the ticket to nobody in particular, and the gate opens. The ship's door swings up with a sigh. Ford gives you a nod that is partly encouragement and partly the nod of a man who has had a bad idea and is committed to it.^^";
    StartAct3();
];

! Late-act Ford: returns true if it answered
[ FordTalkLate;
    if (act == 4 && FordTalkLate4()) rtrue;
    if (act == 2) {
        if (TopicAny('towel', 'stain', 'norway', 'lost')) {
            "~Your towel is probably in Lost Property,~ says Ford. ~You'll need to describe it exactly. The stain, remember: that was Slartibartfast's best work. It's the coastline of somewhere with a great many fjords. You'll know it when you think about it.~";
        }
        if (TopicAny('gruba', 'customs', 'form', 'stamp', 'queue', 'clerk')) {
            "~Gruba,~ says Ford. ~Never looks away. Unless you give him something to look away from. Leave that to me. When he turns to the window, you'll have about a breath. Don't waste it.~";
        }
        if (TopicAny('clearance', 'gate', 'ship', 'probability', 'honey', 'sandwich')) {
            "~The clearance clerk,~ says Ford. ~Bureaucrats love something homemade, especially if it is sweet. Honey, I'd say. Never underestimate a sandwich.~";
        }
        if (TopicAny('cube', 'guide', 'mk', 'ii')) {
            if (cube_knew && awarded->A_SUSPECT == 0) {
                Award(A_SUSPECT, 5);
                "Ford looks at the cube for a long moment. ~It told you what the clerk was going to say,~ he says, slowly, and takes out a pocket notebook, which he does not often do. ~Before he'd said it. I'm writing that down. Don't tell it I wrote it down.~";
            }
            "~Keep it in your pocket,~ says Ford. ~Don't encourage it.~";
        }
        if (TopicAny('coin', 'coins', 'ticket', 'machine', 'vending', 'money')) {
            if (ticket in player) "~You have your ticket,~ says Ford. ~Keep it somewhere dull.~";
            if (coin notin player && awarded->A_COIN == 0) {
                move coin to player;
                "~Lost your coin?~ says Ford, patting his pockets. ~Everyone does. I always carry a few that nobody recognises. Here: try this one in the machine.~ He presses a small, heavy coin into your hand.";
            }
            "~The machine takes whatever nobody recognises,~ says Ford. ~Try your coin. It is very probably the right sort.~";
        }
        if (TopicAny('station', 'hub', 'plan', 'where', 'next'))
            "~Ursa Minor,~ says Ford. ~The Guide's offices. Where the story gets written. We need to know what the girl was told, and by whom. That means getting a ticket, a clearance and a form that Gruba will stamp.~";
    }
    if (act == 3) {
        if (TopicAny('badge', 'terminal', 'visitor', 'register'))
            "~Say it is routine,~ says Ford. ~Say audit. Say compliance. Say anything that sounds like a form that was filled out in advance. They never listen to the second word.~";
        if (TopicAny('scanner', 'fish', 'cloakroom', 'jar', 'knife', 'security'))
            "~Don't take the fish through,~ says Ford. ~The scanner will notice a translator at a hundred yards. Pim in the cloakroom is trustworthy, in a gloomy way.~";
        if (TopicAny('lift', 'lifts', 'elevator'))
            "~Lifts,~ says Ford. ~Sirius Cybernetics. The cheerful one wants praise; the grey one wants sympathy. Try kindness. It works on more machines than you would think.~";
        if (TopicAny('desk', 'combination', 'lock', 'drawer'))
            "~I always set it to the Earth entry,~ says Ford, with a hint of embarrassment. ~Draft by draft. I was sentimental. The drafts are in the archive.~";
        if (TopicAny('orlo', 'poem', 'poetry', 'stairs', 'server'))
            "~Vogons adore bad poetry,~ says Ford. ~They can't help it. Give him the worst you have. The more it hurts, the better.~";
        if (TopicAny('boardroom', 'meeting', 'towel', 'hide', 'vent'))
            "~If you can't see them, they can't see you,~ says Ford. ~It is the oldest rule in the Galaxy and it has never once failed. Put your towel over your head.~";
        if (TopicAny('core', 'cube', 'mk', 'ii', 'coordinates'))
            "~It tells you what you want to hear,~ says Ford. ~Make it tell you the thing you don't.~";
    }
    rfalse;
];

! Per-turn events for acts 2 and 3
[ ActTick;
    if (act == 2) {
        ! the skiff cabin
        if (location == SkiffCabin) {
            cabin_t++;
            if (cabin_t == 1 && guide hasnt on) {
                give guide on;
                print "^In your pocket, the cube switches itself on. A line of text surfaces on its face: GOOD. THE GUIDE WAS GETTING BORED OF THE DARK.^";
            }
            if (cabin_t == 3) {
                remove pilot;
                print "^~Hub Station,~ says the pilot. ~Mind the gap.~ There is no gap. You mind it anyway.^";
                PlayerTo(Arrivals);
            }
            return;
        }
        ! the cube knows the clerk's words before he says them
        if (location == Customs && cube_knew == 0 && guide has on) {
            cube_knew = 1;
            print "^The cube writes on its face, in neat capitals, before Gruba has so much as looked up: FORM 27B/6. YOU WERE ABOUT TO ASK. A moment later, from behind the glass, Gruba says exactly that, in exactly that tone.^";
        }
        ! Ford distracts Gruba
        if (location == Customs && form_state == 2 && Ford in location) {
            gruba_t++;
            if (gruba_t == 1)
                print "^Ford leans on the counter and clears his throat. ~Have you ever considered,~ he says to Gruba, ~the mathematics of queues?~^";
            if (gruba_t == 2)
                print "^~Queues,~ says Gruba, in the tone of one who has been lured by his favourite subject and knows it, ~are not a mathematical object.~ ~Aren't they?~ says Ford, delighted.^";
            if (gruba_t == 3) {
                gruba_away = 1;
                Award(A_DISTRACT, 6);
                print "^~And another thing,~ says Ford loudly, and points out of the window. Gruba turns, with something like longing, to look at the queue that is not there. His stamp stands unattended.^";
            }
            if (gruba_t == 4 && difficulty ~= DIFF_EASY) {
                gruba_away = 0;
                print "^Gruba's attention comes back to the room, like a blind being drawn.^";
            }
            if (gruba_t == 5) {
                gruba_away = 0;
                gruba_t = 0;
            }
        } else if (location ~= Customs) {
            gruba_away = 0;
            gruba_t = 0;
        }
        ! the voice on the concourse
        if (location == Concourse) {
            ann_t++;
            if (ann_t % 8 == 1) {
                switch ((ann_t / 8) % 3) {
                  0: print "^~This is a public announcement,~ says a very tired voice from the ceiling. ~I'm told it is a pleasure to serve you. I wouldn't know.~^";
                  1: print "^~Would the owner of a depressed robot,~ says the voice, ~kindly collect it. Not that anyone will.~^";
                  2: print "^~Passengers are reminded that the end of the Universe is not a delay,~ says the voice, ~and will not be treated as one. This message has been brought to you by Marvin, who is wasted on it.~^";
                }
            }
        }
        return;
    }
    if (act == 3) Act3Tick();
    if (act == 4) Act4Tick();
    if (act == 5) Act5Tick();
    if (act == 6) Act6Tick();
];

[ StartAct2;
    act = 2;
    pt = 0;
    ford_follows = 1;
    random_follows = 0;
    move Ford to SkiffCabin;
    move pilot to SkiffCabin;
    PlayerTo(SkiffCabin);
];
