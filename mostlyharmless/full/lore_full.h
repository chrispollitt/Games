!  lore_full.h -- answers to questions in THE LONG WAY ROUND (Act I so far).

! About the player and the game itself.  Runs before the real-world facts.
[ LoreSelf v;
    v = verb_word;

    ! ---- the player and the game itself ----
    if (TopicAny('should', 'next', 'supposed', 'stuck', 'help', 'now')
            && TopicAny('i//', 'we', 'me', 'do')) {
        HintSub();
        rtrue;
    }
    if (TopicAny('can', 'could') && TopicAny('do', 'try', 'say', 'type')) {
        "You can do most of the usual things: LOOK, EXAMINE things, TAKE and DROP them, go in compass directions (or GO TO somewhere), and TALK to people (ASK HOB ABOUT LUNCH, or just ask a question like WHO IS FORD PREFECT?). At the sandwich hut you can MAKE any sandwich you like: MAKE A SANDWICH WITH MEAT AND SOURBERRIES, or even MAKE HOB THE USUAL. You can type naturally; the game tries to work out what you mean. Commands that are not part of the story start with a slash: /HINT gives a nudge, /ABOUT tells you about the game, and /HELP lists the rest (/SAVE, /RESTORE and so on).";
    }
    if ((v == 'where' or 'wheres') && TopicAny('i//', 'am', 'we', 'this')) {
        print "You are on Lamuella, if you are anywhere near the start, or on an Earth, if you have got that far. Looking around:^";
        LookSub();
        rtrue;
    }
    if ((v == 'who' or 'whos' or 'what' or 'whats') && TopicAny('i//', 'me', 'myself', 'am')) {
        "You are Arthur Dent, formerly of Earth, currently of Lamuella: a man of no great consequence who has, over the years, been dragged through more of the Galaxy than anyone ought to be, and has decided with some firmness to stay put. You make sandwiches. It suits you.";
    }
    if (TopicAny('you', 'narrator', 'author', 'wrote', 'made', 'game', 'story')
            && TopicAny('who', 'whos', 'what', 'game', 'story', 'this', 'you')) {
        print "[Out of the story: this is an unofficial interactive fan adaptation of the fifth Hitchhiker's Guide novel, with entirely original prose, written as a non-commercial tribute. It has two endings: the book's, and the happier one from the BBC radio adaptation. Type /ABOUT for the details, /HINT if you are stuck, or ask it things like WHO WAS DOUGLAS ADAMS?]^";
        rtrue;
    }
    rfalse;
];

