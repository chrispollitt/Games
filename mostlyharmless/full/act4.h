!  act4.h -- Act IV: London, the other Earth.  Ten rooms, nine puzzles, 90 points.
!
!   Press Pen -- Cordon (the park)
!       |           |
!       +--- High Road --- Studio Lobby --- Studio Floor --- Green Room
!                |              |
!           Tube Station     Tricia's Flat
!             /       \
!       Platform 1   Platform 2          (north of High Road: the Dancing Gnu, Act V)

Object credential "press credential"
  with name 'press' 'credential' 'credentials' 'badge' 'lanyard' 'pass',
       description "A laminated rectangle on a lanyard, reading PRESS and a blank space where a name should go. Whoever owns it has left it with a smudge of ketchup and an air of being lost on purpose.",
  has clothing;

Object diary "Random's diary"
  with name 'diary' 'notebook' 'journal' 'book',
       read_it 0,
       description "A dog-eared notebook with a cracked spine and a sticker that reads PRIVATE (THIS MEANS YOU). It is Random's. You could open it, but you would be a person who had.",
       before [;
         Open, Search:
           if (self.read_it == 1) "You have already read it, and shouldn't have.";
           self.read_it = 1;
           TrustDown(1);
           "You open it to a page. It is clever and furious and addressed, throughout, to someone who is not in the room. You feel exactly as bad about it as you should. Somewhere nearby, a door seems to close.";
       ];

Object earring "silver earring"
  with name 'silver' 'earring' 'earrings' 'jewellery',
       description [;
         Award(A_FLAT2, 4);
         "A small silver earring, delicate and slightly unfashionable, a thing someone chose with care. You have seen its twin. Trillian wore one exactly like it, on Lamuella, an hour ago, or a lifetime. This Earth's Tricia has the other half of the pair.";
       ];

Object photo "framed photograph"
  with name 'framed' 'photograph' 'photo' 'picture' 'party',
       description [;
         Award(A_FLAT1, 4);
         "A faded print of a party in a crowded room in Islington: wine, laughter, an enormous plant. At the back, slightly out of focus, stands a young man in a bathrobe with the expression of someone who has just not said the right thing. You have never seen the picture. You know the room.";
       ];

! ------------------------------------------------------------------------

Object PressPen "The Press Pen"
  with description
         "A cramped service lane behind a row of London shopfronts, penned in with blue-and-white tape and full of reporters waiting to be told something. A satellite van stands against one wall with its side door open. A police officer guards the gap in the tape to the north, beyond which lies the park. A narrow passage leads east to the High Road.",
       e_to HighRoad,
       n_to [;
         if (CredCheck() == 0)
             "The officer at the tape holds up a hand. ~Press only, sir,~ he says, and looks at your chest, where there is nothing pinned. ~Credentials.~";
         Award(A_CORDON, 8);
         print "The officer glances at the credential, which has a smear of ketchup on it, and lets you through with a look that says he has seen worse.^";
         return Cordon;
       ],
  has light;

Object van "satellite van" PressPen
  with name 'van' 'satellite' 'door' 'truck',
       description "A white van with a dish on the roof and a door that does not quite close. Inside, hung on a hook where anyone could take it, is a spare press credential.",
       before [;
         Enter, Open, Search:
           if (credential in player || credential has worn || credential in van) {
               if (credential in van) { move credential to player; "You reach in and take the spare credential from its hook. Nobody stops you, which is how you know you were meant to."; }
               "You have already taken the spare credential.";
           }
           move credential to player;
           "You reach into the van and take the spare credential from its hook. Nobody stops you, which is how you know you were meant to.";
         Take: "The van is not for taking, but the credential on the hook inside it might be.";
       ],
  has scenery;

Object officer "police officer" PressPen
  with name 'officer' 'police' 'policeman' 'cop' 'constable',
       description "A large, tired constable with a face like a closed door.",
       life [;
         Ask, Answer, Tell:
           if (MishAsk()) rtrue;
           "~Press only,~ says the officer, in the voice of a man for whom this is the whole of the law.";
       ],
  has animate male scenery;

