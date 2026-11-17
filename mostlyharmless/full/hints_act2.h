!  hints_act2.h -- hint ladders for Act II (Hub Station).

Puzzle h_form "2.1 The form"
  with hint1 "The form needs filling in, and you need a pen.",
       hint2 "There is a pen on a chain by the forms in the Customs Hall.",
       hint3 "Take the form, then fill it in at the Customs counter.",
       hint4 "TAKE FORM, then FILL FORM (the pen is chained in the Customs Hall). The stamp comes later, when Gruba looks away.",
       active [; return act == 2 && form_state < 2; ],
       done [; return form_state >= 2; ];

Puzzle h_towel "2.3 The towel"
  with hint1 "The Attendant wants a description.",
       hint2 "Remember what was on the towel.",
       hint3 "It had a stain. Ford knows the story.",
       hint4 "ASK ATTENDANT ABOUT NORWAY (or the fjords). Ford says the stain was Slartibartfast's best coastline.",
       active [; return act == 2 && location == LostProp && towel notin player; ],
       done [; return awarded->A_TOWEL; ];

Puzzle h_distract "2.4 Ford's distraction / 2.1 stamp"
  with hint1 "Let Ford do the talking, once the form is ready.",
       hint2 "Wait in the Customs Hall with the filled form.",
       hint3 "Watch Gruba's eyes: when Ford says 'and another thing', you have a moment.",
       hint4 "Stand in Customs with the completed form and WAIT until Gruba turns to the window; then STAMP FORM.",
       active [; return act == 2 && form_state == 2; ],
       done [; return form_state >= 3; ];

Puzzle h_coin "2.2 The coin"
  with hint1 "The machine is picky.",
       hint2 "What do you have that nobody recognises?",
       hint3 "Hob paid you in something.",
       hint4 "PUT COIN IN MACHINE.",
       active [; return act == 2 && location == Concourse && ticket notin player; ],
       done [; return awarded->A_COIN; ];

Puzzle h_clear "2.5 Clearance"
  with hint1 "The desk wants something.",
       hint2 "The clerk looks hungry. The cube has an offer, but is it a good one?",
       hint3 "The clerk likes sandwiches, and sweet ones.",
       hint4 "Get a honey sandwich (Lost Property has one that nobody claimed) and GIVE SANDWICH TO CLERK.",
       active [; return act == 2 && location == Gate && cleared == 0; ],
       done [; return cleared; ];

Puzzle h_suspect "2.6 First suspicion"
  with hint1 "The cube said something odd in the Customs Hall.",
       hint2 "Look back at what it wrote before Gruba spoke.",
       hint3 "Ask Ford about the cube.",
       hint4 "ASK FORD ABOUT CUBE after the cube has predicted Gruba's words.",
       active [; return act == 2 && cube_knew && awarded->A_SUSPECT == 0; ],
       done [; return awarded->A_SUSPECT; ];

Puzzle h_act2 "Next: the gate"
  with hint1 "You have what you need for the next stage, I think.",
       hint2 "Go to the Departure Gate.",
       hint3 "ENTER the ship at the gate.",
       hint4 "GO EAST from the Concourse and ENTER SHIP.",
       active [; return act == 2 && form_state == 3 && ticket in player && cleared; ];

