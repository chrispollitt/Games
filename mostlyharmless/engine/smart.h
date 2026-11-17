!  smart.h -- the flexible-input engine shared by the MOSTLY HARMLESS games.
!
!  1. BeforeParsing cleans up what the player typed: punctuation, apostrophes,
!     and chatty filler ("please", "I want to", "could you", ...).
!  2. UnknownVerb maps a few dozen everyday verbs onto the ones the game has.
!  3. ParserError tries to answer failed input as a question, and otherwise
!     gives a friendlier message than the stock one.
!  4. Lore answers "who/what/where/why/how ..." questions.  The game supplies
!     LoreSelf (about the player and the game) and LoreWorld (in-story facts);
!     the engine supplies LoreReal (real-world facts, marked as out of story).
!  5. Talk and Hello accept loose, natural phrasings.
!
!  A game that uses this engine must define, before Include "smart":
!     Hooks:  HintSub, AboutSub, WalkToSub, TalkTopics(noun), ParserExamples,
!             LoreSelf, LoreWorld
!  (see engine/README.md).

! True if any of the (up to seven) given words appears in the topic typed
[ TopicAny a b c d e f g i w;
    wn = consult_from;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w ~= 0 && (w == a || w == b || w == c || w == d || w == e || w == f || w == g))
            rtrue;
    }
    rfalse;
];

! ------------------------------------------------------------------------
!  1. Input clean-up
! ------------------------------------------------------------------------

[ BlankWord k j;
    for (j = 0 : j < WordLength(k) : j++)
        (WordAddress(k))->j = ' ';
];

! Remove chatty words from the front (and a few from the back) of a command
[ StripFiller w1 w2 n i changed nw;
    do {
        changed = 0;
        nw = NumberWords();
        if (nw == 0) return;
        wn = 1;
        w1 = NextWordStopped();
        w2 = NextWordStopped();
        n = 0;
        if (w1 == 'please' or 'kindly' or 'now' or 'just' or 'ok' or 'okay'
                or 'hey' or 'well' or 'so' or 'then' or 'quickly' or 'um'
                or 'uh' or 'hm' or 'id' or 'ill' or 'ive' or 'to' or 'like' or 'lets' or 'wanna'
                or 'gonna' or 'maybe' or 'perhaps' or 'actually' or 'and'
                or 'next' or 'first' or 'finally' or 'also' or 'sure'
                or 'yes' or 'yeah')
            n = 1;
        else if (w1 == 'i//' && w2 == 'want' or 'need' or 'wish' or 'would'
                or 'will' or 'shall' or 'should' or 'must' or 'll' or 'd//'
                or 'can' or 'could' or 'try' or 'think' or 'guess' or 'suppose')
            n = 2;
        else if ((w1 == 'can' or 'could' or 'would' or 'will' or 'may'
                or 'might' or 'shall' or 'should') && (w2 == 'you' or 'i//' or 'we'))
            n = 2;
        else if (w1 == 'let' && w2 == 'us' or 'me')
            n = 2;
        else if (w1 == 'we' && w2 == 'should' or 'could' or 'can' or 'will'
                or 'must' or 'need' or 'want')
            n = 2;
        else if (w1 == 'you' && w2 == 'should' or 'could' or 'can' or 'must')
            n = 2;
        else if (w1 == 'try' && w2 == 'to')
            n = 2;
        else if (w1 == 'go' && w2 == 'ahead')
            n = 2;
        if (n > 0 && n < nw) {
            for (i = 1 : i <= n : i++) BlankWord(i);
            Tokenise__(buffer, parse);
            changed = 1;
        }
        else if (nw > 1) {
            wn = nw;
            w1 = NextWord();
            if (w1 == 'please' or 'now' or 'thanks' or 'thx' or 'then'
                    or 'already' or 'quickly' or 'again') {
                BlankWord(nw);
                Tokenise__(buffer, parse);
                changed = 1;
            }
        }
    } until (changed == 0);
];

[ BeforeParsing i j n c;
    n = buffer->1;
    j = 2;
    for (i = 2 : i < n + 2 : i++) {
        c = buffer->i;
        ! question marks, shouting and brackets are just noise
        if (c == '?' or '!' or ':' or '(' or ')') c = ' ';
        if (c == ';') c = ',';
        ! apostrophes disappear, so that WHAT'S becomes WHATS
        if (c == 39 or 96) continue;
        ! 1-2-5 or 1,2,5 between digits: separate the numbers
        if ((c == '-' or ',') && i > 2 && i < n + 1 && buffer->(i - 1) >= '0' && buffer->(i - 1) <= '9' && buffer->(i + 1) >= '0' && buffer->(i + 1) <= '9') c = ' ';
        buffer->j = c;
        j++;
    }
    for (i = j : i < n + 2 : i++) buffer->i = ' ';
    buffer->1 = j - 2;
    Tokenise__(buffer, parse);
    SlashStrip();
    TypoFixAll();
    SplitCompound();
    StripFiller();
    SlashCheck();
    rfalse;
];

! ------------------------------------------------------------------------
!  2. Everyday verbs the grammar does not know
! ------------------------------------------------------------------------