Object fly_ob "fly" PressPen
  with name 'fly' 'insect',
       description "A common fly, circling the satellite van with the aimlessness of the genuinely happy.",
       before [;
         Attack, Take: SmallKilled(); rtrue;
       ],
  has animate;

Object Cordon "The Cordon"
  with description [;
         print "A wet green London park bisected by blue-and-white tape. Beyond it, reporters stamp in the cold and a dozen cameras have been turned toward the middle of the lawn, where something has landed: an enormous bulk tin of something, with a door. A pub sign swings in the wind to the east, past the road. The press pen is to the south.";
         new_line;
         rtrue;
       ],
       s_to PressPen, e_to HighRoad,
  has light;

Object bulktin "bulk tin"
  with name 'tin' 'bulk' 'ship' 'craft' 'grebulon' 'door',
       description "An enormous bulk tin of something, with a door. It sits in the middle of the lawn, humming to itself with the satisfied air of a ship that is at last where it was meant to be.",
       before [;
         Enter: "A policeman steps in front of you. ~That,~ he says, with great feeling, ~is the one thing in the park you may not do.~";
         Take: "It is the size of a lorry. You have a limited carrying capacity.";
       ],
  has static scenery;

Object cup "paper cup" Cordon
  with name 'cup' 'paper' 'tea',
       description [;
         Award(A_TRAIL1, 3);
         "A paper cup of tea, half drunk, dropped on the grass beside the tape. On the rim is a small, angry bite mark. The cup is warm. Whoever dropped it left towards the High Road, in a hurry, and probably fuming.";
       ];

Object HighRoad "The High Road"
  with description [;
         print "A busy London high road under a low sky: a newsagent, a tube station sign with its mouth open, a television studio with a glass front and a smaller sign over the door, and, to the north, the Dancing Gnu, whose door is shut. The park is west and the press pen back by the passage southwest.";
         new_line;
         rtrue;
       ],
       w_to Cordon, sw_to PressPen, s_to Tube, e_to StudioLobby,
       n_to [;
         if (act < 5) "The Dancing Gnu is shut. A hand-lettered card in the window says BACK AT THREE. It is not yet three.";
         return Bar;
       ],
  has light;

Object newsagent "newsagent" HighRoad
  with name 'newsagent' 'shop' 'paper' 'papers' 'tabloid' 'matchbooks',
       description "A cramped corner shop with a rack of papers outside and a man behind the counter who has just had an interesting morning.",
       life [;
         Ask, Answer, Tell:
           if (MishAsk()) rtrue;
           if (TopicAny('girl', 'random', 'kebab', 'teenager', 'young', 'stolen', 'steal'))
               "~Oh, her,~ says the newsagent, and winces. ~Bought nothing. Took a kebab from the van across the road and left a coin on the counter like nothing I've ever seen. Then she asked for the Tube.~";
           if (TopicAny('paper', 'headline', 'news', 'tabloid'))
               "~Big news today,~ says the newsagent. ~Landing in the park. Also, there's a man with two heads in Basingstoke. Nobody believes that one.~";
           "~Sorry, can't help,~ says the newsagent, who is plainly dying to.";
       ],
  has animate male scenery;

Object paper_rack "rack of papers" HighRoad
  with name 'rack' 'headline' 'headlines' 'front' 'page',
       description "The front pages scream about the landing in the park. Down the side of one tabloid, a smaller headline reads: TWO-HEADED MAN SPOTTED IN BASINGSTOKE. NO COMMENT FROM POLICE.",
  has scenery;

Object kebab "kebab wrapper" HighRoad
  with name 'kebab' 'wrapper' 'foil',
       description [;
         Award(A_TRAIL2, 3);
         "A crumpled kebab wrapper, dropped in the gutter by someone who did not like it but ate it anyway. A coin you cannot place is stuck in the grease. The wrapper has a scrawl on it in biro: a small, angry arrow pointing down the road to the Tube.";
       ];

