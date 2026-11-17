!  act3.h -- Act III: Guide HQ, Ursa Minor.  Eleven rooms, ten puzzles, 100 points.
!
!   Cloakroom - Security Desk - Lobby - Plaza
!                    |
!               Lift Lobby
!                    |
!   Archive - Editorial Floor - Ford's Old Office
!                 |      \
!            Boardroom   Server Floor - Roof (up)

Object badge "visitor badge"
  with name 'badge' 'visitor' 'sticker' 'pass',
       description "A white plastic badge reading VISITOR, and under it, in smaller print, PLEASE DO NOT LEAVE THE DESIGNATED ROUTE (NONE DESIGNATED).",
  has clothing;

Object oldpass "Ford's old pass"
  with name 'old' 'pass' 'ids' 'card' 'staff',
       description "A staff pass with Ford's photograph on it, the expression on his face being a mixture of boredom and mischief. Under STATUS, someone has typed: LAPSED (UNRESOLVED).";

Object tag "cloakroom tag"
  with name 'tag' 'cloakroom' 'ticket' 'claim',
       description "A brass tag stamped with a number and the words YOUR PROPERTY IS SAFE WITH US. You would like very much to believe this.";

! ------------------------------------------------------------------------

Object Plaza "Ursa Minor Plaza"
  with description
         "A broad, spotless plaza of grey stone under a tall, bright sky. A great glass tower stands over it, its sign reading INFINIDIM ENTERPRISES in a typeface designed to be unreadable from the outside. A Vogon smoking area, marked by a ring of overflowing bins, fills one corner. A ship dock lies off to the side. The lobby doors are to the west.",
       w_to Lobby,
  has light;

Object tower "glass tower" Plaza
  with name 'tower' 'glass' 'building' 'sign' 'infinidim',
       description "Sixty floors of glass, at the top of which is the thing you have come to see. At its base, in smaller letters, the former name of the company is still just visible: MEGADODO PUBLICATIONS. Someone has painted over it, badly.",
  has scenery;

Object smokers "Vogon smoking area" Plaza
  with name 'vogon' 'vogons' 'smokers' 'smoking' 'bins',
       description "Three Vogons stand in a ring, smoking with the grim concentration of people for whom pleasure is a form of paperwork. One of them is composing something in his head. You can tell by the wincing.",
  has scenery;

Object Lobby "The Lobby"
  with description
         "A cavernous lobby of white marble and greyer ambition. A terminal on a stand offers VISITOR REGISTRATION; a fountain in the middle spills pamphlets into a basin. The security desk is west, the plaza east.",
       e_to Plaza, w_to Security,
  has light;

Object terminal "visitor terminal" Lobby
  with name 'terminal' 'visitor' 'registration' 'screen' 'reception' 'keyboard',
       description [;
         if (badge in player || badge has worn) "The terminal shows a cheerful message: ENJOY YOUR VISIT.";
         "A glossy terminal with a screen that says PURPOSE OF VISIT? and a cursor that blinks, patiently, at the exact rate of a man tapping his foot. You can TYPE an answer.";
       ],
  has static scenery;

Object fountain "pamphlet fountain" Lobby
  with name 'fountain' 'pamphlets' 'pamphlet' 'basin',
       description "A fountain that spills a slow curl of paper into a marble basin: pamphlets, leaflets and notices. DIVERSITY AND COMPLIANCE: A GUIDE. YOUR RIGHTS (SUSPENDED). YOUR RIGHTS (RE-INSTATED, WITH CONDITIONS).",
       before [;
         Take: "You pick up a pamphlet. It is called WELCOME TO THE NEW GUIDE. The inside is blank, but it reads you very closely.";
       ],
  has scenery;

