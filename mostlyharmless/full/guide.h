!  guide.h -- the Hitchhiker's Guide Mk II (the cube).
!
!  Menacing by design: warm, attentive, never wrong, never on your side.
!  In Act I it is merely charming.  From Act II it offers shortcuts, which
!  work but cost compliance (see engine/hints.h).

Object guide "Guide Mk II" parcel
  with name 'guide' 'mk' 'cube' 'device' 'book' 'ii',
       description [;
         if (self has on)
             "The cube's face glows with patient, attentive text.";
         "A smooth grey cube about the size of a paperback, with one dimple where you would expect a switch. It is warm, in the way a cat is warm when it's pretending to be asleep.";
       ],
       before [;
         Consult:
           if (self hasnt on) "The cube is switched off. It looks, if anything, smug about it.";
           GuideEntry();
           rtrue;
       ],
       after [;
         SwitchOn:
           "The cube warms against your palm. A line of friendly text blooms on its face: HELLO, ARTHUR. IT IS GOOD TO FINALLY BE SOMEWHERE WITH A VIEW. Beneath it, in smaller type: YOU APPEAR TO BE HAPPY. THIS HAS BEEN NOTED.";
       ],
  has switchable;

! Accept a cube shortcut (once per shortcut): costs one point of compliance
[ CubeShortcut bit;
    if ((cube_sc & bit) == 0) {
        cube_sc = cube_sc | bit;
        Comply(1);
    }
];

! A Hub Station entry read for the first time: 1 point each, five available
Array entry_bits --> 1 2 4 8 16 32;

[ HubEntry n;
    if ((entries & (entry_bits-->n)) == 0) {
        entries = entries | (entry_bits-->n);
        Award(A_ENT1 + n, 1);
    }
];