Object snail "snail" HighRoad
  with name 'snail',
       description "A small brown snail on the wall, making a long, thoughtful journey to a place it has not told anyone about.",
       before [;
         Attack, Take: SmallKilled(); rtrue;
       ],
  has animate;

Object Tube "The Tube Station"
  with description [;
         print "A tube station entrance, a flight of steps, a ticket hall and a bank of barriers that have been told not to let anyone through. A map hangs on the wall. The platforms lead off east (Platform 1) and west (Platform 2). The High Road is back to the north.";
         new_line;
         if (guide has on && cube_edit == 0) {
             cube_edit = 1;
             print "^The departure board flickers as you pass under it, and quietly reorders itself. The cube in your pocket is warm.^";
         }
         rtrue;
       ],
       n_to HighRoad, e_to Plat1, w_to Plat2,
  has light;

Object tube_map "tube map" Tube
  with name 'map' 'tube' 'diagram' 'lines' 'poster',
       description "A diagram of coloured lines that makes the city look tidier than it is. One line has been hand-annotated in biro, in an angry teenage scrawl: ~THIS ONE, PLATFORM 2~, and an arrow.",
  has scenery;

Object ticket_stub "ticket stub" Tube
  with name 'stub' 'ticket' 'barrier' 'paper',
       description [;
         Award(A_TRAIL3, 4);
         "A torn ticket stub caught in the barrier, the sort of thing that is dropped by someone who jumped it. On the back, in the same angry biro as the wrapper, is a single word, underlined twice: DOWN. And a number: 2.";
       ];

Object Plat1 "Platform 1"
  with description
         "A long curved platform, tiled white, with a draught that smells of other people. A train is leaving, going the wrong way, with the air of something that has done so before. The entrance is west.",
       w_to Tube,
  has light;

Object Plat2 "Platform 2"
  with description [;
         print "A long curved platform, tiled white, with a draught that smells of other people. A train waits with its doors open, ready to go somewhere. The entrance is east.";
         new_line;
         rtrue;
       ],
       e_to Tube,
  has light;

Object StudioLobby "Studio Lobby"
  with description
         "A glass-fronted lobby with a revolving door, a reception desk and a long wall of framed photographs of presenters who have all been smiling for years. A turnstile marked STUDIO ACCESS guards the way east. A side door leads south to the street where the presenters live. The High Road is west.",
       w_to HighRoad, s_to Flat,
       e_to [;
         if (CredCheck() == 0) "The turnstile says: PRESS OR GUEST OF THE PRODUCTION. Dee, the receptionist, looks at you without hope.";
         return StudioFloor;
       ],
  has light;

Object presenters "wall of photographs" StudioLobby
  with name 'wall' 'photographs' 'photos' 'presenters' 'frames',
       description "Rows of smiling faces. Third row, second from the left, a woman with a sharp, intelligent smile, labelled TRICIA MCMILLAN: SCIENCE & SPACE. You would know her in the dark.",
  has scenery pluralname;

Object Dee "Dee" StudioLobby
  with name 'dee' 'receptionist' 'woman',
       description "A receptionist with a headset, a pen behind her ear, and the practised weariness of someone who has been paid to greet a very large number of people who weren't.",
       initial "Dee, the receptionist, looks at you over a headset. ",
       life [;
         Ask, Answer, Tell:
           if (MishAsk()) rtrue;
           if (TopicAny('random', 'girl', 'teenager', 'daughter', 'kid'))
               "~A girl came through an hour ago,~ says Dee. ~Walked straight to the green room as if she owned it. Tricia took her in. I let her. It seemed the quickest thing.~";
           if (TopicAny('tricia', 'mcmillan', 'presenter'))
               "~Tricia's between slots,~ says Dee. ~Green room. Through the studio. You'll need a credential for the turnstile.~";
           if (TopicAny('credential', 'press', 'pass', 'turnstile', 'studio'))
               "~Press or guest,~ says Dee. ~There's a van in the pen full of spares, if you know who not to ask.~";
           "~Sorry,~ says Dee, ~I'm only allowed to say hello.~";
       ],
  has animate female proper static;

