!  hintvars.h -- the constants and variables of hints.h.
!  A game whose own objects need them before the library is loaded
!  includes this file first; hints.h includes it again harmlessly.

#ifndef HINTVARS_DONE;
Constant HINTVARS_DONE;

Constant DIFF_EASY     = 1;
Constant DIFF_AVERAGE  = 2;
Constant DIFF_FAITHFUL = 3;

Global difficulty     = 2;
Global compliance     = 0;
Global cube_warned    = 0;
Global hint_idle      = 0;
Global hint_lastscore = 0;
#endif;