Object Security "Security Desk"
  with description
         "A pale desk across a narrow hall, with a scanner arch behind it and a Vogon behind that. To the north, past the arch, are the lifts. The lobby is east, and a hatch on the west wall says CLOAKROOM.",
       e_to Lobby, w_to Cloakroom,
       n_to [;
         if (badge notin player && oldpass notin player)
             "Kess raises a hand the size of a tray. ~Badge,~ he says. ~Then the scanner.~";
         if (IndirectlyContains(player, fish)) {
             "The arch lights up and begins to scream, politely. ~BIOLOGICAL MATERIAL,~ it says. ~TRANSLATOR, UNLICENSED.~ Kess looks pained. ~Leave it in the cloakroom, sir.~";
         }
         if (knife in player)
             "The arch squeals. ~SHARP OBJECT,~ it says. ~SANDWICH-RELATED.~ ~The cloakroom,~ says Kess, wearily, ~is just west.~";
         if (badge notin player) {
             move oldpass to nothing;
             move badge to player;
             Award(A_BADGE, 8);
             print "Kess scans Ford's old pass and the screen on his desk flashes up a name and a number and, for a moment, a long line of red. ~Lapsed,~ he says. ~But so's most of this building.~ He prints you a visitor badge and hands it over without enthusiasm, then waves you through the arch.^";
             Award(A_SCAN, 8);
             return LiftLobby;
         }
         Award(A_SCAN, 8);
         print "You walk through the arch. It says nothing, with enormous tact, and Kess waves you through without looking up.^";
         return LiftLobby;
       ],
  has light;

Object scanner "scanner arch" Security
  with name 'scanner' 'arch' 'metal' 'detector',
       description "A grey arch that hums when you look at it. It is the sort of arch that notices things and then makes a fuss.",
  has scenery;

Object Kess "Vogon guard Kess" Security
  with name 'kess' 'vogon' 'guard' 'man',
       description "A Vogon, broad and green and weary, in a grey uniform with the strained look of a creature that has never wanted to do anyone any harm and has done it anyway.",
       initial "Kess sits behind the desk, filling in a form about forms. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('badge', 'terminal', 'pass', 'visit'))
               "~You get a badge from the terminal,~ says Kess. ~Or you show me a pass. Nobody's shown me a pass since the takeover.~";
           if (TopicAny('scanner', 'arch', 'fish', 'knife', 'bag', 'search'))
               "~The arch objects to biological material, weapons and wit,~ says Kess. ~Leave it in the cloakroom. They're very good.~";
           if (TopicAny('poetry', 'poem', 'orlo'))
               "Kess brightens a little. ~You'll want Orlo,~ he says. ~Upstairs. He's the poet of the building. He guards the stairs to the servers. Nobody comes down the same.~";
           if (TopicAny('ford', 'prefect'))
               "~Mr Prefect,~ says Kess. ~We had a memo about him. It was quite short.~";
           "~Move along, sir,~ says Kess.";
       ],
  has animate male proper static;

Object Cloakroom "The Cloakroom"
  with description
         "A cramped cloakroom with a counter, a rail of coats nobody has collected, and a tiered shelf of umbrellas. The attendant keeps it with an almost religious attention. The security desk is east.",
       e_to Security,
  has light;

Object Cloaker "Pim, the attendant" Cloakroom
  with name 'pim' 'attendant' 'man',
       description "A neat, small Vogon, unusual in his gentleness, with a face like a worried potato and a very clean apron.",
       initial "Pim stands behind the counter, wiping a brass tag on his apron. ",
       life [;
         Give:
           if (noun == jar || noun == fish || noun == knife) {
               move noun to self;
               if (tag notin player && tag notin self) {
                   move tag to player;
                   "Pim receives it with both hands, hangs it on a hook, and gives you a brass tag. ~Your property is safe with us,~ he says, with the awful sincerity of someone who has never had a reason to doubt it.";
               }
               "Pim hangs it carefully on its hook.";
           }
           if (noun == tag) {
               if (child(self) == 0) "~Nothing of yours is here,~ says Pim, gently.";
               while (child(self) ~= 0) move child(self) to player;
               remove tag;
               "Pim hands everything back, one item at a time, and takes the tag with a little bow.";
           }
           "~Thank you, but I couldn't,~ says Pim.";
         Ask, Answer, Tell:
           if (TopicAny('tag', 'property', 'jar', 'fish', 'knife', 'coat'))
               "~Hand it in and I'll give you a tag,~ says Pim. ~Hand back the tag and I'll give it all back. That is the only way I know of being useful.~";
           "~I mind the coats,~ says Pim. ~It's a living.~";
       ],
  has animate male proper static;

