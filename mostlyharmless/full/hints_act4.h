!  hints_act4.h -- hint ladders for Act IV (London).

Puzzle h_cordon "4.1 Past the cordon"
  with hint1 "Press are allowed through the tape.",
       hint2 "The satellite van in the press pen has spares.",
       hint3 "Take a credential from the van, and wear it.",
       hint4 "OPEN VAN (or SEARCH VAN) to take the credential, WEAR CREDENTIAL, then go north.",
       active [; return act == 4 && awarded->A_CORDON == 0; ],
       done [; return awarded->A_CORDON; ];

Puzzle h_trail "4.2 The trail"
  with hint1 "Random left a trail.",
       hint2 "Look for small disasters: a cup, a wrapper, a stub.",
       hint3 "The park, the High Road, the Tube; examine what you find.",
       hint4 "EXAMINE CUP in the park, KEBAB WRAPPER on the High Road, TICKET STUB in the Tube station; then go to the studio.",
       active [; return act == 4 && random_gone == 0 && Tricia.convinced == 0 && awarded->A_TRAIL3 == 0; ],
       done [; return awarded->A_TRAIL1 && awarded->A_TRAIL2 && awarded->A_TRAIL3; ];

Puzzle h_tricia "4.4 Tricia"
  with hint1 "She doesn't know you.",
       hint2 "You share a night she never had.",
       hint3 "The party, the man who walked in, what she studied.",
       hint4 "TELL TRICIA ABOUT THE PARTY IN ISLINGTON, THE MAN WITH TWO HEADS, THE ASTROPHYSICS (any two of the three).",
       active [; return act == 4 && Tricia.convinced == 0 && random_gone == 0 && (location == GreenRoom || location == StudioFloor || location == StudioLobby); ],
       done [; return Tricia.convinced; ];

Puzzle h_broadcast "4.6 The broadcast"
  with hint1 "The cameras are live.",
       hint2 "The autocue is the key.",
       hint3 "Tricia can give you air time, if she trusts you.",
       hint4 "ASK TRICIA ABOUT BROADCAST in the green room, then EXAMINE AUTOCUE on the studio floor.",
       active [; return act == 4 && Tricia.convinced && awarded->A_BROAD == 0 && random_caught == 0; ],
       done [; return awarded->A_BROAD; ];

Puzzle h_chase "4.8 The Underground"
  with hint1 "She went down.",
       hint2 "Look at the map.",
       hint3 "The map and the stub both say the same platform.",
       hint4 "Go to the Tube station and then west to Platform 2, and wait for her.",
       active [; return act == 4 && random_gone && random_caught == 0; ],
       done [; return random_caught; ];

Puzzle h_trust "4.3 Random's trust"
  with hint1 "Random doesn't trust you yet.",
       hint2 "Listen more than you speak.",
       hint3 "Ask about her. Say sorry. Don't read what isn't yours.",
       hint4 "ASK RANDOM ABOUT HERSELF, ASK RANDOM ABOUT HER MOTHER, APOLOGISE TO RANDOM, and GIVE DIARY TO RANDOM unread.",
       active [; return act == 4 && random_caught && trust < 4; ],
       done [; return trust >= 4; ];

Puzzle h_flat "4.7 The flat"
  with hint1 "Tricia's flat is where she keeps her past.",
       hint2 "A photograph and an earring.",
       hint3 "The flat is south of the studio lobby.",
       hint4 "From the Studio Lobby go south; EXAMINE PHOTO and EXAMINE EARRING.",
       active [; return act == 4 && random_caught && awarded->A_FLAT2 == 0; ],
       done [; return awarded->A_FLAT1 && awarded->A_FLAT2; ];

Puzzle h_pub "To the pub"
  with hint1 "Tricia and Random are with you; there is a pub up the road.",
       hint2 "Go to the High Road.",
       hint3 "Tricia will suggest the pub herself.",
       hint4 "GO TO THE HIGH ROAD with Tricia and Random, then NORTH.",
       active [; return act == 4 && random_caught && Tricia.convinced; ];