! In-story facts about the Galaxy and its people.  Runs after LoreReal.
[ LoreWorld;
    if (TopicAny('wsogmm', 'mish', 'mash', 'mishmash')) {
        print "The Whole Sort of General Mish Mash, as Ford would tell you, is what everything is until somebody decides otherwise: every way things might have gone, all piled up together like coats at a party. It is a handy name for the sort of mess that cannot be sorted, and a handier excuse. Do not try to tidy it. It has views.^^";
        MishFootnote();
        rtrue;
    }
    if (TopicAny('towel', 'towels')) {
        "A towel is, by long Guide tradition, the most useful thing a hitchhiker can carry: warm, absorbent, a weapon in a pinch, and above all a signal. Anyone seen with one is assumed to have everything else as well. Ford has never been seen without his.";
    }
    if (TopicAny('babel', 'fish')) {
        "The Babel fish: a small yellow thing that lives in your ear, feeds on the brainwaves of whoever is near, and in return translates every language you hear into your own. It is such a useful coincidence that some people take it as proof of something. You have never had the energy to find out what. There is one in a jar in your hut.";
    }
    if (TopicAny('vogon', 'vogons', 'jeltz')) {
        "Vogons are the Galaxy's most efficient bureaucrats and its least attractive people. They demolished the Earth to build a hyperspace bypass, and have been completing the paperwork ever since. Their poetry is also said to be dreadful. Nobody has asked you to confirm this.";
    }
    if (TopicAny('earth', 'planet', 'bypass', 'demolished')) {
        "Earth: a small blue-green planet, demolished some time ago to make way for a hyperspace bypass. The Guide's entry on it was two words long and has been enlarged since. You have strong views on the matter, mostly kept to yourself.";
    }
    if (TopicAny('guide', 'hitchhiker')) {
        "The Hitchhiker's Guide to the Galaxy is the best-selling travel book in the Galaxy, partly because of its helpful cover and partly because it is cheaper than the Encyclopaedia Galactica. It has been correct about you twice. The Mk II is a different matter.";
    }
    if (TopicAny('mk', 'ii', 'cube', 'megadodo', 'infinidim')) {
        "The Guide Mk II is a new edition that has not been published and, you suspect, should not be. Ford says it reads you while you read it. The company that owns it has six names and one address, and the address belongs to Vogons.";
    }
    if (TopicAny('zaphod', 'beeblebrox')) {
        "Zaphod Beeblebrox: two heads, three arms at the last count, one-time President of the Galaxy, and a man who has never been seen to doubt himself. He is nowhere near here, which is a mercy.";
    }
    if (TopicAny('marvin', 'robot', 'android')) {
        "Marvin: a robot with a brain the size of a planet and no one to talk to about it. He has a terrible pain in all the diodes down his left side. You have always rather liked him.";
    }
    if (TopicAny('magrathea', 'slartibartfast', 'deep', 'thought', 'improbability')) {
        "These are old stories now. A planet that builds planets; a computer that took seven and a half million years to answer a question; a drive that makes the most unlikely things happen. You were present for some of it, and it has not made you happier.";
    }
    if (TopicAny('restaurant', 'universe')) {
        "The Restaurant at the End of the Universe: a restaurant where you can watch the cosmos finish, from a good table, with dinner. It would be a lovely place to wait for someone.";
    }
    if (TopicAny('panic', 'dont')) {
        "DON'T PANIC: the words on the front of the Guide, in large friendly letters. Excellent advice. Rarely followed.";
    }
    if (TopicAny('fenchurch')) {
        "Fenchurch: the woman you loved. She vanished in the middle of a hyperspace jump, and nobody has ever told you where she went. The pencil sketch on your shelf is all you have of her tilt of the head.";
    }
    if (TopicAny('agrajag')) {
        "Agrajag: a creature who is born again and again as small things you happen to kill. It is not a friendly relationship. You have been told it expects to meet a particular end at a particular time.";
    }
    if (TopicAny('grebulon', 'grebulons')) {
        "You have not met the Grebulons yet. From what Ford says, they are a ship's crew with a mission, a broken memory and a great interest in star charts.";
    }
    if (TopicAny('lamuella')) {
        "Lamuella: a quiet, green planet far from anywhere, where the biggest event of the week is lunch. You are the sandwich maker, which is the first useful thing you have been in years. It is, very nearly, a home.";
    }
    if (TopicAny('random')) {
        "Random Dent: Trillian's daughter and, by arrangement you would prefer not to picture, yours. She is clever, furious and exactly as old as the trouble she is about to cause.";
    }
    if (TopicAny('trillian', 'tricia', 'mcmillan')) {
        "Trillian: the one who got away, in the sense that she got away from the Earth rather a long time before you did. There is another Earth where she is called Tricia McMillan and never left.";
    }
    if (TopicAny('ford', 'prefect')) {
        "Ford Prefect: a researcher for the Guide, an alien from a small planet near Betelgeuse, and the best friend and worst influence you have ever had. He is almost always right about the important things and almost never about the dates.";
    }
    if (TopicAny('arthur', 'dent')) {
        "Arthur Dent: you. An Englishman, a survivor, a sandwich maker. The man whose house was demolished on the day his planet was.";
    }
    if (TopicAny('sandwich', 'sandwiches', 'lunch')) {
        "You are very good at them: bread, then whatever the customer actually wants, no more and no less. People on Lamuella have views about this. Try MAKE A SANDWICH WITH MEAT AND SOURBERRIES, or ASK a customer what they like.";
    }
    rfalse;
];

! Who the player might want to talk to, appended to "You could ASK X ABOUT something"
[ TalkTopics who;
    switch (who) {
      hob:      print " (his lunch, say)";
      Dunna:    print " (meat, her cleaver, the price)";
      Tem:      print " (his tooth, the Stone)";
      pilot:    print " (the skiff, or his lunch)";
      Trillian: print " (Random, or Earth)";
      Daughter: print " (food, her mother, the Guide)";
      Ford:     print " (the Guide, the Vogons, the plan)";
    }
];

! Sample commands for the "I didn't follow that" message
[ ParserExamples;
    print "TAKE KNIFE, ASK HOB ABOUT LUNCH, GO TO THE MEADOW, MAKE A SANDWICH WITH MEAT, or a question such as WHO IS FORD PREFECT?";
];

! Where this game sits in the franchise (used by /HHGG)
[ GameBlurb;
    print "Where this game fits: THE LONG WAY ROUND adapts the fifth and last novel, Mostly Harmless (1992), in which Arthur has settled on a quiet planet and found a good use for his hands: he makes sandwiches. It follows the book's main events and invents the rest; and, like the BBC radio version of that book, it has a happier ending as well as the novel's.^";
];

