# Full game — story, cast, systems, style

Title: **MOSTLY HARMLESS: THE LONG WAY ROUND** (decided 2026-11-16: "The Long Way Round" is the subtitle; the mini-game is "Last Orders on Lamuella").
Target: ~42 rooms, ~400 points, 8–15 hours. Built on the shared `engine/`. Z8 story file.

Companion docs: `02-map.md`, `03-puzzles.md`, `04-cast-and-objects.md`, `05-score-and-walkthrough.md`.

Fidelity rule (from the user): every HHGG medium diverges from the others. The main beats and the characters stay
recognisable; the rest is ours. Anything marked **[invented]** is new. Book details were confirmed by the user on 2026-11-16
(see section 3); decisions are recorded in `05-score-and-walkthrough.md`.

---

## 1. Premise

Arthur Dent has settled on Lamuella as the village sandwich maker. Fenchurch vanished in a hyperspace jump and he has
stopped looking. One morning a parcel arrives from Ford Prefect containing something that should not exist: the
**Hitchhiker's Guide Mk II**. On the same day Trillian turns up with a daughter Arthur did not know he had. The Guide's
new owners are Vogons, the Mk II is not really a guidebook (it is patient, courteous and entirely on its own side), and the
daughter, **Random**, is about to steal it.

The player's job, in one line: *keep a very small cube and a very angry girl from becoming the end of every Earth that has
ever existed, mostly by making sandwiches and asking the right person the right question.*

## 2. Shape in six acts

| Act | Where | Rooms | Pts | What it is |
|---|---|---|---|---|
| I | Lamuella | 8 | 60 | A gentle, comic day in the life; foraging and sandwiches; the parcel; Trillian and Random; Ford; the theft; the pilot. Teaches the game's rules. |
| II | Hub Station (spaceport) | 6 | 50 | Bureaucracy puzzle act. Customs, lost property, a ticket machine, a departure gate. First proof that the Mk II is "helping" too much. |
| III | Guide HQ, Ursa Minor | 11 | 100 | The heist/infiltration act. Ford's old workplace is run by Vogons. Learn the plan; get the coordinates. The big set-piece act. |
| IV | London (the other Earth) | 8 | 90 | Social act. Press cordon, TV studio, Tricia McMillan, Random, the Underground. Agrajag's trail begins. |
| V | The Dancing Gnu and the lawn | 5 | 60 | The endgame. Laser pistol, matchbook, Agrajag, the Grebulons, the choice. Two endings branch here. |
| VI | The Restaurant at the End of the Universe | 4 | 40 | Epilogue; reached only by the radio-play ending. Marvin, Fenchurch, dinner. |

Total 42 rooms, 400 points (the book ending tops out at 360).

## 3. Book beats and what we do with them

| Beat | Source | Our treatment |
|---|---|---|
| Arthur is the sandwich maker on Lamuella | book (certain) | Act I opens here; sandwich system is the game's signature mechanic |
| Fenchurch vanished in a jump | book (certain) | The pencil sketch; the Act VI payoff |
| Ford mails Arthur the Guide Mk II | book (certain) | Act I parcel |
| Trillian leaves Random with Arthur | book (certain) | Act I |
| The Guide's new owners are Vogons | book (confirmed: Vogons are the new owners) | Act III HQ; the company with "six names and one address" |
| Mk II reads and anticipates the reader | book (confirmed; menace is central) | Core mechanic: the cube answers before you ask, and is not on your side (see section 5) |
| Random takes the Guide and goes to Earth | book (certain) | The Act I theft; Acts II–IV are the pursuit |
| Tricia McMillan is Trillian's counterpart on the other Earth, a TV presenter | book (confirmed: from a parallel Earth) | Act IV |
| Agrajag, endlessly killed by Arthur, finally gets his chance | earlier book + book 5 | A thread through IV and V; the matchbook |
| A pub scene with a laser pistol goes wrong | book (confirmed) | Act V; the accident always happens |
| Grebulons reduce every Earth at once | book (confirmed) | Act V lawn scene; the button |
| The Earth is destroyed, in every version | book (certain) | Book ending |
| Everyone is rescued, ends at Milliways | BBC radio adaptation | Radio ending and Act VI |

Invented: the whole of Hub Station, nearly all Lamuella geography beyond the village and hut (river, orchard, smokehouse,
lodge), the HQ floor plan and its vogon-poetry guard, the studio and Underground in Act IV, the epilogue puzzle.

## 4. Cast

| Character | Role |
|---|---|
| **Arthur Dent** (you) | Sandwich maker, reluctant hero, owner of a dressing gown and one towel (misplaced). |
| **Ford Prefect** | Friend, Guide researcher, mostly useful, often wrong about dates. A *follower* in Acts I–V with a long-run ability to get you into trouble and out of it. |
| **Random Dent** | Trillian and Arthur's daughter. Furious, brilliant, the thief. Has a **trust** score the player builds across the game (see §6). Follower on and off. |
| **Trillian / Tricia McMillan** | Trillian in Act I, her Earth counterpart in Act IV. |
| **Agrajag** | The man in the raincoat. A funny, tragic antagonist. |
| **Vogons** | The Guide's new editors and its guards. Bureaucratic, poetry-loving, pitiless. |
| **Grebulons** | Ship crew with a broken memory and a star chart. |
| **Marvin, Zaphod, Fenchurch** | Fenchurch and Marvin in Act VI; **a few cameos earlier (decided)**: Marvin as the Hub Station announcement voice and a sulking service robot at HQ; Zaphod in a margin note on Ford's old proofs, a London tabloid headline, and a star on the Grebulons' chart **[invented]**. |
| **Lamuellans** | Hob (sandwich regular), Dunna (smoker), Tem (the Reeve), others **[invented]**. |
| **The Mk II** | The cube. A character in its own right (see §5). |

