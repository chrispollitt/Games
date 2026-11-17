!  full_gram.h -- grammar specific to THE LONG WAY ROUND.

Verb 'make' 'prepare' 'assemble' 'build' 'cook' 'create' 'concoct'
     'construct' 'fashion' 'whip'
    * topic                                 -> MakeFree;

Verb 'fly' 'launch' 'depart' 'takeoff'
    *                                       -> Fly
    * topic                                 -> Fly;

Verb 'sharpen' 'hone' 'whet'
    * noun 'with' noun                      -> Sharpen
    * noun 'on' noun                        -> Sharpen;

Extend 'cut' first
    * noun 'with' held                      -> Cut;

Verb meta 'about' 'credits' 'info'
    *                                       -> About;

! ---- Act II ----
Extend 'fill' first
    * noun                                  -> FillForm
    * 'in' noun                             -> FillForm
    * noun 'in'                             -> FillForm
    * noun 'with' noun                      -> FillForm;

Verb 'complete' 'sign'
    * noun                                  -> FillForm;

Verb 'write'
    * 'on' noun                             -> FillForm
    * 'on' noun 'with' noun                 -> FillForm;

Verb 'stamp'
    *                                       -> Stamp
    * noun                                  -> Stamp
    * noun 'with' noun                      -> Stamp;

! ---- Act III ----
Verb 'type'
    * topic                                 -> TypeText
    * topic 'on' noun                       -> TypeText
    * topic 'into' noun                     -> TypeText;

Verb 'compliment' 'praise' 'flatter' 'thank'
    * creature                              -> Compliment;

Verb 'apologise' 'apologize'
    * 'to' creature                         -> Apologise
    * 'to' creature 'for' topic             -> Apologise;

Verb 'dial'
    * topic                                 -> Dial
    * topic 'on' noun                       -> Dial;

Verb 'compose' 'recite'
    * 'poem'                                -> Compose
    * 'a' 'poem'                            -> Compose
    * 'poem' 'for' creature                 -> Compose
    *                                       -> Compose;

Verb 'poem' 'verse'
    * topic                                 -> PoemAct;

Extend 'put' first
    * noun 'on' 'head'                      -> Wear
    * noun 'on' 'my' 'head'                 -> Wear
    * noun 'over' 'head'                    -> Wear
    * noun 'over' 'my' 'head'               -> Wear;

! ---- Act IV / V ----
Extend 'tell' first
    * 'everyone' 'to' topic                 -> Warn
    * 'everybody' 'to' topic                -> Warn
    * 'all' 'to' topic                      -> Warn;

Verb 'warn'
    * 'everyone'                            -> Warn
    * 'everybody'                           -> Warn
    * 'all'                                 -> Warn;

Verb 'duck' 'crouch' 'cower'
    *                                       -> Duck;

Extend 'get' first
    * 'down'                                -> Duck;

Verb 'seat'
    *                                       -> Seat;

Extend 'sit' first
    *                                       -> Seat
    * 'down'                             -> Seat
    * 'at' noun                             -> Seat
    * 'down' 'at' noun                      -> Seat;

Verb 'swat' 'crush' 'squish' 'stomp'
    * noun                                  -> Attack;

Extend 'put' first
    * noun 'in' 'ear'                       -> FishEar
    * noun 'in' 'my' 'ear'                  -> FishEar
    * noun 'in' 'the' 'ear'                 -> FishEar
    * noun 'into' 'ear'                     -> FishEar;

! ---- Act VI ----
Verb 'order' 'request'
    * topic                                 -> Dine;

Verb 'draw' 'redraw' 'amend'
    *                                       -> Draw
    * topic                                 -> Draw;

Verb 'toast' 'cheer'
    *                                       -> Toast
    * topic                                 -> Toast;
