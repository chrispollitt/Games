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

! /MACRO state (see macro.h), kept in an array because the Z-machine has only
! 233 global variables:  mv-->0 state (0 idle, 1 recording, 2 playing),
! 1 macro in use, 2 next command to play, 3 recorded command awaiting its
! snapshot (-1 none), 4 a replayed command could not be parsed
Constant MV_STATE = 0;
Constant MV_CUR   = 1;
Constant MV_POS   = 2;
Constant MV_LAST  = 3;
Constant MV_FAIL  = 4;
Constant MV_SCORE = 5;      ! the score before the command now running
Array mv --> 6;

#endif;
