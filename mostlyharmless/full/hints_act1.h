!  hints_act1.h -- hint ladders for Act I (see docs/full/03-puzzles.md).
!  Declared in the order the player is likely to need them.

Puzzle h_wake "1.1 Wake and look around"
  with hint1 "The shelf holds things worth looking at.",
       hint2 "Look at both objects on the shelf.",
       hint3 "Take the jar; the fish matters later.",
       hint4 "EXAMINE SKETCH, TAKE JAR (or TAKE FISH).",
       active [; return act == 1 && location == Hut && phase == 0; ],
       done [; return awarded->A_SKETCH && awarded->A_FISH; ];

Puzzle h_berries "1.2 Pick the berries"
  with hint1 "Hob will want sourberries, and they grow on the slope south of the green.",
       hint2 "The bushes bite. Something you own is thick enough to protect your arms.",
       hint3 "Wear the gown, and carry the basket from your hut.",
       hint4 "TAKE BASKET (in your hut), WEAR GOWN, then TAKE BERRIES at the Orchard Slope.",
       active [; return act == 1 && phase == 0 && hob_fed == 0; ],
       done [; return awarded->A_BERRIES; ];

Puzzle h_trade "1.3 The smoker's price"
  with hint1 "Hob wants meat, and Dunna in the smokehouse has it, at a price.",
       hint2 "Her cleaver is blunt. Something would sharpen it.",
       hint3 "The river has stones that could sharpen blades. Look under things.",
       hint4 "Lift the flat rock on the River Bank, TAKE WHETSTONE, then SHARPEN CLEAVER WITH STONE in the smokehouse.",
       active [; return act == 1 && phase == 0 && hob_fed == 0 && awarded->A_BERRIES; ],
       done [; return awarded->A_TRADE; ];

Puzzle h_hob "1.4 Hob's order"
  with hint1 "Ask the customer.",
       hint2 "Hob has views about pickle.",
       hint3 "Meat and sourberries only. You'll need both in the hut.",
       hint4 "ASK HOB ABOUT LUNCH, put meat and berries on the board with the loaf, MAKE SANDWICH, GIVE SANDWICH TO HOB.",
       active [; return act == 1 && phase == 0 && hob_fed == 0 && awarded->A_BERRIES && awarded->A_TRADE; ],
       done [; return hob_fed; ];

Puzzle h_tem "1.5 Tem's order"
  with hint1 "Look at how Tem eats.",
       hint2 "He keeps touching his jaw.",
       hint3 "Soft bread and honey, no hard crust.",
       hint4 "MAKE A SANDWICH WITH HONEY AND CURD, then GIVE SANDWICH TO TEM at the lodge.",
       active [; return act == 1 && location == Lodge && tem_fed == 0; ],
       done [; return tem_fed; ];

Puzzle h_parcel "1.6 The parcel"
  with hint1 "Something is tied shut.",
       hint2 "You carry something that cuts.",
       hint3 "CUT CORD.",
       hint4 "CUT CORD WITH KNIFE, OPEN PARCEL, TAKE GUIDE, READ NOTE.",
       active [; return act == 1 && phase == 1; ],
       done [; return guide_revealed && awarded->A_NOTE; ];

Puzzle h_trillian "1.7 Trillian and Random"
  with hint1 "Trillian has something to tell you.",
       hint2 "Ask her about her daughter.",
       hint3 "Ask about Earth, and about Ford.",
       hint4 "ASK TRILLIAN ABOUT RANDOM, ABOUT EARTH, ABOUT FORD.",
       active [; return act == 1 && phase == 2; ];

Puzzle h_random "1.8 Feed Random"
  with hint1 "Random won't be questioned about food.",
       hint2 "Watch what she takes from your counter.",
       hint3 "She likes the sharp and sour things.",
       hint4 "MAKE A SANDWICH WITH BERRIES AND PICKLES, then GIVE SANDWICH TO RANDOM.",
       active [; return act == 1 && phase == 3; ],
       done [; return random_fed; ];

Puzzle h_ford "1.9 Ford's arrival"
  with hint1 "Ford will explain.",
       hint2 "Ask about the Guide.",
       hint3 "Ask about the Vogons.",
       hint4 "ASK FORD ABOUT GUIDE, ASK FORD ABOUT VOGONS.",
       active [; return act == 1 && phase == 4; ];

Puzzle h_pilot "1.10 The pilot's break"
  with hint1 "He's on his break.",
       hint2 "What does a man on his break like?",
       hint3 "He says he has strong views: one ingredient only.",
       hint4 "MAKE A SANDWICH WITH PICKLES, GIVE SANDWICH TO PILOT, ENTER SKIFF.",
       active [; return act == 1 && phase == 5; ],
       done [; return act_ended; ];