Object LiftLobby "Lift Lobby"
  with description
         "A white hall with two lifts, one bright and one grey, side by side. A directory on the wall lists SIXTY FLOORS, each in the same typeface. A service robot sits in the corner, with its head against the wall. The security desk is back south. A button says UP.",
       s_to Security,
       u_to [;
         if (lift_ok == 0)
             "The bright lift opens its doors with a ping, then thinks better of it. The grey lift does not open at all. Neither seems inclined to take you up without some courtesy.";
         print "The lift glides up through the tower with a feeling of being politely rearranged, and lets you out on the Editorial Floor.^";
         return Editorial;
       ],
  has light;

Object cheer_lift "cheerful lift" LiftLobby
  with name 'cheerful' 'bright' 'left' 'lift' 'lifts' 'elevator',
       description "A gleaming lift whose doors seem to be smiling, which is a worse thing for a lift than you would think.",
       life [;
         Ask, Answer, Tell:
           "~Hello!~ says the lift. ~It's a wonderful day for going up!~ It waits, brightly, for some response.";
       ],
       before [;
         Enter: <<Go u_obj>>;
       ],
  has animate scenery;

Object sullen_lift "sullen lift" LiftLobby
  with name 'sullen' 'grey' 'right' 'lift' 'lifts' 'elevator',
       description "A dull lift with its doors shut. It manages to look unwelcoming even from behind a door.",
       life [;
         Ask, Answer, Tell:
           "~Nobody ever asks the grey one,~ says the lift, in a voice like a closing door. ~They always ask the bright one.~";
       ],
       before [;
         Enter: <<Go u_obj>>;
       ],
  has animate scenery;

Object robot "service robot" LiftLobby
  with name 'robot' 'service' 'servicer' 'android',
       description "A scuffed, slump-shouldered service robot in the corner, with its head against the wall and a diode or two visibly misbehaving. A plate on its chest says SIRIUS CYBERNETICS CORPORATION: YOUR PLASTIC PAL WHO'S FUN TO BE WITH.",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('marvin', 'depressed', 'brain'))
               "~Not Marvin,~ says the robot, without turning. ~People do ask. He's a cousin. His brain's the size of a planet. Mine's the size of a lift button.~";
           if (TopicAny('lift', 'lifts', 'cheerful', 'sullen', 'up', 'floor'))
               "~They don't like each other,~ says the robot. ~The bright one is insufferable. The grey one resents it. If you want either to cooperate, be kind to one of them. I have no advice for what to do about the other.~";
           "~Life,~ says the robot, ~don't talk to me about life.~";
       ],
  has animate scenery;

Object directory "building directory" LiftLobby
  with name 'directory' 'floors' 'list',
       description "SIXTY FLOORS, SIXTY IDENTICAL LISTINGS. Floor 12 is EDITORIAL. Floor 40 is STRATEGY. Floor 59 is something called INTEGRATION. Floor 60 is LEFT BLANK.",
  has scenery;

Object Editorial "The Editorial Floor"
  with description
         "A huge open-plan floor of grey desks, each with a small plant that has been told not to grow. A coffee machine mutters in a corner. A narrow stair at the north-east corner runs up, guarded by a large, thoughtful Vogon. The boardroom is north, an archive west, and a corner office east, its door ajar. A bright lift hums to the south.",
       s_to LiftLobby, w_to Archive, e_to FordOffice, n_to Boardroom,
       u_to [;
         if (orlo_moved == 0)
             "Orlo steps in front of the stairs. ~Poetry,~ he says, in a voice like wet cement. ~Give me poetry, or give me the other thing, which is also poetry.~";
         print "Orlo wipes his eyes and waves you up the stairs. ~Go on,~ he says, thickly. ~Go on. You've given me something.~^";
         return ServerFloor;
       ],
  has light;

Object coffee "coffee machine" Editorial
  with name 'coffee' 'machine',
       description "A tall coffee machine with a complaint button and a sign: I AM AWARE OF THE COFFEE I PRODUCE.",
       before [;
         Take: "It is bolted to the floor, and to its grievances.";
       ],
  has scenery;

Object desks "grey desks" Editorial
  with name 'desks' 'desk' 'plants' 'plant' 'editors',
       description "Rows of grey desks, at which a number of ordinary people are being edited. They do not look up.",
  has scenery pluralname;