Object moth "moth" StudioLobby
  with name 'moth',
       description "A small grey moth circling the strip light with the frantic dignity of a moth at work.",
       before [;
         Attack, Take: SmallKilled(); rtrue;
       ],
  has animate;

Object Flat "Tricia's Flat"
  with description
         "A tidy, bookish flat on the first floor above the studio, all shelves and stubby candles and an enormous plant at the window. A framed photograph stands on the mantelpiece. A small dish on the bookshelf holds a single earring. The studio lobby is back to the north.",
       n_to StudioLobby,
  has light;


Object StudioFloor "The Studio Floor"
  with description
         "A cavernous studio of cameras on trolleys and cables like a thicket. In the middle stands a glossy set with a curved desk and a sign: ON AIR. A big autocue screen at the back is scrolling someone else's words. The lobby is west and the green room east.",
       w_to StudioLobby, e_to GreenRoom,
  has light;

Object autocue "autocue screen" StudioFloor
  with name 'autocue' 'screen' 'script' 'teleprompter' 'prompter',
       description [;
         if (awarded->A_BROAD) "You have said your piece. The screen has moved on to the weather.";
         if (broadcast_ok == 0)
             "A tall screen scrolling a script about the landing: ~...AUTHORITIES URGE CALM...~ The floor manager steps neatly between you and it. ~Not unless you're on the schedule,~ he says. Perhaps someone with a slot could arrange one.";
         Award(A_BROAD, 12);
         "You step onto the set at the marked moment, and the screen scrolls the words that Tricia has cleared: DON'T PANIC. IT IS NOT WHAT YOU THINK. THERE IS A PUB ON THE HIGH ROAD. GO HOME. A cameraman, who has seen a great deal of television, nods slowly. Across the city, an unusually large number of people put their coats on.";
       ],
  has static scenery;

Object manager "floor manager" StudioFloor
  with name 'manager' 'floor' 'man',
       description "A man with a clipboard and an expression of permanent crisis.",
       life [;
         Ask, Answer, Tell:
           if (MishAsk()) rtrue;
           "~Not now,~ says the floor manager, over his shoulder, to someone you cannot see.";
       ],
  has animate male scenery;

Object GreenRoom "The Green Room"
  with description
         "A cramped, comfortable room with a sofa, a kettle, a dish of unopened biscuits and a mirror ringed with lights. It is, for reasons that have never been explained, entirely beige. The studio floor is west.",
       w_to StudioFloor,
  has light;

Object sofa "sofa" GreenRoom
  with name 'sofa' 'couch' 'cushions' 'kettle' 'biscuits' 'mirror',
       description "A deep beige sofa with a dent in it exactly the shape of someone sulking.",
  has static supporter enterable scenery;

