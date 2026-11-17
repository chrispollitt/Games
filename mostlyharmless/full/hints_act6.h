!  hints_act6.h -- hint ladders for Act VI (Milliways).

Puzzle h_order "6.1 The order"
  with hint1 "The menu is interesting.",
       hint2 "Something on it is a little too eager.",
       hint3 "Politely decline the dish of the day, and order something else.",
       hint4 "ORDER SALAD (or soup, or a sandwich: anything that did not volunteer).",
       active [; return act == 6 && location == DiningHall && awarded->A_ORDER == 0; ],
       done [; return awarded->A_ORDER; ];

Puzzle h_marvin "6.2 Marvin"
  with hint1 "He is carrying a tray.",
       hint2 "Ask about his health.",
       hint3 "His left side, in particular.",
       hint4 "ASK MARVIN ABOUT HIS DIODES.",
       active [; return act == 6 && awarded->A_MARVIN == 0; ],
       done [; return awarded->A_MARVIN; ];

Puzzle h_tilt "6.3 The tilt"
  with hint1 "Something is wrong with the sketch.",
       hint2 "Compare it to her.",
       hint3 "Look at her properly, find something to draw with, then draw.",
       hint4 "EXAMINE FENCHURCH, TAKE PENCIL (on the terrace table), DRAW (with the sketch in your hand).",
       active [; return act == 6 && awarded->A_TILT == 0 && (location == Terrace || awarded->A_ORDER); ],
       done [; return awarded->A_TILT; ];

Puzzle h_toast "6.4 The toast"
  with hint1 "Everyone is here.",
       hint2 "A toast is traditional.",
       hint3 "On the terrace, with everybody in the room: Ford, Random, Tricia, Marvin.",
       hint4 "Go to the terrace with everyone and TOAST. (Ask Marvin about his diodes first, so that he will come.)",
       active [; return act == 6; ];