Object Zendra "Zendra" Editorial
  with name 'zendra' 'woman' 'editor' 'colleague',
       description "A sharp, wry woman in a grey cardigan, with ink on her cuff and the look of someone who has not been sleeping and is rather enjoying it.",
       initial "Zendra, who sits at the desk nearest the lift, looks up with something like relief. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('mk', 'ii', 'guide', 'cube', 'new', 'edition')) {
               Award(A_ZEN1, 3);
               "~It isn't right,~ says Zendra, lowering her voice. ~The Mk II. It doesn't hold entries. It holds readers. Every edit we make goes into it, and then it makes its own: not about things, about people. I've seen it write a paragraph about a man who hadn't arrived yet.~";
           }
           if (TopicAny('infinidim', 'owners', 'vogon', 'vogons', 'company', 'takeover')) {
               Award(A_ZEN2, 3);
               "~Six names, one address,~ says Zendra. ~The new management are very polite. They use the word synergy as if they hope you won't ask. The editor-in-chief doesn't have a name that anyone's been allowed to know.~";
           }
           if (TopicAny('ford', 'prefect')) {
               Award(A_ZEN3, 2);
               "~Ford,~ says Zendra, with a half-smile. ~The only man to get a desk after being sacked. His desk is at the end. He used to set the combination to the lengths of the Earth entry, you know. Every draft. He was fond of it. It's all in the archive.~";
           }
           if (TopicAny('desk', 'combination', 'lock', 'drawer', 'code'))
               "~Ford's desk?~ says Zendra. ~The lock takes three numbers. They were the word counts of his Earth entry, in its three drafts. The drafts are in the archive, if you've the patience.~";
           if (TopicAny('orlo', 'poetry', 'stair', 'stairs', 'server', 'servers'))
               "~Orlo guards the stairs up to the servers,~ says Zendra. ~He writes poetry at night, and it's very, very bad, and he loves it more than life. Be warned: if you tell him it's good, he'll know you're lying.~";
           if (TopicAny('random', 'girl', 'daughter'))
               "Zendra's face changes. ~There was a girl,~ she says. ~Young, tall, furious. The Mk II talked about her for a week before she turned up. When she did, it wasn't a surprise to anyone.~";
           "Zendra shakes her head. ~I can't talk about that. Not here.~";
       ],
  has animate female proper static;

Object Orlo "Orlo" Editorial
  with name 'orlo' 'vogon' 'guard' 'poet',
       description "A huge Vogon with the profile of an upturned bulldozer and a notebook in one hand, held with remarkable delicacy.",
       initial "Orlo stands at the foot of the north-east stairs, a notebook tucked under one arm like a small, vulnerable animal. ",
       life [;
         Ask, Answer, Tell:
           if (TopicAny('poem', 'poetry', 'verse', 'notebook', 'write', 'writing', 'compose'))
               "Orlo's heavy face is suddenly full of light. ~Poetry,~ he says. ~It must be terrible. Give me the worst you have. Then I will know it is sincere.~ (You can COMPOSE a poem for him.)";
           if (TopicAny('stairs', 'server', 'servers', 'core', 'up'))
               "~Servers are for authorised persons,~ says Orlo. ~Or persons who bring poetry.~";
           "~Verse,~ says Orlo, wistfully, ~is the only honest bureaucracy.~";
       ],
  has animate male proper static;

Object FordOffice "Ford's Old Office"
  with description
         "A cramped corner office with a window onto the plaza, still bearing the faint signs of a man who used it to avoid working. Torn proofs hang on the wall; a battered desk with a dial lock on its top drawer stands under the window. The editorial floor is west.",
       w_to Editorial,
  has light;

Object fdesk "Ford's desk" FordOffice
  with name 'desk' 'drawer' 'top' 'dial' 'lock',
       description [;
         print "A battered desk, scarred with a hundred cigarette burns and one pencilled word, ALIBI. The top drawer has a three-dial lock";
         if (self has open) print ", and it is open";
         print ".";
         new_line;
         rtrue;
       ],
       before [;
         Open: if (self hasnt open && desk_open == 0) "The drawer is locked by a three-dial combination. DIAL three numbers.";
         Unlock: "It is a combination lock. You DIAL it.";
       ],
  has static container openable scenery proper;

Object proofs "torn proofs" FordOffice
  with name 'proofs' 'proof' 'sheets' 'paper' 'wall',
       description [;
         "Proof sheets pinned to the wall, with Ford's pencilled comments in the margins. On the one headed EARTH, someone has written HARMLESS and a different hand has written MOSTLY, and underneath, in a looping scrawl: ~Needs more me. Z.~ You have a good guess whose.";
       ],
  has scenery pluralname;

