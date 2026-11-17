!  slash.h -- out-of-story commands are slash commands, as in a chat program.
!
!  /SAVE  /RESTORE  /QUIT  /HINT  /SCORE ... are the commands about the game
!  rather than the story.  The leading slash is removed before the parser
!  sees them.  If the player types one of those words *without* the slash,
!  the game says so instead of obeying, and names the slash version.
!  Anything else typed (including longer sentences that merely begin with such
!  a word, like "save the earth") is treated as ordinary story input.
!
!  Called from BeforeParsing (see smart.h).

Array slash_txt -> 24;
Global slash_len  = 0;
Global slash_on   = 0;     ! 1 if this turn's command began with a slash
Global slash_miss = 0;     ! 1: bare word needing a slash; 2: unknown /command
Global slash_code = 0;
Array slash_file -> 44;
Global slash_file_len = 0;

! Which slash command, if any, does dictionary word w stand for?
[ SlashCode w;
    switch (w) {
      'save':                                  return 1;
      'restore':                               return 2;
      'restart':                               return 3;
      'quit', 'q//', 'die':                    return 4;
      'undo':                                  return 5;
      'score':                                 return 6;
      'fullscore', 'full':                     return 7;
      'notify':                                return 8;
      'verbose', 'long':                       return 9;
      'brief', 'normal':                       return 10;
      'superbrief', 'short':                   return 11;
      'script', 'transcript':                  return 12;
      'unscript', 'noscript':                  return 13;
      'version':                               return 14;
      'hint', 'hints', 'clue', 'clues':        return 15;
      'about', 'credits', 'info':              return 16;
      'help':                                  return 17;
      'pronouns', 'nouns':                     return 18;
      'difficulty':                            return 19;
      'macro':                                 return 20;
      'intro', 'introduction', 'tutorial', 'newbie': return 21;
      'hhgg', 'background':                    return 22;
    }
    rfalse;
];

[ SlashPrint k;
    switch (k) {
      1:  print "SAVE";
      2:  print "RESTORE";
      3:  print "RESTART";
      4:  print "QUIT";
      5:  print "UNDO";
      6:  print "SCORE";
      7:  print "FULLSCORE";
      8:  print "NOTIFY";
      9:  print "VERBOSE";
      10: print "BRIEF";
      11: print "SUPERBRIEF";
      12: print "SCRIPT";
      13: print "UNSCRIPT";
      14: print "VERSION";
      15: print "HINT";
      16: print "ABOUT";
      17: print "HELP";
      18: print "PRONOUNS";
      19: print "DIFFICULTY";
      20: print "MACRO";
      21: print "INTRO";
      22: print "HHGG";
    }
];

! Make the buffer say just 'slashmiss' (a word the player is never likely
! to type), so that the parser lands on SlashMissSub with nothing else to do.
[ SlashReplace;
    buffer->1 = 9;
    buffer->2 = 's'; buffer->3 = 'l'; buffer->4 = 'a'; buffer->5 = 's';
    buffer->6 = 'h'; buffer->7 = 'm'; buffer->8 = 'i'; buffer->9 = 's';
    buffer->10 = 's';
    Tokenise__(buffer, parse);
];

! First step: a leading slash is noted and removed.  A lone "/" means /HELP.
[ SlashStrip;
    slash_on = 0;
    slash_miss = 0;
    if (NumberWords() == 0) rfalse;
    if ((WordAddress(1))->0 ~= '/') rfalse;
    (WordAddress(1))->0 = ' ';
    slash_on = 1;
    Tokenise__(buffer, parse);
    if (NumberWords() == 0) {
        buffer->1 = 4;
        buffer->2 = 'h'; buffer->3 = 'e'; buffer->4 = 'l'; buffer->5 = 'p';
        Tokenise__(buffer, parse);
    }
];

! Last step, after typo correction: decide whether the command is allowed.
[ SlashCheck nw w k i n;
    nw = NumberWords();
    if (nw == 0) rfalse;
    wn = 1;
    w = NextWord();
    k = SlashCode(w);

    if (slash_on) {
        if ((k == 1 || k == 2) && nw >= 2) { SlashFileName(k); rfalse; }
        if (k == 2 && slash_file_len > 0) {
            print "(Your last file name this session was ~";
            for (i = 0 : i < slash_file_len : i++) print (char) slash_file->i;
            print "~.)^";
        }
        if (k ~= 0) rfalse;                ! a real slash command: go ahead
        n = WordLength(1);
        if (n > 20) n = 20;
        for (i = 0 : i < n : i++) slash_txt->i = (WordAddress(1))->i;
        slash_len = n;
        slash_miss = 2;
        SlashReplace();
        rfalse;
    }

    if (k == 0) rfalse;                    ! nothing to do with slash commands
    if (nw > 2) rfalse;                    ! a sentence, so story input
    if (nw == 2) {
        wn = 2;
        w = NextWord();
        if (k == 19) { }                    ! /DIFFICULTY takes any word
        else if (k == 15) {                 ! /HINT BACK, /HINT MORE
            if (w ~= 'back' or 'more' or 'again' or 'repeat') rfalse;
        }
        else if (w ~= 'on' or 'off' or 'score') rfalse;
    }
    slash_code = k;
    slash_miss = 1;
    SlashReplace();
    rfalse;
];

[ SlashMissSub i;
    if (slash_miss == 2) {
        print "There is no slash command called /";
        for (i = 0 : i < slash_len : i++) print (char) slash_txt->i;
        print ". /HELP lists them.^";
        rtrue;
    }
    print "I'm afraid I didn't follow that. Did you want the slash version? /";
    SlashPrint(slash_code);
    new_line;
    rtrue;
];

[ HelpSub;
    print "^Commands outside the story start with a slash, as in a chat program:^^";
    print "  /SAVE  /RESTORE  /RESTART  /QUIT  /UNDO       the game itself^";
    print "  /SCORE  /FULLSCORE  /NOTIFY ON|OFF            your progress^";
    print "  /VERBOSE  /BRIEF  /SUPERBRIEF                 how much is described^";
    print "  /SCRIPT  /UNSCRIPT                            a transcript of your game^";
    print "  /HINT [BACK]  /DIFFICULTY  /ABOUT  /VERSION  /HELP    help^";
    print "  /MACRO RECORD|STOP|PLAY|LIST|SHOW|DELETE   record and replay commands^";
    print "  /INTRO  /HHGG                              overviews for newcomers^^";
    print "Everything else you type is part of the story. Plain English is fine, and so are questions, such as WHO IS FORD PREFECT? Without the slash, those words are just ordinary input, and the game will tell you if you needed one.^";
    rtrue;
];

! ------------------------------------------------------------------------
!  /SAVE name  and  /RESTORE name
!
!  A Z-machine game cannot choose the file name: the interpreter asks for it
!  itself.  So the name is taken from the command, removed, and handed back
!  to the player as a reminder just before the interpreter's own prompt.
! ------------------------------------------------------------------------


[ SlashFileName k  n i p;
    n = WordLength(2);
    if (n > 40) n = 40;
    for (i = 0 : i < n : i++) slash_file->i = (WordAddress(2))->i;
    slash_file_len = n;
    p = (WordAddress(1) - buffer) + WordLength(1);
    buffer->1 = p - 2;                       ! the verb alone
    Tokenise__(buffer, parse);
    print "(When your interpreter asks for a file name, give it ~";
    for (i = 0 : i < n : i++) print (char) slash_file->i;
    print "~. A story file cannot choose the name for you.)^";
];
