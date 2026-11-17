!  floyd.h -- a hidden cameo: a small robot found in the heap at Lost Property
!  (a nod to the robot of Infocom's Planetfall).  He tags along through Hub
!  Station, stays behind when the ship goes, and turns up again in the kitchen
!  at Milliways.  State: eg-->EG_FLOYD = 0 unseen, 1 following, 2 parted.
!  No points, no puzzle role.

Object Floyd "Floyd"
  with name 'floyd' 'robot' 'small' 'dented',
       description [;
         if (location == Kitchen)
             "Floyd sits on a stool at the chef's elbow, handing him onions one at a time with tremendous care. He has the look of a small robot who has been given an important job and intends to be worthy of it.";
         "A small, dented robot about the height of a ten-year-old, with a flexible head, two eyes that do not quite match, and the posture of someone who is about to ask you a question. He is, by any measure, delighted to be here.";
       ],
       initial [;
         if (location == Kitchen) {
             print "Floyd sits on a stool at the chef's elbow, handing him onions. ";
             rtrue;
         }
         print "Floyd bounces on the spot beside you, looking at everything. ";
         rtrue;
       ],
       life [;
         Ask, Answer, Tell:
           if (MishAsk()) rtrue;
           if (TopicAny('hide', 'seek', 'game', 'play', 'hiding'))
               "Floyd covers his eyes with both hands, counts to eleven because he forgot ten, and then peeks through his fingers to see whether you have gone. ~Oh boy,~ he whispers. ~You didn't move! You're very good.~";
           if (TopicAny('cube', 'guide', 'mk', 'ii'))
               "Floyd looks at your pocket and takes a small step backward. ~That one says nice things,~ he says. ~Nice things aren't the same as being nice. I'm only a robot, but I know that.~";
           if (TopicAny('marvin', 'robot', 'robots'))
               "~Marvin?~ says Floyd. ~I haven't met him. I'd like to. I think he'd be sad, and I'm good at that. I stay and I don't say anything clever.~";
           if (TopicAny('sandwich', 'sandwiches', 'lunch', 'food'))
               "~Do you have anything electrical?~ says Floyd hopefully. ~No? That's all right. I like watching people eat. They look so pleased with themselves.~";
           if (TopicAny('towel'))
               "~A towel,~ says Floyd, and gives it his full attention. ~It's very soft. It's very good at being a towel. I think that's a nice thing to be.~";
           if (TopicAny('fenchurch', 'sketch', 'girl'))
               "Floyd looks at you for a moment longer than usual. ~You're missing someone,~ he says. ~I can tell. I'd be missing someone too, if I had one. Oh boy. I hope you find her.~";
           if (TopicAny('ford', 'prefect'))
               "~Ford smells like a friend who's been in trouble,~ says Floyd, approvingly.";
           if (TopicAny('arthur', 'you', 'dent', 'name', 'who'))
               "~I'm Floyd,~ says Floyd. ~I'm a robot. I was a helper, once, and then I wasn't needed, and then I was in a box, and then you came. I think you're what the box was for.~";
           if (TopicAny('planetfall', 'infocom', 'game'))
               "[Out of the story: Floyd is a nod to the robot of Infocom's Planetfall (1983, by Steve Meretzky), one of the best-loved companions in interactive fiction. This one is original, and is only saying hello.]";
           "Floyd tilts his head one way and then the other. ~Oh boy,~ he says, which seems to cover it.";
       ],
  has animate male proper;

! Third search of the heap finds him
[ FloydFound;
    eg-->EG_FLOYD = 1;
    move Floyd to LostProp;
    print "Behind the trumpet, among the single gloves, a battered crate lid lifts a quarter of an inch, and then all at once. Out climbs a small, dented robot, who looks at you, then at the Attendant, then at you again with enormous joy.^^";
    print "~You found me!~ he says. ~I've been hiding since before the Collapse of Something! Nobody's ever looked this far! I'm Floyd. Can I come? I'll be very quiet. (I won't. But I'll try.)~^^";
    print "The Attendant, from his counter, does not look up. ~He's not mine,~ he says. ~I thought he was part of the shelves.~^^";
    print "[Out of the story: Floyd is a nod to the robot of Infocom's Planetfall; type ASK FLOYD ABOUT PLANETFALL for a note.]^";
];

! Called from FollowStep each turn (Hub Station only)
[ FloydFollow;
    if (eg-->EG_FLOYD ~= 1 || act ~= 2) return;
    if (location == SkiffCabin) return;
    if (Floyd notin location) {
        move Floyd to location;
        print "^Floyd skids in after you, delighted with everything.^";
    }
];

! The ship leaves; Floyd stays
[ FloydParts;
    if (eg-->EG_FLOYD ~= 1) return;
    eg-->EG_FLOYD = 2;
    move Floyd to LostProp;
    print "Floyd stops at the edge of the gate and waves with his whole arm. ~I'll stay and mind the heap!~ he calls. ~Somebody ought to. Oh boy. Good luck!~^^";
];

! At Milliways he turns up again in the kitchen, if you found him
[ FloydKitchen;
    if (eg-->EG_FLOYD == 2) move Floyd to Kitchen;
];
