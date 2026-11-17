!  hints_act3.h -- hint ladders for Act III (Guide HQ).

Puzzle h_badge "3.1 The visitor badge"
  with hint1 "The terminal wants a reason.",
       hint2 "Plain truth won't work.",
       hint3 "Make it sound administrative.",
       hint4 "TYPE ROUTINE AUDIT.",
       active [; return act == 3 && badge notin player && oldpass notin player; ],
       done [; return awarded->A_BADGE; ];

Puzzle h_scan "3.2 The scanner"
  with hint1 "The scanner objects to something you are carrying.",
       hint2 "Some things shouldn't go in.",
       hint3 "There's a cloakroom next to the security desk.",
       hint4 "GIVE JAR TO PIM, and GIVE KNIFE TO PIM, then go north.",
       active [; return act == 3 && (badge in player || oldpass in player) && ~~awarded->A_SCAN; ],
       done [; return awarded->A_SCAN; ];

Puzzle h_lift "3.3 The lifts"
  with hint1 "They have moods.",
       hint2 "One likes praise.",
       hint3 "The other would like sympathy about the first.",
       hint4 "COMPLIMENT CHEERFUL LIFT (or APOLOGISE TO SULLEN LIFT) then go UP.",
       active [; return act == 3 && awarded->A_SCAN && lift_ok == 0; ],
       done [; return lift_ok; ];

Puzzle h_zendra "3.4 Zendra"
  with hint1 "She knows Ford.",
       hint2 "Ask about the new Guide.",
       hint3 "Ask about the owners, and then about Ford.",
       hint4 "ASK ZENDRA ABOUT MK II, ABOUT INFINIDIM, ABOUT FORD.",
       active [; return act == 3 && location == Editorial && awarded->A_ZEN3 == 0; ],
       done [; return awarded->A_ZEN1 && awarded->A_ZEN2 && awarded->A_ZEN3; ];

Puzzle h_desk "3.5 Ford's desk"
  with hint1 "The drawer is locked.",
       hint2 "Zendra said something about numbers.",
       hint3 "Count the words of each draft of the Earth entry. The archive has them.",
       hint4 "The drafts run to 1, 2 and 5 words: DIAL 1 2 5.",
       active [; return act == 3 && location == FordOffice && desk_open == 0; ],
       done [; return desk_open; ];

Puzzle h_archive "3.6 The Earth entry"
  with hint1 "The Archive has the history of the entry.",
       hint2 "Search the stacks for Earth.",
       hint3 "A heavy ledger hides a key; a drawer hides a memorandum.",
       hint4 "SEARCH STACKS, OPEN LEDGER, UNLOCK DRAWER WITH KEY, OPEN DRAWER, READ MEMO.",
       active [; return act == 3 && location == Archive && awarded->A_MEMO == 0; ],
       done [; return awarded->A_MEMO; ];

Puzzle h_poem "3.7 Vogon poetry"
  with hint1 "Orlo wants poetry.",
       hint2 "It must be bad.",
       hint3 "Pick the ugliest lines: the biggest, squishiest words.",
       hint4 "COMPOSE POEM, then POEM 2 2 3 (or any combination that is bad enough).",
       active [; return act == 3 && location == Editorial && orlo_moved == 0; ],
       done [; return orlo_moved; ];

Puzzle h_meeting "3.8 Under the towel"
  with hint1 "The Vogons will see you.",
       hint2 "Remember the old rule about not being seen.",
       hint3 "Your towel.",
       hint4 "WEAR TOWEL (or PUT TOWEL ON HEAD), go north into the Boardroom, and WAIT.",
       active [; return act == 3 && location == Boardroom && heard_meeting == 0; ],
       done [; return heard_meeting; ];

Puzzle h_core "3.9 The paradox"
  with hint1 "The core flatters.",
       hint2 "Make it say what you don't want to hear.",
       hint3 "Ask it a question whose honest answer you dread.",
       hint4 "ASK CORE ABOUT THE WORST (or the truth).",
       active [; return act == 3 && location == ServerFloor && have_coords == 0; ],
       done [; return have_coords; ];

Puzzle h_roof "3.10 Out by the roof"
  with hint1 "The core was the last stop.",
       hint2 "Up.",
       hint3 "The roof has a ship.",
       hint4 "GO UP, ENTER SHIP.",
       active [; return act == 3 && have_coords; ];

