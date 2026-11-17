!  macro.h -- /MACRO: record and replay long command sequences.
!
!  /MACRO RECORD name   start recording the story commands you type
!  /MACRO STOP          finish recording
!  /MACRO PLAY name     replay (or just /MACRO name)
!  /MACRO LIST | SHOW name | DELETE name
!
!  It is deliberately not a skip button.  Every replayed command is echoed and
!  run as if typed: it takes its turn, prints its text and lets the story's
!  events happen.  Replay stops at the first surprise: a command the parser
!  could not follow, or any change in where you are, your score, or what is in
!  the room or in your hands, compared with how things stood after the same
!  command when you recorded it.  Slash commands are never recorded.
!
!  Needs "Replace KeyboardPrimitive;" before Include "Parser" in the game file.
!  The macros live in ordinary arrays, so /SAVE and /RESTORE keep them.

Constant MAC_N    = 6;      ! macros
Constant MAC_CMDS = 16;     ! commands per macro
Constant MAC_LEN  = 60;     ! bytes per command (length byte + 59 characters)

Array mac_name  -> 54;      ! 6 x 9: length byte, then up to 8 characters
Array mac_count -> 6;
Array mac_cmd   -> 5760;    ! 6 x 16 x 60
Array mac_fp1   --> 96;     ! where you were after each command
Array mac_fp2   --> 96;     ! the score
Array mac_fp3   --> 96;     ! what was in the room and in your hands
Array macro_tmp -> 12;

[ MacroTopicName  n i;
    macro_tmp->0 = 0;
    if (consult_words == 0) return 0;
    n = WordLength(consult_from);
    if (n > 8) n = 8;
    for (i = 0 : i < n : i++) macro_tmp->(i + 1) = (WordAddress(consult_from))->i;
    macro_tmp->0 = n;
    return n;
];

! Index of the macro whose name is in macro_tmp, or -1
[ MacroFind  i j same;
    if (macro_tmp->0 == 0) return -1;
    for (i = 0 : i < MAC_N : i++) {
        if (mac_name->(i * 9) ~= macro_tmp->0) continue;
        same = 1;
        for (j = 1 : j <= macro_tmp->0 : j++)
            if (mac_name->(i * 9 + j) ~= macro_tmp->j) same = 0;
        if (same) return i;
    }
    return -1;
];

[ MacroPrintName m  j;
    for (j = 1 : j <= mac_name->(m * 9) : j++) print (char) mac_name->(m * 9 + j);
];

[ MacroSnap c  idx;
    idx = mv-->MV_CUR * MAC_CMDS + c;
    mac_fp1-->idx = location;
    mac_fp2-->idx = score - mv-->MV_SCORE;         ! points that command earned
    mac_fp3-->idx = children(location) * 64 + children(player);
];

! Is the world as it was after command c when recorded?
[ MacroSame c  idx;
    idx = mv-->MV_CUR * MAC_CMDS + c;
    if (mac_fp1-->idx ~= location) rfalse;
    if (score - mv-->MV_SCORE > mac_fp2-->idx) rfalse;     ! unexpected points are a surprise; fewer are not
    if (mac_fp3-->idx ~= children(location) * 64 + children(player)) rfalse;
    rtrue;
];

[ MacroHalt why;
    mv-->MV_STATE = 0;
    if (why == 1)
        print "^(The macro stops here: the last command did not work this time.)^^>";
    else
        print "^(The macro stops here: things are not quite as they were when you recorded it.)^^>";
];

! Feed the next recorded command to the parser, if a macro is playing.
! Returns true if it supplied the line.
[ MacroFeed a_buffer a_table  base j n k p;
    if (mv-->MV_STATE ~= 2) rfalse;
    base = mv-->MV_CUR * MAC_CMDS;
    j = mv-->MV_POS - 1;
    if (mv-->MV_FAIL) { mv-->MV_FAIL = 0; MacroHalt(1); rfalse; }
    if (j >= 0 && MacroSame(j) == 0) { MacroHalt(2); rfalse; }
    if (mv-->MV_POS >= mac_count->(mv-->MV_CUR)) {
        mv-->MV_STATE = 0;
        print "^(The macro has finished.)^^>";
        rfalse;
    }
    p = (base + mv-->MV_POS) * MAC_LEN;
    n = mac_cmd->p;
    for (k = 0 : k < n : k++) {
        print (char) mac_cmd->(p + 1 + k);
        a_buffer->(2 + k) = mac_cmd->(p + 1 + k);
    }
    new_line;
    a_buffer->1 = n;
    Tokenise__(a_buffer, a_table);
    mv-->MV_SCORE = score;
    mv-->MV_POS = mv-->MV_POS + 1;
    rtrue;
];

! Remember the line the player has just typed, if recording
[ MacroRecordLine a_buffer  n k c p;
    n = a_buffer->1;
    if (n == 0) return;
    if (a_buffer->2 == '/') return;                ! slash commands are never recorded
    c = mac_count->(mv-->MV_CUR);
    if (c >= MAC_CMDS) {
        mv-->MV_STATE = 0;
        print "(The macro is full at sixteen commands, so recording has stopped.)^";
        return;
    }
    if (n > MAC_LEN - 1) n = MAC_LEN - 1;
    p = (mv-->MV_CUR * MAC_CMDS + c) * MAC_LEN;
    mac_cmd->p = n;
    for (k = 0 : k < n : k++) mac_cmd->(p + 1 + k) = a_buffer->(2 + k);
    mac_count->(mv-->MV_CUR) = c + 1;
    mv-->MV_LAST = c;
];