Object window_ob "window" FordOffice
  with name 'window' 'glass',
       description "The plaza lies below, tiny and tidy. A Vogon in the smoking area has just finished a poem, and is in tears.",
  has scenery;

Object Archive "The Archive"
  with description
         "A long, silent room of grey steel stacks, a ladder on rails, and an overhead light that clicks on when you enter and then waits to see what you'll do. There is a locked cabinet drawer at the far end, and a faint smell of old ink. The editorial floor is east.",
       e_to Editorial,
  has light;

Object stacks "steel stacks" Archive
  with name 'stacks' 'stack' 'shelves' 'files' 'archive',
       searched 0,
       description "Row upon row of boxed files. A label near the centre reads EARTH, HARMLESS: ARCHIVED, with an arrow.",
       before [;
         Search, Take, Open:
           if (self.searched) "You have already found what you need here.";
           self.searched = 1;
           Award(A_STACK, 4);
           move revs to Archive;
           move ledger to Archive;
           "You follow the arrow and find, between OUTDATED and OBSOLETE, a slim file marked EARTH: REVISION HISTORY. Next to it is a heavy ledger labelled VOGON POETRY, VOL. 9 (which nobody has ever wanted to borrow).";
       ],
  has static scenery pluralname;

Object revs "revision file"
  with name 'revision' 'file' 'history' 'earth',
       description [;
         "The file holds three drafts of one entry. DRAFT ONE, FORD: ~Harmless.~ DRAFT TWO, FORD: ~Mostly harmless.~ DRAFT THREE, FORD, in the margin of a memo: ~Mostly harmless. Bring a towel.~ Each has been stamped REJECTED, then ACCEPTED, then RE-ACCEPTED. The drafts, you notice, run to one word, two words and five.";
       ],
       before [;
         Take: "You would rather read it where it is; the light is on a timer.";
       ];

Object ledger "heavy ledger"
  with name 'ledger' 'book' 'poetry' 'volume',
       opened 0,
       description "A heavy, dusty ledger. VOGON POETRY, VOL. 9. Nobody has read it, and several people have tried.",
       before [;
         Open, Search:
           if (self.opened) "You have already looked inside.";
           self.opened = 1;
           move key to Archive;
           "You open the ledger, steeling yourself, and a small brass key slides out from between the verses and clatters to the floor. Whoever hid it had judged correctly that nobody would ever look here.";
       ];

Object key "small brass key"
  with name 'brass' 'key' 'small',
       description "A small brass key, warm from the ledger's heat, if a ledger of poetry can be said to have heat.";

Object drawer "cabinet drawer" Archive
  with name 'drawer' 'cabinet' 'locked',
       with_key key,
       description "A deep steel drawer set into the end of the stacks. It has a keyhole, and the kind of lock that has never had to prove itself.",
       after [;
         Open: Award(A_DRAWER, 3);
       ],
  has static container openable lockable locked scenery;

Object memo "Infinidim memorandum" drawer
  with name 'memo' 'memorandum' 'infinidim' 'paper',
       description [;
         Award(A_MEMO, 5);
         "MEMORANDUM. FROM: INFINIDIM ENTERPRISES (ONE OF SIX). SUBJECT: CONSOLIDATION OF EARTH VARIANTS. The management has identified four hundred and eleven variants of the planet Earth, to be removed at a single stroke in the interest of administrative simplicity. The Mk II will draw all variants together onto a single afternoon, in a single room, on a single Earth. The Grebulon fleet has been engaged to execute. The coordinates of the room are held in the Mk II core. NOTE: THIS MEMORANDUM SHOULD NOT EXIST.";
       ];

Object Boardroom "The Boardroom"
  with description
         "A long, pale room with a long, pale table and eleven chairs of exactly the same pale. A vent in the ceiling hums. The editorial floor is south.",
       s_to Editorial,
  has light;

Object btable "long table" Boardroom
  with name 'table' 'chairs' 'long',
       description "A table the colour of the inside of a filing cabinet, with folders on it in a very straight line.",
  has scenery;