## 5. Systems

**Sandwiches.** Ingredients: meat, sourberries, pickled roots, plus new ones in later acts (honey, herb curd, flatbread). Each
customer has a likes/dislikes sheet; clues are discoverable. The free-form parser (`MAKE HOB THE USUAL`, `WITHOUT PICKLES`)
stays. Sandwiches are a currency for favours across all six acts.

**The Guide Mk II (the cube) — menacing by design (decided: this is central).** It never lies and is never rude. It is warm,
attentive and wrong for you: it greets you by name, answers questions before you ask them, offers to save you the trouble,
and every convenience it provides moves you and Random along the rails it wants (all roads to one room on one afternoon).
The menace escalates by act:

| Act | What the cube is like |
|---|---|
| I | Charming. "You appear to be happy. This has been noted." |
| II | Answers unasked questions, files your paperwork, knows things it should not. |
| III | Shows what it is: a funnel. Starts talking about Random as if she were a component. |
| IV | Edits small things (a sign, a photograph, a timetable) to keep Random moving. Begins to count. |
| V | Plays its part. The count ends. |

**Compliance (0–6)** is the mechanic. Accepting a *cube shortcut* (letting it file the clearance, forge a credential, give you the
combination, point out the trail, and so on; about nine are on offer, listed at the end of `03-puzzles.md`) gives the full puzzle
points but adds 1. Solving the same puzzle yourself adds nothing. At 3 the cube's tone changes ("Good."); at 4 or more it
starts to answer for you, Random's trust is capped at 3, and the radio ending is closed. The out-of-story `/HINT` never costs
compliance, so a stuck player is never punished for asking. In Easy mode the cube volunteers its shortcuts; in Faithful mode
it only offers them once you have been stuck for a while. Guide entries are collectible reading (1 point each, about 20).

**The towel.** Lost on the way in (Act I), recovered in Act II, and used in Acts III and IV (disguise, ID, a surprising
amount of drying).

**The Babel fish.** In the jar from the first room. It has to stay with the player for the radio ending. Act III's security
scan forces an inventory decision (cloakroom ticket) so that the fish can never be lost.

**Followers.** Ford and Random travel with you when they should. They have their own idle lines and will comment on rooms.
They never block progress and can be told to wait or go.

**Random's trust (0–5).** Raised by: feeding her correctly, returning her things, listening (ASK her about herself), not
reading her diary, getting a drawing right. Lowered by lying, stealing her cube. The radio ending requires trust 4 or more
**and** cube compliance 3 or less.

**No dead ends.** The game checks critical items (fish, towel, cube, knife, tickets) and respawns or re-offers them.
Anything lost can be got back (at a cost in points or time, never in winnability).

## 6. Hints and difficulty

Each puzzle in `03-puzzles.md` has a four-step ladder. `/HINT` finds the puzzle you are working on and gives the next rung.

| Setting | Easy | Average | Faithful |
|---|---|---|---|
| Hints | free; cube volunteers nudges | `/HINT` on request | `/HINT` rationed to 1 rung per puzzle |
| Implicit actions | on (take, open, unlock when obvious) | on | off |
| Timed events | slow | normal | tight |
| Puzzle assists | listed as "E" in the chart | none | extra red herrings listed as "F" |
| Dead-end protection | full | full | full |

## 7. Style guide

- Tone: dry, warm, a little sad. Funny from the situation and the angle, not from random silliness.
- Voice: second person, past-tense narration where the story moves, present tense in room descriptions.
- Length: room descriptions 3–6 sentences; first visit longer than the repeat look.
- No quotation of the novels or the Infocom game; no reproduced jokes. Original prose only.
- Out-of-story facts stay in `[Out of the story: ...]` brackets (engine `lore_real.h`).
- NPCs speak in short paragraphs; no more than three sentences without an action beat.
- Every puzzle has at least two ways in (the parser accepts synonyms and loose phrasing).
- Every NPC has at least eight topics and a fallback that stays in character.

## 8. Technical notes (full game)

- `full/full.inf` plus one file per act: `act1.h` … `act6.h`; shared `sandwich.h`, `guide.h`, `followers.h`, `hints.h`.
- Target Z8 (`inform6 -v8 -e` abbreviations on). Budget: ~330 KB raw text in, ~200 KB packed, ~100 KB code.
- Debug verbs: `ACT n` (jump), `SCORE n`, `GIVE obj`, `TRUST n`, `SHOWPUZZLE`.
- Tests: one `.cmd` per act plus two ending walkthroughs plus a parser list; `tests/check.sh full`.