[ KeyboardPrimitive  a_buffer a_table;
    if (mv-->MV_STATE == 1 && mv-->MV_LAST >= 0) {
        MacroSnap(mv-->MV_LAST);                      ! the previous command is finished
        mv-->MV_LAST = -1;
    }
    if (MacroFeed(a_buffer, a_table)) return;
    read a_buffer a_table;
    if (mv-->MV_STATE == 1) MacroRecordLine(a_buffer);
    mv-->MV_SCORE = score;
];

! ------------------------------------------------------------------------
!  The /MACRO commands
! ------------------------------------------------------------------------

[ MacroGate;
    if (difficulty == DIFF_FAITHFUL) {
        print "Faithful mode does without macros. /DIFFICULTY AVERAGE turns them on.^";
        rtrue;
    }
    rfalse;
];

[ MacroListSub  i any;
    if (MacroGate()) rtrue;
    for (i = 0 : i < MAC_N : i++) {
        if (mac_name->(i * 9) == 0) continue;
        if (any == 0) print "Macros:^";
        any = 1;
        print "  ";
        MacroPrintName(i);
        print " (", mac_count->i, " command";
        if (mac_count->i ~= 1) print "s";
        print ")^";
    }
    if (any == 0)
        print "You have no macros yet. /MACRO RECORD name starts one; everything you type after that is remembered until /MACRO STOP.^";
    rtrue;
];

[ MacroRecordSub  m i;
    if (MacroGate()) rtrue;
    if (mv-->MV_STATE ~= 0) "You are already recording or replaying a macro.";
    if (MacroTopicName() == 0) "Name the macro: /MACRO RECORD lunch, for example.";
    m = MacroFind();
    if (m < 0) {
        for (i = 0 : i < MAC_N : i++) if (mac_name->(i * 9) == 0) { m = i; break; }
        if (m < 0) "All six macro slots are in use. /MACRO DELETE one first.";
        mac_name->(m * 9) = macro_tmp->0;
        for (i = 1 : i <= macro_tmp->0 : i++) mac_name->(m * 9 + i) = macro_tmp->i;
    }
    mac_count->m = 0;
    mv-->MV_CUR = m;
    mv-->MV_STATE = 1;
    mv-->MV_LAST = -1;
    print "(Recording ~";
    MacroPrintName(m);
    print "~. Everything you type now is remembered, up to sixteen commands. /MACRO STOP finishes.)^";
    rtrue;
];

[ MacroStopSub;
    if (MacroGate()) rtrue;
    if (mv-->MV_STATE ~= 1) "You are not recording a macro.";
    mv-->MV_STATE = 0;
    print "(Recorded ", mac_count->(mv-->MV_CUR), " command";
    if (mac_count->(mv-->MV_CUR) ~= 1) print "s";
    print " as ~";
    MacroPrintName(mv-->MV_CUR);
    print "~. /MACRO PLAY ";
    MacroPrintName(mv-->MV_CUR);
    print " replays it.)^";
    rtrue;
];

[ MacroPlaySub  m;
    if (MacroGate()) rtrue;
    if (mv-->MV_STATE ~= 0) "You are already recording or replaying a macro.";
    if (MacroTopicName() == 0) "Which macro? /MACRO LIST shows them.";
    m = MacroFind();
    if (m < 0) "You have no macro by that name. /MACRO LIST shows them.";
    if (mac_count->m == 0) "That macro is empty.";
    mv-->MV_CUR = m;
    mv-->MV_POS = 0;
    mv-->MV_FAIL = 0;
    mv-->MV_STATE = 2;
    print "(Replaying ~";
    MacroPrintName(m);
    print "~. Each command is run as if you had typed it, and the replay stops if anything is different.)^";
    rtrue;
];

[ MacroShowSub  m i p k;
    if (MacroGate()) rtrue;
    if (MacroTopicName() == 0) "Which macro? /MACRO LIST shows them.";
    m = MacroFind();
    if (m < 0) "You have no macro by that name.";
    print "Macro ~";
    MacroPrintName(m);
    print "~:^";
    for (i = 0 : i < mac_count->m : i++) {
        p = (m * MAC_CMDS + i) * MAC_LEN;
        print "  ", i + 1, ". ";
        for (k = 0 : k < mac_cmd->p : k++) print (char) mac_cmd->(p + 1 + k);
        new_line;
    }
    rtrue;
];

[ MacroDeleteSub  m;
    if (MacroGate()) rtrue;
    if (mv-->MV_STATE ~= 0) "Finish recording or replaying first.";
    if (MacroTopicName() == 0) "Which macro? /MACRO LIST shows them.";
    m = MacroFind();
    if (m < 0) "You have no macro by that name.";
    mac_name->(m * 9) = 0;
    mac_count->m = 0;
    "Deleted.";
];