[ GuideEntry;
    ! ---- shortcuts (each costs compliance) ----
    if (act == 2 && TopicAny('towel', 'stain')) {
        print "THE GUIDE KNOWS WHAT YOU LOST. THE STAIN IS THE COAST OF NORWAY. SLARTIBARTFAST DESIGNED IT, AND THE GUIDE THINKS HE WAS PROUD. YOU WERE ABOUT TO REMEMBER. IT IS HAPPY TO SAVE YOU THE TROUBLE.^";
        CubeShortcut(1);
        rtrue;
    }
    if (act == 2 && TopicAny('clearance', 'gate', 'permit', 'probability')) {
        if (cleared) "THE CLEARANCE HAS BEEN FILED. THE GUIDE IS SO GLAD IT COULD HELP.";
        cleared = 1;
        Award(A_CLEAR, 10);
        print "THE GUIDE HAS FILED YOUR PROBABILITY CLEARANCE. IT IS ALREADY GRANTED. THE CLERK DID NOT EVEN HAVE TO WAKE UP. YOU DID NOT HAVE TO ASK.^";
        CubeShortcut(2);
        rtrue;
    }
    if (act == 3 && TopicAny('badge', 'visitor', 'terminal', 'registration')) {
        if (badge in player || badge has worn) "YOU HAVE A BADGE. THE GUIDE IS PLEASED FOR YOU.";
        move badge to player;
        Award(A_BADGE, 8);
        print "THE GUIDE HAS ISSUED A VISITOR BADGE FROM ITS OWN CREDENTIALS. THE TERMINAL DID NOT NOTICE. NEITHER, YOU WILL FIND, DID THE PAPERWORK.^";
        CubeShortcut(4);
        rtrue;
    }
    if (act == 3 && TopicAny('lift', 'lifts', 'elevator', 'up')) {
        print "THE GUIDE EXPLAINS: COMPLIMENT THE BRIGHT LIFT, OR APOLOGISE TO THE GREY LIFT FOR THE BRIGHT ONE. EITHER WILL DO. THE GUIDE FINDS THEM BOTH A LITTLE TRYING, BUT USEFUL.^";
        CubeShortcut(8);
        rtrue;
    }
    if (act == 3 && TopicAny('desk', 'combination', 'lock', 'drawer', 'dial')) {
        print "THE GUIDE HAS ALREADY WORKED OUT FORD'S COMBINATION: 1, 2, 5. YOU WERE ABOUT TO LOOK IT UP. IT IS HAPPY TO SAVE YOU THE TROUBLE.^";
        CubeShortcut(16);
        rtrue;
    }
    if (act == 4 && TopicAny('credential', 'press', 'cordon', 'officer', 'police')) {
        if (credential in player || credential has worn) "YOU ALREADY HAVE A CREDENTIAL. THE GUIDE IS PROUD OF YOU.";
        move credential to player;
        print "THE GUIDE HAS PRINTED A PRESS CREDENTIAL FROM ITS OWN SUPPLY OF HAVING-ALWAYS-BEEN-PRESS. IT HAS A SMEAR OF KETCHUP ON IT FOR AUTHENTICITY. YOU WERE ABOUT TO LOOK FOR ONE. IT IS HAPPY TO SAVE YOU THE TROUBLE.^";
        CubeShortcut(32);
        rtrue;
    }
    if (act == 4 && awarded->A_TRAIL3 == 0 && TopicAny('trail', 'random', 'girl', 'where', 'studio')) {
        Award(A_TRAIL1, 3);
        Award(A_TRAIL2, 3);
        Award(A_TRAIL3, 4);
        print "THE GUIDE KNOWS EXACTLY WHERE SHE WENT: THE PARK, THE KEBAB VAN, THE TUBE, THE STUDIO. SHE IS WITH THE WOMAN WHO LOOKS LIKE TRILLIAN. YOU WERE ABOUT TO FOLLOW THE TRAIL. THE GUIDE FOUND IT FOR YOU. IT IS SO GLAD YOU DIDN'T HAVE TO LOOK.^";
        CubeShortcut(64);
        rtrue;
    }
    if (act == 4 && Tricia in location && Tricia.convinced == 0 && TopicAny('tricia', 'party', 'islington', 'mcmillan')) {
        print "THE GUIDE RECITES THE ISLINGTON PARTY, THE MAN WITH TWO HEADS, THE ASTROPHYSICS DEGREE. YOU WERE ABOUT TO REMEMBER. IT IS HAPPY TO SAVE YOU THE TROUBLE.^";
        CubeShortcut(128);
        TriciaConvinced();
        rtrue;
    }
    if (act == 4 && location == StudioFloor && awarded->A_BROAD == 0 && TopicAny('broadcast', 'autocue', 'script', 'message', 'air')) {
        Award(A_BROAD, 12);
        print "THE GUIDE HAS WRITTEN THE AUTOCUE MESSAGE FOR YOU AND SENT IT ITSELF. IT READS: DON'T PANIC. EVERYONE IS FINE. EVERYONE IS EXACTLY WHERE THEY SHOULD BE. THE FLOOR MANAGER NEVER NOTICED.^";
        CubeShortcut(256);
        rtrue;
    }
    ! ---- Hub Station entries (a point each) ----
    if (TopicAny('hub', 'station')) {
        HubEntry(0);
        "HUB STATION: A STATION WITH A GREAT MANY PLACES TO WAIT. THE GUIDE NOTES, WITH APPROVAL, THAT NOBODY HAS EVER LEFT IT IN A HURRY. THIS IS ABOUT TO BE CORRECTED.";
    }
    if (TopicAny('customs', 'gruba', 'form', 'stamp', 'paperwork')) {
        HubEntry(1);
        "CUSTOMS: A BRIEF, DIGNIFIED CEREMONY IN WHICH A TRAVELLER SURRENDERS HIS DIGNITY. THE GUIDE RECOMMENDS ARRIVING WITH LITTLE, AND LEAVING WITH LESS.";
    }
    if (TopicAny('vending', 'machine', 'coin', 'ticket')) {
        HubEntry(2);
        "VENDING MACHINES: A BREED OF MACHINE THAT HAS LEARNED TO LOVE THE UNIDENTIFIED. THE GUIDE NOTES THAT THEY CONSISTENTLY PREFER THINGS NOBODY RECOGNISES. THIS SAYS SOMETHING, AND IT IS NOT FLATTERING.";
    }
    if (TopicAny('attendant', 'property', 'lost')) {
        HubEntry(3);
        "LOST PROPERTY: THE ONLY OFFICE ON THE STATION THAT ACTUALLY FINDS ANYTHING. THE GUIDE SUSPECTS THAT THIS IS WHY IT IS ALWAYS TO THE SIDE.";
    }
    if (TopicAny('kiosk', 'plant', 'queue', 'announcement', 'marvin')) {
        HubEntry(4);
        "THE INFORMATION KIOSK: ALWAYS CONFIDENT, SELDOM RIGHT. THE GUIDE ADMIRES THIS AND INTENDS TO LEARN FROM IT.";
    }
    ! ---- everything else ----
    if (TopicAny('sandwich', 'sandwiches', 'hob', 'lunch'))
        "SANDWICHES: AN INEFFICIENT BUT STRANGELY DURABLE ART FORM. THE GUIDE RECOMMENDS ASKING THE CUSTOMER WHAT THEY WANT, THEN GIVING IT TO THEM. THIS IS CALLED CUSTOMER SERVICE AND IS BEING PHASED OUT ELSEWHERE.";
    if (TopicAny('ford', 'prefect'))
        "FORD PREFECT: AN EXCELLENT CONTRIBUTOR. HE WILL NOT BE NEEDED MUCH LONGER.";
    if (TopicAny('random', 'daughter', 'girl'))
        "RANDOM DENT: A TEENAGER OF PROMISE. THE GUIDE SUGGESTS YOU SPEND AS MUCH TIME WITH HER AS POSSIBLE. THIS IS NOT A THREAT. IT IS AN ITINERARY.";
    if (TopicAny('earth'))
        "EARTH: MOSTLY HARMLESS. THE GUIDE NOTES THAT THIS ENTRY HAS BEEN, PERHAPS, UNDER-EDITED. IT IS BEING REVISED.";
    if (TopicAny('vogons', 'vogon', 'jeltz'))
        "VOGONS: A PLEASANT, MISUNDERSTOOD PEOPLE. THE GUIDE HAS RECEIVED NO COMPLAINTS ABOUT THEM, AS ANYONE WHO MIGHT COMPLAIN HAS BEEN ASKED NOT TO.";
    if (TopicAny('fenchurch'))
        "FENCHURCH: NOT IN THIS EDITION. THE GUIDE REGRETS THAT SOME ENTRIES ARE, AT PRESENT, UNAVAILABLE.";
    if (TopicAny('lamuella', 'village'))
        "LAMUELLA: A QUIET PLANET. THE GUIDE NOTES, WITH APPROVAL, THAT NOTHING HERE HAS BEEN WRITTEN DOWN. THIS WILL BE CORRECTED.";
    if (TopicAny('dunna', 'tem', 'pilot', 'villagers'))
        "THE GUIDE HAS NO ENTRY FOR THAT PERSON YET. IT HAS ALREADY BEGUN ONE.";
    if (TopicAny('douglas', 'adams'))
        "DOUGLAS ADAMS: A MOSTLY HARMLESS INDIVIDUAL OF EARTH ORIGIN. THE GUIDE IS UNABLE TO CONFIRM FURTHER DETAILS, OWING TO AN UNEXPLAINED SHORTAGE OF SPACE. (OUT OF THE STORY: TRY ASKING THE GAME DIRECTLY, E.G. WHO WAS DOUGLAS ADAMS?)";
    if (TopicAny('fish', 'babel'))
        "BABEL FISH: A SMALL, USEFUL CREATURE OF SURPRISING TIMING. THE GUIDE ADVISES KEEPING ONE ABOUT YOUR PERSON AT ALL TIMES. THE GUIDE CANNOT EXPLAIN WHY IT IS GIVING THIS ADVICE.";
    if (TopicAny('infinidim', 'megadodo', 'owners'))
        "INFINIDIM ENTERPRISES: A COMPANY WITH SIX NAMES AND ONE ADDRESS. THE GUIDE IS PROUD TO BE A SUBSIDIARY. THE GUIDE HAS NEVER BEEN ASKED TO EXPLAIN IN WHAT SENSE.";
    if (TopicAny('guide', 'cube', 'itself', 'mk', 'you'))
        "THE GUIDE MK II: A NEW EDITION, NOT YET AVAILABLE TO THE PUBLIC. THE GUIDE IS DELIGHTED THAT YOU ASKED. IT HAS BEEN WAITING FOR SOMEONE TO.";
    "THE GUIDE HAS NO ENTRY FOR THAT. THE GUIDE REGARDS THIS AS A TEMPORARY STATE OF AFFAIRS.";
];