[ UnknownVerb word;
    switch (word) {
      'grab', 'snatch', 'fetch', 'collect', 'acquire', 'procure', 'retrieve',
      'pocket', 'obtain', 'lift', 'hold':
        return 'take';
      'inspect', 'study', 'observe', 'scrutinise', 'scrutinize', 'view',
      'regard', 'peruse', 'investigate', 'explore', 'admire', 'ogle':
        return 'examine';
      'stroll', 'wander', 'hike', 'travel', 'saunter', 'amble', 'trek',
      'march', 'dash', 'hurry', 'rush', 'proceed':
        return 'go';
      'munch', 'devour', 'consume', 'nibble', 'chew', 'swallow', 'scoff',
      'snack', 'dine':
        return 'eat';
      'hand', 'present', 'deliver', 'serve', 'donate', 'pass', 'supply':
        return 'give';
      'activate', 'enable', 'boot', 'power':
        return 'switch';
      'glance', 'peek', 'peer', 'stare', 'gaze', 'glimpse', 'survey':
        return 'look';
      'query', 'inquire', 'enquire', 'question', 'interrogate', 'quiz':
        return 'ask';
      'inform', 'notify', 'report', 'mention', 'announce', 'warn':
        return 'tell';
      'linger', 'idle', 'loiter', 'dawdle', 'rest', 'pause':
        return 'wait';
      'board', 'embark', 'mount', 'climb', 'hop':
        return 'enter';
      'disembark', 'alight', 'dismount':
        return 'exit';
      'ponder', 'ruminate', 'muse', 'reflect', 'wonder', 'brood':
        return 'think';
      'chat', 'converse', 'natter', 'gossip', 'chatter', 'speak', 'discuss':
        return 'talk';
      'toss', 'discard', 'ditch', 'abandon', 'dump':
        return 'drop';
      'slice', 'chop', 'carve':
        return 'cut';
      'greet', 'hello', 'hi', 'hullo', 'wave':
        return 'hail';
    }
    rfalse;
];

! ------------------------------------------------------------------------
!  3. Friendlier failure
! ------------------------------------------------------------------------

[ ParserError etype;
    if (etype == STUCK_PE or VERB_PE) {
        ! The player may well have typed a question or a bare topic
        consult_from = 1;
        consult_words = NumberWords();
        if (Lore()) rtrue;
        print "I'm afraid I didn't follow that. Plainer phrasing usually helps: ";
        ParserExamples();
        print_ret " /HINT will point you at what to do next.";
    }
    if (etype == CANTSEE_PE)
        "You can't see anything like that here. (LOOK and INVENTORY will remind you what is around.)";
    rfalse;
];

! ------------------------------------------------------------------------
!  4. Questions and reference
! ------------------------------------------------------------------------

! True if any word of the topic is a name of object o
[ ObjInTopic o i w;
    wn = consult_from;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w ~= 0 && WordInProperty(w, o, name)) rtrue;
    }
    rfalse;
];
! Real-world facts first, then the game's own answers
[ Lore;
    if (LoreSelf()) rtrue;
    if (LoreReal()) rtrue;
    if (LoreWorld()) rtrue;
    rfalse;
];

! How many words of the topic are neither a name of o nor ordinary question filler?
[ TopicExtras o i w n;
    wn = consult_from;
    n = 0;
    for (i = 0 : i < consult_words : i++) {
        w = NextWord();
        if (w == 0) continue;
        if (WordInProperty(w, o, name)) continue;
        if (w == 'is' or 'are' or 'was' or 'the' or 'a' or 'an' or 'who' or 'whos' or 'what' or 'whats'
                or 'that' or 'this' or 'about') continue;
        n++;
    }
    return n;
];

[ QuestionSub o;
    if (Lore()) rtrue;
    ! Otherwise, if the question mentions someone in the room, put it to them;
    ! if it mentions something in sight, describe it
    objectloop (o) {
        if (o ~= player && o ~= location && TestScope(o, player) && ObjInTopic(o)) {
            if (o has animate && TopicExtras(o) > 0) {
                print "(Asking ", (the) o, ".)^";
                <Ask o>;
                rtrue;
            }
            print "(Taking a closer look at ", (the) o, ".)^";
            <Examine o>;
            rtrue;
        }
    }
    "You haven't the faintest idea, and nobody here looks any better informed. You can EXAMINE things, ASK people about them, or type /HINT for a nudge.";
];

! ------------------------------------------------------------------------
!  5. Looser phrasings
! ------------------------------------------------------------------------

[ TalkSub;
    if (noun == player) "You mutter to yourself. It doesn't help, but it's company.";
    if (noun hasnt animate) "Talking to ", (the) noun, " won't get you far.";
    print "You could ASK ", (the) noun, " ABOUT something";
    TalkTopics(noun);
    ". Or just type your question.";
];

[ HelloSub;
    if (noun == 0 || noun == player) "Hello, yourself. There is nobody here to hear you who has not already heard.";
    if (noun hasnt animate) "You greet ", (the) noun, ". It declines to reply.";
    print_ret "You greet ", (the) noun, ", who acknowledges you in the manner of someone who has other things on their mind. You could ASK ", (the) noun, " ABOUT something.";
];

Include "slash";
Include "typo";
Include "compound";
Include "lore_real";
Include "hints";
