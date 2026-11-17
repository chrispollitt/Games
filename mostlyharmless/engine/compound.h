!  compound.h -- compound commands:  TAKE PARCEL AND EXAMINE IT, DROP ALL, LOOK;
!  GO NORTH, THEN TAKE LAMP.
!
!  The standard library already treats THEN as the start of a second command,
!  but its test for "AND followed by a verb" mis-parses its own operator
!  precedence, so "take parcel and examine it" is read as a list of objects.
!  Here every AND, comma or semicolon that is followed by a verb is turned into
!  THEN before the parser sees the line; the library then runs the commands in
!  turn.  Called from BeforeParsing (see smart.h).

Array then_txt -> ' ' 't' 'h' 'e' 'n' ' ';

! Is word number k a verb (and not mainly a noun)?
[ CompoundVerb k  wv fl nxt;
    if (k < 1 || k > NumberWords()) rfalse;
    wv = WordValue(k);
    ! compass directions: "take berries and n"
    if (wv == 'n//' or 's//' or 'e//' or 'w//' or 'ne' or 'nw' or 'se' or 'sw' or 'u//' or 'd//'
            or 'north' or 'south' or 'east' or 'west') rtrue;
    if (wv == 'inventory' or 'inv' or 'i//' or 'wait' or 'z//' or 'look' or 'l//') rtrue;
    if (wv == 0) rfalse;
    fl = wv->#dict_par1;
    if ((fl & DICT_VERB) == 0) rfalse;
    if ((fl & DICT_NOUN) == 0) rtrue;
    ! a word that is both (STAMP is a verb and a rubber stamp): a verb if what
    ! follows looks like the start of its object
    if (k < NumberWords()) {
        wn = k + 1;
        nxt = NextWord();
        if (nxt == 'it' or 'them' or 'him' or 'her' or 'the' or 'a' or 'an' or 'my' or 'all' or 'everything')
            rtrue;
    } else
        rtrue;
    rfalse;
];

! Replace the len characters at buffer index p by " then "
[ SpliceThen p len  i d lim;
    d = 6 - len;
    if (buffer->1 + d > buffer->0) rfalse;          ! no room: leave it alone
    lim = buffer->1 + 2;                           ! one past the last character
    if (d > 0) {
        for (i = lim - 1 : i >= p + len : i--) buffer->(i + d) = buffer->i;
    }
    if (d < 0) {
        for (i = p + len : i < lim : i++) buffer->(i + d) = buffer->i;
    }
    for (i = 0 : i < 6 : i++) buffer->(p + i) = then_txt->i;
    buffer->1 = buffer->1 + d;
    rtrue;
];

[ SplitCompound  pass k nw w p len done;
    for (pass = 0 : pass < 4 : pass++) {
        nw = NumberWords();
        done = 0;
        for (k = 2 : k < nw : k++) {
            wn = k;
            w = NextWord();
            if (w == 'and' or comma_word) {
                ! a comma is only a separator when the line starts with a verb
                ! (otherwise it is a person being addressed: "Hob, give me ...")
                if (w == comma_word && ((WordValue(1) == 0) || ((WordValue(1)->#dict_par1) & DICT_VERB) == 0)) continue;
                if (CompoundVerb(k + 1) || (k + 2 <= nw && WordValue(k + 1) == 'then' && CompoundVerb(k + 2))) {
                    p = WordAddress(k) - buffer;
                    len = WordLength(k);
                    if (SpliceThen(p, len)) {
                        Tokenise__(buffer, parse);
                        done = 1;
                        break;
                    }
                }
            }
        }
        if (done == 0) return;
    }
];