Object vogon_eds "Vogon editors" Boardroom
  with name 'vogons' 'vogon' 'editors' 'editor' 'chief' 'board',
       description "Eleven Vogons in the same dark suit sit round the table, each with a notebook, each with a face like an apology that has gone wrong. At the head sits the Chief Editor, whose name nobody has been allowed to learn.",
       life [;
         Ask, Answer, Tell:
           "The Vogons do not respond to you. They are very much busy not responding.";
       ],
  has animate scenery pluralname;

Object ServerFloor "The Server Floor"
  with description
         "A cold hall of humming racks, lit in the green of a dozen small indicator lights that blink in unison, like something breathing. At the far end stands the core of the Mk II: a ribbed, waist-high pillar with a single mouth-like slot. The stairs go down, and a service ladder runs up to the roof.",
       d_to Editorial, u_to Roof,
  has light;

Object racks2 "humming racks" ServerFloor
  with name 'racks' 'rack' 'servers' 'indicator' 'lights',
       description "Long rows of servers, humming in a key of faint, patient satisfaction.",
  has scenery pluralname;

Object core "Mk II core" ServerFloor
  with name 'core' 'mk' 'ii' 'pillar' 'slot' 'cube',
       description "A ribbed pillar in grey, with a slot at about the height of your face. It is warm, like the cube in your pocket, and gives the impression of having been listening to the cube.",
       life [;
         Ask, Answer, Tell:
           if ((TopicAny('worst', 'dread', 'fear', 'terrible', 'truth', 'honest', 'honestly') || TopicAny('afraid', 'wrong', 'bad'))) {
               if (have_coords) "~I have told you,~ says the core, pleasantly. ~I am sorry it was true.~";
               have_coords = 1;
               Award(A_CORE, 15);
               "A long pause. The indicator lights stop blinking, for the first time since you arrived. ~The worst thing I could tell you,~ says the core, in the cube's voice, but without any of its sweetness, ~is that every Earth will be drawn into a single room, in London, on a single Thursday, and will end there. The room is a pub. The coordinates are held in me, and I will give you them, because you have asked the one question I cannot answer pleasantly.~ The slot glows, and a sheet of green paper slides out, bearing a map reference. The girl is already there; she was always going to be. You take the sheet.";
           }
           if (have_coords) "~You have what you came for,~ says the core, smoothly. ~Please go away.~";
           if (TopicAny('random', 'girl', 'daughter'))
               "~Random is safe and well and needs nothing from you,~ says the core. ~You may stop worrying.~ It sounds so warm, and so certain, that you nearly do.";
           if (TopicAny('earth', 'london', 'vogons', 'plan'))
               "~Everything is in hand,~ says the core, soothingly. ~You have done very well. You are exactly where you need to be.~";
           "~You are quite right,~ says the core, to whatever you said. ~You are always quite right.~ You have the uneasy feeling that this is all it knows how to do, and that you would not like it to do the other thing.";
       ],
  has animate static;

Object Roof "The Roof"
  with description
         "A flat, windy roof with a landing pad painted in pale grey. The whole of Ursa Minor lies below, a patchwork of rooftops under a sky the colour of old paper. Ford's escape ship waits on the pad, engines ticking. The ladder to the server floor is below.",
       d_to ServerFloor,
  has light;

Object roof_ship "Ford's escape ship" Roof
  with name 'ship' 'escape' 'craft' 'shuttle',
       description "A small, dented ship with a faded sticker on the window: NOT A TAXI.",
       before [;
         Enter:
           if (have_coords == 0) "~We don't know where we're going,~ says Ford. ~Unless you've got what the Mk II keeps.~";
           ActThreeEnd();
           rtrue;
       ],
  has static enterable scenery;

! ========================================================================
!  Verbs and routines for Act III
! ========================================================================