! Egg hooks (eggs.h)
[ EggRoomOK r;
    switch (act) {
      1: if (r == Hut or Green or SandHut or Orchard or Smokehouse or Lodge or Meadow or River) rtrue;
      2: if (r == Arrivals or Customs or Concourse or LostProp or Gate) rtrue;
      3: if (r == Plaza or Lobby or Security or Cloakroom or LiftLobby or Editorial or FordOffice or Archive) rtrue;
      4: if (r == PressPen or Cordon or HighRoad or Tube or Plat1 or Plat2 or StudioLobby or StudioFloor or GreenRoom or Flat) rtrue;
      5: if (r == Bar or Snug or Cellar or PubYard or Lawn) rtrue;
      6: if (r == DiningHall or Foyer or Kitchen or Terrace) rtrue;
    }
    rfalse;
];

[ EggKeep o;
    if (o == guide) rtrue;
    rfalse;
];

! The Mish Mash dice stay quiet at the story's tense moments (mishmash.h)
[ MishQuiet;
    if (act == 5 && gun_fired == 1) rtrue;
    if (act == 3 && location == Boardroom && heard_meeting == 0) rtrue;
    rfalse;
];

! ------------------------------------------------------------------------
!  The Whole Sort of General Mish Mash (WSOGMM): a running gag
! ------------------------------------------------------------------------

[ MishFootnote;
    print "[Out of the story: the joke has a respectable ancestor. In 1957 the physicist Hugh Everett III published a paper on the ~relative state~ formulation of quantum mechanics, from which grew the many-worlds interpretation, in which every possible outcome of a quantum event is real in some branch of a larger whole. Richard Feynman's sum over histories (1948) likewise treats a particle as taking every path at once. Neither would care to be quoted on sandwiches. The Mish Mash is the game having a little fun at their expense.]^";
];

! Something a character says when asked about it; true if it answered
[ MishTalk who;
    switch (who) {
      Ford:     "~The Mish Mash,~ says Ford, with the weariness of a man who has stopped trying to explain things. ~It's what you call everything when you can't be bothered to separate it. Possible, impossible and what was for lunch. Don't try to sort it. It dislikes being sorted.~";
      Daughter: "~Is it a real thing?~ says Random. ~Everyone says it as if it is. If it is, I'd like to see the file.~";
      Tricia:   "~It sounds like a theory with the nouns taken out,~ says Tricia. ~Give me the maths and I'll tell you whether it's physics or marketing.~";
      Zendra:   "~Management adores it,~ says Zendra. ~It means whatever the memo needs it to mean. It's what we write when we've been told not to explain.~";
      Marvin:   "~The Whole Sort of General Mish Mash,~ says Marvin. ~Every way things could go wrong, in one heap, all at once. I find it restful. I have been in it.~";
      hob:      "~Mish what?~ says Hob. ~Sounds like what happens to my sandwiches when you're not looking.~";
      Tem:      "~The Stone mentions it,~ says Tem, carefully. ~It does not say whether it is fond of it.~";
      Gruba:    "~Cause of delay,~ says Gruba, flatly. ~Form 27B/6 has a box for it. Third from the bottom. It is the only box anyone ever ticks.~";
      Orlo:     "~The Mish Mash,~ says Orlo, and his eyes mist. ~I have a poem about it. It is nine hundred lines. It is mostly about lunch.~";
      Fenchurch: "~Oh, the Mish Mash,~ says Fenchurch, smiling. ~It's where I was. It's a good deal tidier than people say, once you stop trying to sort it.~";
      Floyd:    "~Is that a game?~ says Floyd. ~I like games. Is it one where nobody wins and everybody has a nice time? Oh boy.~";
      Dunna:    "~Mish mash,~ says Dunna, sniffing the smoke. ~Sounds like a stew.~";
    }
    print_ret (The) who, " gives a small, careful shrug. ~Not my department,~ comes the reply, in the tone of someone who has heard the phrase used in a more important room.";
];


! After XYZZY: the Guide would say it was the Mish Mash leaking
[ EggAside;
    if (eg-->EG_COUNT <= 2)
        print "(That, the Guide would say, was merely the Whole Sort of General Mish Mash leaking.)^";
];

! words the player may type about the Mish Mash, so that the typo corrector
! does not mistake them for something else
Dictionary 'sort';
Dictionary 'whole';
Dictionary 'general';

! Called first thing in every character's Ask / Tell / Answer handler
[ MishAsk;
    if (TopicAny('wsogmm', 'mish', 'mash', 'mishmash')) {
        MishTalk(self);
        rtrue;
    }
    rfalse;
];