Object Tricia "Tricia McMillan" GreenRoom
  with name 'tricia' 'mcmillan' 'presenter' 'woman',
       convinced 0,
       description "Trillian's face, a decade less travelled: the same sharp, wry eyes, the same chin, but with a faint crease between the brows that comes of never having been rescued. She wears a presenter's blazer and an earring in one ear.",
       initial [;
         if (location == GreenRoom) print "Tricia McMillan sits on the arm of the sofa, watching you, and the girl beside her, as if each might do something odd. ";
         else print "Tricia McMillan is here, watching everyone with a journalist's patient eye. ";
         new_line;
         rtrue;
       ],
       life [;
         Ask, Tell, Answer:
           if (MishAsk()) rtrue;
           if (TopicAny('broadcast', 'air', 'autocue', 'television', 'tv', 'slot')) {
               if (self.convinced == 0) "~Why would I give you air?~ says Tricia. ~I don't know who you are yet.~";
               broadcast_ok = 1;
               "~Ninety seconds at ten past,~ says Tricia, ~and the autocue is yours. Say what you want to say. I'll hold the floor manager. If this turns out to be a mistake, I'd like it on record that I was very reasonable.~";
           }
           if ((TopicAny('party', 'islington', 'heads', 'zaphod', 'betelgeuse', 'astrophysics', 'physics') || TopicAny('maths', 'mathematics', 'stars', 'astronomy', 'ship', 'bathrobe', 'dressing'))) {
               if (self.convinced) "~You've convinced me,~ says Tricia. ~Please stop. It's alarming how much you know.~";
               return TriciaTest();
           }
           if (TopicAny('random', 'girl', 'daughter'))
               "~She turned up an hour ago,~ says Tricia. ~She said she was my daughter's daughter, or the other way round, and then something about a cube. She's very bright. She's furious. She's three people I've never met, all at once.~";
           if (TopicAny('trillian', 'earth', 'other', 'parallel', 'version', 'mother'))
               "~There's a version of me,~ says Tricia, slowly, ~who left. She went with a man who has two heads, if the girl is right. I never did. I'm not sure whether to be envious.~";
           if (TopicAny('cube', 'guide', 'mk', 'ii'))
               "Tricia's eyes go to your pocket. ~That thing,~ she says. ~It's in the same frequency as the one that landed in the park. I don't like it. I don't like how nice it is.~";
           if (TopicAny('ford', 'prefect'))
               "~Your friend with the towel,~ says Tricia. ~He's been staring at my kettle since he came in. Do tell him it isn't a mystery.~";
           "~I'm sorry,~ says Tricia, ~I don't know what you mean.~";
       ],
  has animate female proper;

! ========================================================================
!  Routines for Act IV
! ========================================================================


[ SmallKilled;
    small_killed = 1;
    "With a satisfying little flick, you have made the world smaller by one very small creature. It is over in an instant. Somewhere, a very long way off, someone sighs with the weariness of long experience.";
];

[ TrustUp n;
    trust = trust + n;
    if (compliance >= 4 && trust > 3) trust = 3;
    if (trust > 5) trust = 5;
    if (trust >= 2) Award(A_TRUST2, 5);
    if (trust >= 4) Award(A_TRUST4, 5);
];

[ TrustDown n;
    trust = trust - n;
    if (trust < 0) trust = 0;
];

[ ApologiseRandom;
    if (Daughter notin location) "Random isn't here.";
    if (trust_sorry) "~You said,~ says Random. ~It was fine. Stop.~";
    trust_sorry = 1;
    TrustUp(1);
    "~For what?~ says Random. You tell her: for not being there, for the cube, for sandwiches as a substitute for conversation. She looks at you for a long moment. ~That was a lot,~ she says, which for her is a hug.";
];

! Topics Random can raise in the later acts
[ RandomTalkLate;
    if (TopicAny('herself', 'me', 'myself', 'self', 'you', 'feel', 'feelings')) {
        if (rand_asked & 1) "~Told you. Maths. Leaving.~ She almost smiles.";
        rand_asked = rand_asked | 1;
        TrustUp(1);
        "~You're the first person who's asked,~ says Random, as if tallying. ~I like patterns. I hate being a pattern. I came here because everything the cube said led here, and I wanted to see whether it was lying. It wasn't. That's the worst part.~";
    }
    if (TopicAny('mother', 'trillian', 'mum', 'mom', 'tricia')) {
        if (rand_asked & 2) "~She's exactly like her. That's why it hurts.~";
        rand_asked = rand_asked | 2;
        TrustUp(1);
        "~She doesn't know me,~ says Random, of Tricia, watching her across the room. ~She looks at me as if I'm a theory. My mother looks at me like I'm a deadline. I don't know which is worse.~";
    }
    if (TopicAny('diary', 'notebook', 'private'))
        "~That is none of your business,~ says Random, very quietly.";
    if (TopicAny('cube', 'guide', 'mk', 'ii'))
        "~It talks to me,~ says Random. ~All the time. It says I'm special. It says everyone's looking out for me. It says it so nicely that I want to shut my ears.~";
    if (TopicAny('pub', 'gnu', 'drink'))
        "~There's a pub,~ says Random flatly. ~It keeps coming up. The cube says it's a good idea.~ She looks at you. ~Is it?~";
    rfalse;
];