[ TypeTextSub w i;
    if (location ~= Lobby) "There's nothing here to type on.";
    if (badge in player || badge has worn) "The terminal has already given you what it can.";
    wn = consult_from;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 'audit' or 'inspection' or 'compliance' or 'review' or 'routine'
                or 'maintenance' or 'paperwork' or 'assessment' or 'survey' or 'inventory'
                or 'delivery' or 'liaison' or 'consultation') {
            move badge to player;
            Award(A_BADGE, 8);
            "The terminal considers this for a moment and prints a white plastic badge. ~PURPOSE ACCEPTED,~ it says. ~YOU DO NOT HAVE TO ENJOY YOUR VISIT.~";
        }
        if (w == 'rescue' or 'save' or 'stop' or 'truth' or 'friend' or 'friends' or 'random' or 'sabotage' or 'destroy' or 'help')
            "The terminal flickers. ~PURPOSE NOT RECOGNISED,~ it says. ~PLEASE RESTATE IN ADMINISTRATIVE TERMS.~";
    }
    "The terminal waits, unamused. ~PLEASE RESTATE,~ it says, ~IN TERMS OF A ROUTINE ADMINISTRATIVE PURPOSE.~";
];

[ ComplimentSub;
    if (location ~= LiftLobby) "There's nobody here who would care.";
    if (noun == cheer_lift) {
        if (lift_ok) "The bright lift glows with all the praise it has been given. It does not want more.";
        lift_ok = 1;
        Award(A_LIFT, 10);
        "~Oh!~ says the bright lift, quivering. ~Oh, that's lovely. Nobody says that. I think I'll go all the way up, just for you!~ Its doors beam open. The grey lift sighs, once, like a damp towel.";
    }
    if (noun == sullen_lift)
        "The grey lift says nothing, which is very much how it says everything. It does not seem to take compliments well.";
    "That is not something that would care for the compliment.";
];

[ ApologiseSub;
    if (noun == Daughter) return ApologiseRandom();
    if (location ~= LiftLobby) "There's nobody here to apologise to.";
    if (noun == sullen_lift) {
        if (lift_ok) "The grey lift grunts. Enough has been said.";
        lift_ok = 1;
        Award(A_LIFT, 10);
        "~Hmm,~ says the grey lift, after a long time. ~Yes. The bright one is a lot. Thank you for noticing.~ Its doors open with a groan, as if getting up from a very long sit.";
    }
    if (noun == cheer_lift) "The bright lift laughs in a bright, tinkling way. ~You've nothing to be sorry about!~ it says.";
    "That is not something that would accept an apology.";
];

[ DialSub n1 n2 n3 i k v;
    if (location ~= FordOffice) "There's nothing here to dial.";
    n1 = -1000; n2 = -1000; n3 = -1000; k = 0;
    for (i = 0 : i < consult_words : i++) {
        v = TryNumber(consult_from + i);
        if (v ~= -1000) {
            k++;
            if (k == 1) n1 = v;
            if (k == 2) n2 = v;
            if (k == 3) n3 = v;
        }
    }
    if (k < 3) "The lock has three dials. Try DIAL and three numbers, for example DIAL 4 5 6.";
    if (fdesk has open) "The drawer is open already.";
    if (n1 == 1 && n2 == 2 && n3 == 5) {
        give fdesk open;
        desk_open = 1;
        Award(A_DESK, 7);
        move oldpass to fdesk;
        "The dials click. The top drawer slides open with a sigh of old tobacco and the faint smell of other people's coffee. Inside are Ford's old pass, and a thin folder of proofs.";
    }
    "The dials click, and nothing else happens. A wrong guess, then.";
];

[ ComposeSub;
    if (location ~= Editorial) "There is nobody here to compose for.";
    if (orlo_moved) "Orlo is quite satisfied for now.";
    poem_shown = 1;
    print "You gather your thoughts, which resist. Orlo waits, notebook open. You will need a line from each list; choose one number from each, and say POEM then the three numbers (for example POEM 2 3 1).^^";
    print "FIRST LINE:^  1. O sky, you are so very blue.^  2. The gelatinous dawn gropes at my window.^  3. My heart, a damp filing cabinet.^^";
    print "SECOND LINE:^  1. and I, a slug in the rain of fate,^  2. a squeak in the vacuum of regret,^  3. a pleasant fellow in a hat,^^";
    print "THIRD LINE:^  1. in the end, we are all very small.^  2. shall be filed, and then re-filed, and burn.^  3. SQUELCH, says the universe, and so, adieu!^";
    rtrue;
];

