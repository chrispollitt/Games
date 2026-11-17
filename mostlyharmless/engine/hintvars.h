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

#ifndef EGGVARS_DONE;
Constant EGGVARS_DONE;
! Easter-egg state (see eggs.h), in an array to spare global variables
Constant EG_LAST  = 0;      ! the turn on which XYZZY last did something
Constant EG_COUNT = 1;      ! how many times it has
Constant EG_FLOYD = 2;      ! the game's own secret (Floyd in the full game): 0 unseen, 1 found, 2 parted
Constant EG_FORCE = 3;      ! debug builds: make the next XYZZY give this outcome
Constant EG_DICE  = 4;      ! 0 the Mish Mash dice are on (default), 1 off
Constant EG_RLAST = 5;      ! the turn on which the dice last rolled
Constant EG_RFLAG = 6;      ! this line contained the word random
Constant EG_RCOUNT = 7;     ! how many times they have rolled
Array eg --> 8;
#endif;
