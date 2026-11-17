!  typo.h -- typo forgiveness for MOSTLY HARMLESS.
!
!  A word the game does not know is tried with every single-letter slip:
!  two letters swapped, one letter too many, one letter missing, or one
!  letter wrong.  If exactly one known word results, that is taken to be the
!  intended word, and the player is told.  Ambiguous guesses are left alone,
!  so the game never silently picks between two possibilities.
!
!  Called from BeforeParsing (see smart.h).

Array typo_src  -> 24;
Array typo_try  -> 24;
Array typo_best -> 24;
Array typo_new  -> 140;
Global typo_strict = 0;
Global typo_hits = 0;
Global typo_first = 0;
Global typo_blen = 0;

! Is typo_try (length l) a known word that makes sense at position pos?
[ TypoTry l pos w j;
    w = DictionaryLookup(typo_try, l);
    if (w == 0) return;
    if (typo_strict) {
        if (pos == 1) {
            if (((w->#dict_par1) & DICT_VERB) == 0) return;
        } else {
            ! after the verb, a word that is only a verb is unlikely
            if (((w->#dict_par1) & (DICT_VERB + DICT_NOUN)) == DICT_VERB) return;
        }
    }
    if (typo_hits == 0) {
        typo_hits = 1;
        typo_first = w;
        typo_blen = l;
        for (j = 0 : j < l : j++) typo_best->j = typo_try->j;
    }
    else if (w ~= typo_first)
        typo_hits = 2;
];

! Correct the word in typo_src (length n); returns the length of the
! correction (left in typo_best), or 0 if there is no single good guess.
[ TypoWord n pos pass i j k c;
    for (pass = 1 : pass <= 2 : pass++) {
        typo_strict = (pass == 1);

        ! two adjacent letters swapped
        typo_hits = 0;
        for (i = 0 : i < n - 1 : i++) {
            if (typo_src->i == typo_src->(i + 1)) continue;
            for (j = 0 : j < n : j++) typo_try->j = typo_src->j;
            typo_try->i = typo_src->(i + 1);
            typo_try->(i + 1) = typo_src->i;
            TypoTry(n, pos);
            if (typo_hits >= 2) break;
        }
        if (typo_hits == 1) return typo_blen;
        if (typo_hits >= 2) return 0;

        ! one letter too many
        if (n >= 4) {
            for (i = 0 : i < n : i++) {
                k = 0;
                for (j = 0 : j < n : j++)
                    if (j ~= i) { typo_try->k = typo_src->j; k++; }
                TypoTry(n - 1, pos);
                if (typo_hits >= 2) break;
            }
            if (typo_hits == 1) return typo_blen;
            if (typo_hits >= 2) return 0;
        }

        ! one letter missing
        if (n >= 4) {
            for (i = 0 : i <= n : i++) {
                for (c = 'a' : c <= 'z' : c++) {
                    k = 0;
                    for (j = 0 : j < i : j++) { typo_try->k = typo_src->j; k++; }
                    typo_try->k = c; k++;
                    for (j = i : j < n : j++) { typo_try->k = typo_src->j; k++; }
                    TypoTry(n + 1, pos);
                    if (typo_hits >= 2) break;
                }
                if (typo_hits >= 2) break;
            }
            if (typo_hits == 1) return typo_blen;
            if (typo_hits >= 2) return 0;
        }

        ! one letter wrong
        if (n >= 4) {
            for (i = 0 : i < n : i++) {
                for (c = 'a' : c <= 'z' : c++) {
                    if (c == typo_src->i) continue;
                    for (j = 0 : j < n : j++) typo_try->j = typo_src->j;
                    typo_try->i = c;
                    TypoTry(n, pos);
                    if (typo_hits >= 2) break;
                }
                if (typo_hits >= 2) break;
            }
            if (typo_hits == 1) return typo_blen;
            if (typo_hits >= 2) return 0;
        }
    }
    return 0;
];

! Look at every word of the command; rebuild the buffer if any were fixed
[ TypoFixAll nw i j k n c total any m;
    nw = NumberWords();
    total = 0;
    any = 0;
    for (i = 1 : i <= nw : i++) {
        n = WordLength(i);
        c = (WordAddress(i))->0;
        m = 0;
        k = 0;
        if (WordValue(i) == 0 && n >= 3 && n <= 14 && c >= 'a' && c <= 'z') {
            for (j = 0 : j < n : j++) typo_src->j = (WordAddress(i))->j;
            k = TypoWord(n, i);
            if (k > 0) m = 1;
        }
        if (total + n + k + 2 > 110) return;       ! too long to rebuild safely
        if (i > 1) { typo_new->total = ' '; total++; }
        if (m) {
            print "(Taking ~";
            for (j = 0 : j < n : j++) print (char) typo_src->j;
            print "~ to mean ~";
            for (j = 0 : j < k : j++) print (char) typo_best->j;
            print "~.)^";
            for (j = 0 : j < k : j++) { typo_new->total = typo_best->j; total++; }
            any = 1;
        } else {
            for (j = 0 : j < n : j++) {
                typo_new->total = (WordAddress(i))->j;
                total++;
            }
        }
    }
    if (any) {
        for (j = 0 : j < total : j++) buffer->(2 + j) = typo_new->j;
        buffer->1 = total;
        Tokenise__(buffer, parse);
    }
];
