# Full game — score, endings, walkthrough outline, open questions

## Score (400)

| Act | Points | Notes |
|---|---|---|
| I Lamuella | 60 | 1.1–1.10 |
| II Hub Station | 50 | 2.1–2.7 |
| III Guide HQ | 100 | 3.1–3.10 |
| IV London | 90 | 4.1–4.9 |
| V Gnu and lawn | 60 | 5.1–5.6 |
| VI Milliways | 40 | radio ending only |

Rank ladder: Passenger Without a Towel (0–49) → Sandwich Maker (50–119) → Reluctant Hitchhiker (120–219) → Seasoned Hitchhiker (220–319) →
Mostly Harmless (320–399) → Remarkably Harmless (400).

## Endings

1. **Book ending A — "the books are balanced".** Matchbook read, no fish. Earth is demolished. Ends at Act V (max 360 less Act V's 10 for 5.6).
2. **Book ending B — "again".** Matchbook not read. Earth is demolished. Ends at Act V.
3. **Radio ending.** Fish in ear with the right people present, Random's trust ≥ 4 and cube compliance ≤ 3. Continues into Act VI; max 400.

## Walkthrough outline (spine, about 360 moves)

| Segment | Beats |
|---|---|
| Act I (about 70) | wake, jar, sketch; basket + gown; orchard berries; river whetstone; smokehouse trade; Hob's sandwich; Tem's soft sandwich; the parcel; Trillian and Random; Random's sandwich; Ford; the pilot's sandwich; skiff |
| Act II (about 45) | Lost Property (pen, towel); Concourse (coin, ticket); Customs form + Ford distraction; Clearance honey sandwich; Guide entries |
| Act III (about 90) | Plaza; Lobby badge; Cloakroom tag; scanner; lifts; Zendra; Ford's office combination; Archive memorandum; Boardroom under the towel; Server Floor poem; paradox; roof |
| Act IV (about 85) | Cordon credential; Press Pen; High Road; Tube; Studio Lobby; Studio Floor; Green Room; Tricia; flat; Random's trust beats; Tube chase; return |
| Act V (about 35) | Bar seating; matchbook; Snug and the pistol; Grebulons on the lawn; fish choice |
| Act VI (about 25) | Dining Hall; Marvin; the tilt; the toast |

## Test plan hooks

- `tests/full/actN.cmd` jumps with `ACT n` (debug) and plays the act's spine.
- `tests/full/book_a.cmd`, `book_b.cmd`, `radio.cmd`: full spines.
- `tests/full/parser.cmd`: typo, filler, question and slash cases from the mini plus full-game words.
- A soak test: random legal commands for 1000 moves per act, looking for crashes only.

## Decisions (user, 2026-11-16)

1. **Pistol accident:** always happens, as in the book. The player's agency is preparing for it (puzzle 5.3), not preventing it.
2. **Third ending:** none. Two endings only (with the two book variants described above).
3. **Cameos:** a few. Marvin and Zaphod appear lightly before the finale (see `03-puzzles.md`, end).
4. **The Mk II is menacing; that is central.** Implemented as the cube's escalating behaviour and the **compliance** mechanic
   (`01-story-and-systems.md`, section 5); the radio ending needs compliance ≤ 3 as well as Random's trust ≥ 4.
5. **Title:** MOSTLY HARMLESS: THE LONG WAY ROUND (the mini-game is MOSTLY HARMLESS: LAST ORDERS ON LAMUELLA).
6. **Book details:** Vogons are the Guide's new owners; the Mk II reads and anticipates the reader; Tricia McMillan is a TV presenter on a parallel
   Earth; the pub scene with the laser pistol and the Grebulon button are right as designed; the Chief Editor stays unnamed.
7. **Size:** about 42 rooms, 400 points, 8–15 hours, about 55,000 words: confirmed.

## Next

M3: hint engine and `/DIFFICULTY` skeleton, including the compliance counter, tested in the mini-game first.