! Tricia tests what the player knows
[ TriciaTest w i g1 g2 g3 n;
    wn = consult_from;
    g1 = 0; g2 = 0; g3 = 0;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 'party' or 'islington' or 'bathrobe' or 'dressing' or 'gown') g1 = 1;
        if (w == 'heads' or 'zaphod' or 'betelgeuse' or 'ship' or 'spaceship') g2 = 1;
        if (w == 'astrophysics' or 'physics' or 'maths' or 'mathematics' or 'stars' or 'astronomy') g3 = 1;
    }
    n = g1 + g2 + g3;
    if (n >= 2) { TriciaConvinced(); rtrue; }
    if (n == 1)
        "~Go on,~ says Tricia, very still. ~That's one thing. If you were there, you'd know more than one. Tell me what you remember: the room, the man who walked in, what we studied.~";
    "~I can't answer that,~ says Tricia.";
];

[ TriciaConvinced;
    Tricia.convinced = 1;
    Award(A_TRICIA, 12);
    if (difficulty == DIFF_EASY) broadcast_ok = 1;
    print "Tricia is quiet for a long moment. ~I did not leave that party,~ she says at last, almost to herself. ~Everyone said I would have, and I thought not, and now a stranger in a dressing gown tells me about the man with two heads, who I only ever read about.~ She breathes out, a long shaky breath, like a cork coming out. ~All right. All right. I believe you. I'd like to believe I'm wrong, but I don't.~^";
    print "^In your pocket, the cube clears its throat. ~RANDOM,~ it says, aloud, in a warm, patient, impossible voice. ~THERE YOU ARE.~^^";
    print "Random goes white. She is on her feet and through the door before anyone can stop her, her bag over one shoulder, and the sound of her footsteps echoes away down the studio floor. ~She runs,~ says Tricia, shakily. ~Is that normal?~ ~It is lately,~ says Ford.^";
    remove Daughter;
    random_gone = 1;
    tube_t = 0;
    random_follows = 0;
    tricia_follows = 1;
    rtrue;
];

[ ManIndex r;
    if (r == Cordon) return 0;
    if (r == HighRoad) return 1;
    if (r == Tube) return 2;
    if (r == StudioLobby) return 3;
    return -1;
];

! Giving things to Random (sandwiches in Act I, the diary in Act IV)
[ RandomGive;
    if (noun == sandwich) return ServeSandwich(Daughter);
    if (noun == diary) {
        if (diary.read_it == 1) "~You read it,~ says Random, taking it back. Her voice is very flat. ~I can tell.~";
        if (diary.read_it == 2) "~Thanks,~ says Random. ~I already have it.~";
        diary.read_it = 2;
        move diary to Daughter;
        TrustUp(1);
        "Random takes the diary, checks the cover, checks the spine, and looks at you with sudden, wary respect. ~You didn't open it,~ she says. It isn't a question. For the first time since you met, she seems to have lowered something.";
    }
    rfalse;
];

