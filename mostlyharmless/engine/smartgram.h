!  smartgram.h -- looser grammar shared by the MOSTLY HARMLESS games.
!  Included from the game file after the standard Grammar. Game-specific verbs (make, fly, ...) live in the game's own grammar file.

! Questions of every kind
Verb 'who' 'whos' 'what' 'whats' 'where' 'wheres' 'when' 'why' 'how' 'whom'
    * topic                                 -> Question;

Verb 'explain' 'define' 'research' 'lookup'
    * topic                                 -> Question;

Extend 'tell' first
    * 'me' 'about' topic                    -> Question
    * 'us' 'about' topic                    -> Question
    * 'me' topic                            -> Question;

Extend 'tell' last
    * creature topic                        -> Tell;

Extend 'look' last
    * 'up' topic                            -> Question;

! Conversation
Extend 'ask' last
    * creature topic                        -> Ask
    * creature                              -> Talk;

Verb 'talk'
    * 'to' creature 'about' topic           -> Ask
    * 'with' creature 'about' topic         -> Ask
    * 'to' creature                         -> Talk
    * 'with' creature                       -> Talk
    * creature                              -> Talk
    * 'about' topic                         -> Question;

Extend 'answer' first
    * 'to' creature 'about' topic           -> Ask
    * 'to' creature                         -> Talk
    * 'with' creature                       -> Talk;

Verb 'hello' 'hi' 'hullo' 'greet' 'hail'
    *                                       -> Hello
    * creature                              -> Hello
    * 'to' creature                         -> Hello;

! Getting about
Extend 'go' first
    * 'to' topic                            -> WalkTo
    * 'towards' topic                       -> WalkTo
    * 'toward' topic                        -> WalkTo
    * 'back' 'to' topic                     -> WalkTo;

Verb 'head' 'visit' 'return'
    * 'to' topic                            -> WalkTo
    * 'back' 'to' topic                     -> WalkTo
    * topic                                 -> WalkTo;

! Nudges
! (out-of-story commands are reached with a leading slash; see slash.h)
Verb meta 'hint' 'hints' 'clue' 'clues'
    *                                       -> Hint
    * 'more'                                -> Hint
    * 'back'                                -> HintBack
    * 'again'                               -> HintBack
    * 'repeat'                              -> HintBack;

Verb meta 'difficulty'
    *                                       -> Difficulty
    * topic                                 -> DifficultySet;

Verb meta 'help'
    *                                       -> Help;

Verb meta 'slashmiss'
    *                                       -> SlashMiss;

! /MACRO: record and replay command sequences (macro.h)
Verb meta 'macro'
    *                                       -> MacroList
    * 'list'                                -> MacroList
    * 'stop'                                -> MacroStop
    * 'record' topic                        -> MacroRecord
    * 'play' topic                          -> MacroPlay
    * 'show' topic                          -> MacroShow
    * 'delete' topic                        -> MacroDelete
    * topic                                 -> MacroPlay;

! Overviews for newcomers (newbie.h)
Verb meta 'intro' 'introduction' 'tutorial' 'newbie'
    *                                       -> Intro;

Verb meta 'hhgg' 'background'
    *                                       -> Hhgg;
