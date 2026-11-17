!  hints_act5.h -- hint ladders for Act V (the Dancing Gnu).

Puzzle h_seat "5.1 The table"
  with hint1 "They need somewhere to sit.",
       hint2 "The corner table.",
       hint3 "Everyone has to be in the bar.",
       hint4 "SIT (or SIT AT TABLE) in the bar with Tricia, Random and Ford.",
       active [; return act == 5 && seated == 0 && gun_fired == 0; ],
       done [; return seated; ];

Puzzle h_prep "5.3 The pistol"
  with hint1 "Random has a dangerous toy.",
       hint2 "There is a manual somewhere.",
       hint3 "The snug has a card, and a warning is worth giving.",
       hint4 "Go west to the Snug, READ CARD, return, and TELL EVERYONE TO GET DOWN once Random puts the pistol on the table.",
       active [; return act == 5 && gun_fired == 0; ],
       done [; return gun_fired; ];

Puzzle h_greb "5.4 The Grebulons"
  with hint1 "There is a ship on the lawn.",
       hint2 "Ask the crew about the button.",
       hint3 "They are hungry. The barman does sandwiches.",
       hint4 "ASK BARMAN FOR A SANDWICH, go east through the yard to the lawn, GIVE SANDWICH TO GREBULONS and ASK GREBULONS ABOUT THE BUTTON.",
       active [; return act == 5 && awarded->A_GREB2 == 0 && gun_fired == 0; ],
       done [; return awarded->A_GREB1 && awarded->A_GREB2; ];

Puzzle h_matchbook "5.2 The matchbook"
  with hint1 "Something small is lying near the corner table.",
       hint2 "After the commotion.",
       hint3 "It's a matchbook.",
       hint4 "TAKE MATCHBOOK, then EXAMINE MATCHBOOK.",
       active [; return act == 5 && gun_fired && matchbook_read == 0; ],
       done [; return matchbook_read; ];

Puzzle h_choice "5.6 The choice"
  with hint1 "This is the moment.",
       hint2 "What did the fish offer? And has the cube been steering you?",
       hint3 "Do the people who matter have to be here, and do they trust you?",
       hint4 "PUT FISH IN EAR with Random, Tricia and Ford present, Random's trust at least 4 and the cube's compliance low.",
       active [; return act == 5 && gun_fired && matchbook_read; ];