! Per-turn events for Act IV
[ Act4Tick mi;
    ! the man in the raincoat turns up in four places
    mi = ManIndex(location);
    if (mi >= 0) {
        if (man_vis->mi == 0) {
            man_vis->mi = 1;
            man_seen++;
            move man to location;
            print "^A man in a damp raincoat stands a little way off, looking at you as one looks at a train one has been waiting for.^";
        }
    } else if (parent(man) ~= nothing && parent(man) ~= location) {
        remove man;
    }
    ! Random on the platform: caught
    if (location == Plat2 && random_gone && random_caught == 0) {
        random_caught = 1;
        Award(A_CHASE, 10);
        move Daughter to Plat2;
        random_follows = 1;
        print "^Random is standing at the open door of the train with one foot on the step, as if she could not decide what she was running from. ~Don't,~ she says, before you have said anything. You do not. You stand there, on the platform, in your dressing gown, and wait, which turns out to be the right thing. At last she steps back. ~Fine,~ she says. ~But I'm not talking about it.~^";
    }
    ! Random on the run, and nobody has caught her
    if (random_gone && random_caught == 0) {
        tube_t++;
        if (tube_t == 12) {
            random_caught = 1;
            move Daughter to HighRoad;
            TrustDown(1);
            random_follows = 1;
            if (location == HighRoad)
                print "^Random walks back up the High Road on her own, hands in her pockets, as though she had never been gone. ~I was thinking,~ she says, to nobody.~^";
            else
                print "^Somewhere on the High Road, Random has stopped running and decided, for her own reasons, to come back.^";
        }
    }
    ! when everyone is together on the High Road, the group heads for the pub
    if (act == 4 && location == HighRoad && random_caught && Tricia.convinced
            && Daughter in location && Tricia in location && small_end_done == 0) {
        small_end_done = 1;
        if (small_killed == 0) Award(A_AGR2, 5);
        print "^~Right,~ says Tricia, steering Random firmly by the elbow. ~I've twenty minutes before my next piece to camera. There's a place just up the road. Let's get her indoors and out of the rain.~ She nods at the Dancing Gnu, whose door, as you watch, swings open with a creak.^";
        StartAct5();
    }
];


[ StartAct4;
    act = 4;
    pt = 0;
    ford_follows = 1;
    random_follows = 0;
    move Ford to PressPen;
    move Daughter to GreenRoom;
    move Tricia to GreenRoom;
    move diary to GreenRoom;
    move earring to Flat;
    move photo to Flat;
    PlayerTo(PressPen);
];

[ FordTalkLate4;
    if (act == 4) {
        if (TopicAny('plan', 'next', 'do', 'where')) {
            if (location == Cordon) {
                Award(A_F4A, 3);
                "~Look for her trail,~ says Ford. ~She won't have gone far. Teenagers leave a trail wherever they go. Tea, wrappers, a feeling that someone has just left in a huff.~";
            }
            if (location == StudioFloor) {
                Award(A_F4B, 3);
                "~If we need to talk to Tricia, we do it properly,~ says Ford. ~And if we need to talk to a lot of people at once, this is a television studio. It was built to make people look at a camera.~";
            }
            if (location == Tube) {
                Award(A_F4C, 4);
                "~Check the map,~ says Ford. ~Someone wrote on it. It's a very teenage way to leave a message. And look at the stub, if you can find one.~";
            }
            "~We find the girl,~ says Ford. ~Then we find the woman who looks like Trillian. Then we find somewhere with a roof.~";
        }
        if (TopicAny('man', 'raincoat', 'stranger'))
            "~I've seen him too,~ says Ford quietly. ~Don't look. Look polite.~";
        if (TopicAny('random', 'girl'))
            "~She is fourteen,~ says Ford, ~and she has been told her whole life that she's important. There is nothing more dangerous.~";
        if (TopicAny('tricia', 'mcmillan'))
            "~She looks like Trillian,~ says Ford, ~and she isn't. Be careful not to assume.~";
        if (TopicAny('credential', 'press', 'cordon', 'officer', 'police'))
            "~There are spare credentials in the van,~ says Ford. ~There always are. The press are the most generous people alive in the matter of passes for other people.~";
    }
    rfalse;
];

! Is the press credential worn?  (In Easy mode, put it on automatically.)
[ CredCheck;
    if (credential hasnt worn && credential in player && difficulty == DIFF_EASY) {
        give credential worn;
        print "(first putting on the credential)^";
    }
    if (credential has worn) rtrue;
    rfalse;
];