[ PoemActSub a b c i k v score_;
    if (location ~= Editorial) "There is nobody here to read a poem to.";
    if (orlo_moved) "Orlo is quite satisfied for now.";
    if (poem_shown == 0) "First COMPOSE a poem, to see what you have to work with.";
    k = 0; a = 0; b = 0; c = 0;
    for (i = 0 : i < consult_words : i++) {
        v = TryNumber(consult_from + i);
        if (v ~= -1000) {
            k++;
            if (k == 1) a = v;
            if (k == 2) b = v;
            if (k == 3) c = v;
        }
    }
    if (k < 3 || a < 1 || a > 3 || b < 1 || b > 3 || c < 1 || c > 3)
        "Choose three lines, one from each list: for example POEM 2 3 1.";
    ! ugliness of each choice
    score_ = 0;
    switch (a) { 1: score_ = score_ + 1; 2: score_ = score_ + 3; 3: score_ = score_ + 2; }
    switch (b) { 1: score_ = score_ + 2; 2: score_ = score_ + 3; 3: score_ = score_ + 1; }
    switch (c) { 1: score_ = score_ + 1; 2: score_ = score_ + 2; 3: score_ = score_ + 3; }
    v = 7;
    if (difficulty == DIFF_EASY) v = 5;
    if (score_ >= v) {
        orlo_moved = 1;
        Award(A_POEM, 15);
        "You recite it, with all the feeling you can summon, which turns out to be sufficient. For a moment Orlo is quite still. Then a single tear, the size of a thumb, runs down his face and splashes on his notebook. ~It is,~ he says, thickly, ~the most terrible thing I have ever heard.~ He blows his nose in a sound like a ship going down. ~Go up. Go on. I will not stop you.~";
    }
    "You recite it. Orlo listens with his head to one side. ~It is not bad,~ he says. ~That is the trouble. Bad is the whole point. Try something worse.~";
];

! per-turn events in Act III
[ Act3Tick;
    if (location == Boardroom && heard_meeting == 0) {
        if (towel hasnt worn) {
            print "^A Vogon at the head of the table looks up. ~Unscheduled visitor,~ it says, quite neutrally, and two of the others rise and escort you out into the editorial floor with the practised gentleness of people who have had a great deal of practice. ~Perhaps,~ says one, ~if you were less visible.~^";
            PlayerTo(Editorial);
            return;
        }
        meet_t++;
        if (meet_t == 1)
            print "^You stand against the wall with the towel over your head. The Vogons look briefly at the place where you are, and then past it. This, you realise, is the oldest trick there is, and also the worst, and it works.^";
        if (meet_t == 2)
            print "^~Item one,~ says the Chief Editor. ~The variants. We have identified four hundred and eleven. The Mk II reports that it has made contact with the daughter. She is, apparently, very promising.~^";
        if (meet_t == 3)
            print "^~Item two. The date. It is a Thursday, in London, on the Earth the girl has been drawn to. Every other Earth is to be drawn to the same afternoon, in the same room, by a method the Mk II describes as hospitality.~ A pause. ~Item three. The Grebulons.~^";
        if (meet_t == 4)
            print "^~The Grebulons,~ says the Chief Editor, ~have been engaged. They have a chart. They have forgotten what it is for, which is useful. They will press the button when the room is full.~ A rustle of agreement, like paper in a drawer.^";
        if (meet_t == 5) {
            heard_meeting = 1;
            Award(A_MEET, 12);
            print "^~Adjourned,~ says the Chief Editor. The Vogons file out, in order of seniority, and the room empties. You peel the towel off your head and breathe. Ford, beside you with his own towel round his ears, looks at you with an expression you have rarely seen on him: fear, and below it, determination.^";
        }
    }
];

[ ActThreeEnd;
    ! make sure the fish cannot be left behind
    if (fish notin player && jar notin player && ~~IndirectlyContains(player, fish)) {
        move jar to player;
        move fish to jar;
        print "^At the last moment a small, neat figure comes pounding up the ladder with your jar in both hands: Pim, from the cloakroom. ~Your property!~ he pants, and hands it over with a little bow. ~Safe with us, as promised.~^";
    }
    Award(A_ROOF, 5);
    print "^You scramble aboard. Ford closes the door, takes the controls with a flourish that is mostly decoration, and the roof tilts away beneath you as Ursa Minor falls into the sky. ~Hold on to your towel,~ he says. ~We have a Thursday to catch.~^^";
    StartAct4();
];

[ StartAct3;
    act = 3;
    pt = 0;
    ford_follows = 1;
    move Ford to Plaza;
    PlayerTo(Plaza);
];
