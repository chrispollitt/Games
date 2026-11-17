# MOSTLY HARMLESS — two-version plan

Status: **approved 2026-11-16; M0 complete.** Decisions already made by the user:

- Two products: the existing **mini-game** and a **full version** to stand beside Infocom's 1984 HHGG.
- Full version = *Mostly Harmless, expanded*: the book's plot as the spine, with new locations, characters and puzzles in the book's spirit.
- Target size ≈ **40 rooms, 8–15 hours**.
- Difficulty: a really good **progressive hint system**, and/or three levels (easy / average / faithful). See section 5.

- **Fidelity (user guidance, 2026-11-16):** every HHGG medium diverges from the others, so the full version has artistic licence to depart from the novel, especially where it helps interactive fiction. The main beats and the characters must stay recognisable.

Everything below that is not on that list is a proposal.

---

## 1. Shape of the project

```
mostlyharmless/
  engine/      shared by both games
    smart.h        input cleanup, synonyms, friendly errors, questions
    smartgram.h    looser grammar
    typo.h         typo forgiveness
    slash.h        /commands
    lore_real.h    out-of-story facts (Douglas Adams, Infocom, ...)   [split out of smart.h]
    hints.h        progressive hint engine                            [new]
    debug.h        debug-only verbs (jump to act, give item, ...)     [new]
  mini/        today's game, moved here: mini.inf, lore_mini.h
  full/        the big game: full.inf + one file per act
  tests/       walkthroughs + expected transcripts + test.sh
  docs/        design notes per act, puzzle charts, PLAN.md
```

Why: the parser work (the part you care most about) is already generic. Only `Lore()` in `smart.h` mixes
generic and game-specific answers, and `MakeFreeSub`/`HintSub` are sandwich/mini specific. Splitting those is the
first job. Both games then share one parser, so every improvement benefits both.

The mini-game stays as it is: it becomes the short "demo/tutorial" edition (and a good regression test).

## 2. Platform limits

- Mini is 138 KB as Z5 (limit 256 KB). A 40-room game with ~40–60k words of prose will not fit in Z5.
- Plan: build the full game as **Z8** (512 KB), using Inform's `-e`/abbreviation optimisation. nfrotz plays Z8.
- Fallback if we outgrow it: **Glulx**. That changes the parser hooks (`buffer`/`parse` layout), so decide early.
  Recommendation: stay with Z8 unless the first two acts already use more than ~200 KB.
- Inform 6 limits to design around: 7-argument routine calls, 9-character dictionary resolution (already handled),
  about 233 global variables (use properties/arrays for state), case-insensitive identifiers.

## 3. Story structure (proposal)

Book beats stay in order; new material expands each beat into a place you can explore. Rooms are targets.
Items marked *(book)* come from the novel's plot; the rest are **invented** and need your OK.

| Act | Setting | Rooms | Main beats |
|---|---|---|---|
| 1 | Lamuella | 7 | Arthur's sandwich-making life *(book)*; village, hunting grounds, river, hills. Warm-up puzzles (sandwich orders, fixing a problem for the villagers). Ford's package and Guide Mk II arrive *(book)*. Trillian & Random *(book)*. |
| 2 | Journey out | 5 | Mail skiff to a port-of-call; a spaceport with bureaucracy; first look at Vogon interest in the Guide. |
| 3 | Guide Mk II headquarters | 9 | Ford's office building *(book)*; floors, lifts, the Vogon "editors"; the Mk II's inner workings; learning what it really does. |
| 4 | Earth (the other one) | 9 | London, Tricia McMillan's world *(book)*; TV studio, flat, streets; the missing-Fenchurch thread *(book)*; Random's trouble. |
| 5 | A chase across space | 5 | Random takes the Mk II *(book)*; pursuit; Marvin and Milliways appear in the radio-play thread. |
| 6 | The Dancing Gnu & endgame | 5 | The pub *(book)*, the Grebulons, the laser, the matchbook, then the fork: book ending vs radio ending. |

Total ≈ 40 rooms. Act lengths are rough: Act 3 and Act 4 carry the weight.

Endings stay as now: **book ending** (Earth demolished, with/without the matchbook) and **radio ending**
(Babel fish to the Restaurant at the End of the Universe). Full version may add a third, harder-earned variant.

**Scoring:** 400 points like the Infocom original (rank ladder scaled up). Mini stays at 100.

**Prose:** all original text. The Guide entries and humour are written fresh; real-world facts remain
"[Out of the story: ...]". No quotation of the novels or the Infocom game.

## 4. Puzzle design rules

1. Every puzzle has a one-line statement, an intended solution, 2–3 alternates the parser should accept, and a
   hint ladder (see 5).
2. **No unwinnable states.** Anything that could be lost or destroyed is either respawned or checked before it
   matters. (Infocom's famously cruel dead ends are explicitly left out.)
3. Humour comes from the situation and the Guide entries, not from guessing the author's word.
4. Each act has a short walkthrough and a golden transcript (section 6).
5. One recurring mechanic per act so it feels different (Act 1 cooking, Act 3 building access, Act 4 social
   engineering, Act 5 timing, Act 6 the fish/matchbook endgame already built).

## 5. Hints and difficulty

You said either 3 levels or a really good progressive hint system. My recommendation: **do the hint system
properly and let it carry the difficulty levels**, so three levels cost little extra.

**Progressive hint engine (`hints.h`):**
- Hints are organised per puzzle as a ladder: nudge → clearer → nearly the answer → the answer.
- `/HINT` finds the puzzle you are most likely stuck on (current location, items held, flags) and gives the next
  rung, remembering how far you went. `/HINT MORE` goes one rung further; `/HINT BACK` re-reads.
- Tracks points lost to hints only if you choose that (optional "pure" mode).

**Three levels (`/DIFFICULTY`), kept deliberately simple — they change *support*, not the world:**

| | Easy | Average | Faithful |
|---|---|---|---|
| Hints | free, unlimited, proactive nudges after you idle | on request | rationed or off |
| Parser help | implicit take/open, objects named in prompts | normal | strict-ish: fewer "(first taking...)" |
| Puzzle assists | a few obvious shortcuts enabled | normal | those shortcuts off, a couple of extra red herrings |
| Danger | forgiving timers | normal | tighter timers |

Only a small number of puzzles get an "easy" or "faithful" variant; the rest are identical. I'd decide those
per puzzle in the act design notes, so testing triples only on a handful of spots.

Open choice: ship hints first and add the difficulty switch in a later milestone (cheaper), or build both from the start.

## 6. Testing

- `tests/test.sh <walkthrough>`: runs a command list through the game, writes a clean transcript via the in-game
  `/script`, and diffs it against a stored golden transcript. This replaces today's hand inspection.
- Debug-only verbs (in `debug.h`, compiled with `-D`): `jump to act N`, `give X`, `set flag`, so each act can be
  tested without replaying earlier ones.
- Proposed tool: build **dfrotz** (dumb frotz) for Cygwin so tests need no pty tricks and no scrambled curses
  output. (Optional but strongly recommended; a small job.)
- A "random-input" soak test per act to catch crashes and parser surprises.
- Typo/phrase regression list for the parser (vogan, sandwhich, etc.) kept in `tests/parser.txt`.

## 7. Milestones

| # | Milestone | Done when |
|---|---|---|
| M0 | Split `engine/` from `mini/`; mini builds and its three endings still pass | **done 2026-11-16**: transcripts identical to pre-split baseline |
| M1 | Build dfrotz; write `test.sh` and golden transcripts for mini | **done 2026-11-16**: `tests/check.sh mini` replays 4 walkthroughs (3 endings plus a parser/typo/slash list) in about 3 s |
| M2 | Design docs: full map, object list, puzzle chart with hint ladders for all six acts | **done and approved 2026-11-16** (`docs/full/`; decisions in `05-score-and-walkthrough.md`) |
| M3 | Hint engine + `/DIFFICULTY` skeleton | **done 2026-11-16**: `engine/hints.h`; mini converted to nine `Puzzle` ladders; compliance counter and the radio-ending gate tested in `tests/mini` |
| M4 | Act 1 implemented and tested | |
| M5 | Acts 2–3 | **done 2026-11-16**: Hub Station (6 rooms, 50 pts) and Guide HQ (11 rooms, 100 pts) playable end to end; `act 2` and `act 3` debug jumps; 5 cube shortcuts with compliance; walkthroughs `act2`, `act3`, `shortcuts` |
| M6 | Acts 4–5 | **done 2026-11-16**: London (10 rooms, 90 pts) and the pub/lawn (5 rooms, 60 pts) playable; both book endings and the radio choice; 10 full-game walkthroughs pass |
| M7 | Act 6 + both endings in the full game, scoring to 400 | **done 2026-11-16**: Milliways (4 rooms, 40 pts); `tests/full/finale.cmd` plays the whole radio route from Act V to exactly 400 |
| M8 | Full playtest, balance, typo/parser tuning, polish | **done 2026-11-16** (first pass): safety nets, Easy/Faithful assists, soak and release tests, parser fixes; see "As built (M8)" in docs/full/03-puzzles.md. A human playtest is still the best next step. |
| M9 | Release builds (Z8) of both games, walkthroughs, README | **done 2026-11-16**: `tests/package.sh` builds `dist/` (story files, README, credits, walkthroughs, zip); player-facing walkthroughs in `docs/`; `tests/full/complete.cmd` and `tests/mini/complete.cmd` play each game start to finish for full marks on both debug and release builds |

M2 is the big gate: we agree the whole story, puzzles and hints on paper before writing code.

## 8. Risks

- **Scale of writing:** ~40–60k words of original prose. Mitigation: act-by-act, each tested before the next.
- **Z-machine size:** see section 2.
- **Parser regressions as the vocabulary grows:** golden-transcript tests plus the parser list.
- **Tone drift:** one style guide, written during M2 as section 7 of `docs/full/01-story-and-systems.md` (there is no separate `style.md`).
- **Inventing too much of the book:** I should re-check each "book" beat against the novel. Please correct me
  wherever my memory of the plot is off.

## 9. Questions for you

1. Approve the six-act outline, or reshape it (more Lamuella? less spaceport?).
2. Hints first, or hints + difficulty modes together?
3. Z8 as the default target (with Glulx as a fallback)?
4. May I build **dfrotz** to make testing reliable?
5. Names: keep "LAST ORDERS ON LAMUELLA" for the mini, and pick a new subtitle for the full game?

## 10. `/MACRO` (requested 2026-11-16; built as M10)

Record and replay often-used long command sequences (the sandwich routine is the obvious case). To be built in the shared engine
after the full game's acts are in (so both games get it); not scheduled yet. Design constraints, so the game stays enjoyable:

- Commands: `/MACRO RECORD <name>`, `/MACRO STOP`, `/MACRO PLAY <name>`, `/MACRO LIST`, `/MACRO SHOW <name>`, `/MACRO DELETE <name>`.
- Only story commands are recorded; slash commands are not (no recursion, no macro that quits the game).
- Playback is not a skip button: every command runs as if typed, takes its turn, and prints its normal text, so the prose and any timed events are never bypassed.
- Playback **stops at the first surprise**: a parser failure, a refusal, a new character arriving, a change of room that the macro did not expect, or any score change. It then says where and why it stopped.
- It echoes each command before running it (`>take knife`) so transcripts stay readable.
- Small and cheap: about ten macros of up to twelve commands, one-word names, stored in game memory so `/SAVE` and `/RESTORE` keep them.
- Records the words as the parser finally understood them (after typo correction and filler stripping).
- Faithful difficulty may disable it; Easy offers it; Average is on by default.
- Tests: record, replay, stop-on-surprise, and a macro surviving save and restore.

## 11. Future features: newcomer overviews (requested 2026-11-16)

Two short, optional, out-of-story texts for people new to the genre or the franchise. Both are written fresh (no quoted text), clearly marked
`[Out of the story: ...]`, spoiler-light, and built into the shared engine so both games get them.

- **`/INTRO`: interactive fiction for newcomers.** What the genre is; how to read the screen; the handful of commands that matter
  (LOOK, EXAMINE, TAKE, directions, INVENTORY, ASK X ABOUT Y); that plain sentences, typos and questions are fine here; how `/HINT`, `/DIFFICULTY`, `/SAVE` and `/HELP` work;
  and the one rule that matters: nothing you try can break the game. About one screen.
- **`/HHGG` (or `/BACKGROUND`): the Hitchhiker's franchise for newcomers.** A short, friendly account of the franchise: radio in 1978, the novels, the TV series, the Infocom game in 1984, the film,
  and how each version differs from the others (the franchise's own habit); who the main characters are; and where this game sits (an adaptation of the fifth book). No spoilers for the games' plots beyond what the back cover would say.
- Offered once, lightly: the opening banner mentions `/INTRO` and `/HHGG`; never forced; skippable.
- Easy to maintain: each is one routine in `engine/newbie.h` taking a flag for which game it is describing.
- Tests: the text appears, is marked out-of-story, and stays under a screen or two.

## 12. Future feature: easter eggs (requested 2026-11-16)

Hidden, never required, never harmful. Every egg is original prose or a short homage, nothing quoted at length, nothing that can break a puzzle or the score.

- **Floyd (a nod to Infocom's *Planetfall*).** A small, dented, relentlessly cheerful robot, found only by a player who pokes about where nobody looks
  (leading candidate: the heap of lost things at Hub Station, with a second appearance in the Milliways kitchen). He is childlike, curious and a little
  heartbreaking; he plays at hiding; he follows for a few rooms and says things like "Oh boy!"-adjacent in his own words. An out-of-story note explains the reference
  (marked `[Out of the story: ...]`, crediting Infocom and its authors). Pure atmosphere: no points, no puzzle role.
- **XYZZY (a nod to *Colossal Cave*), with random consequences.** Typing `XYZZY` anywhere rolls a die. A table of about twelve outcomes, roughly half good and half mildly annoying, none permanent:
  *good:* a lucky find (a spare towel, a sweet, a free hint rung), a brief teleport to a pleasant earlier room, a kind word from the cube-that-isn't, a free Guide entry.
  *bad:* a hollow voice calling you a fool, a trip to a random earlier room, a gentle dropping of one carried item (put back where you stand), a short bout of sarcasm from the parser, the cube announcing "Good." with no explanation.
  Rules: never alters puzzle state, score, compliance or trust irrecoverably; at most one per ten turns; `PLUGH` and `PLOVER` get a sly one-liner each.
- **A few hidden surprises of my choosing** (all one- or two-liners, found by trying the obvious joke):
  `GRUE` / `WAIT IN THE DARK` (a short Zork-style warning, played straight); asking anyone for `TEA` (a drink "almost, but not quite, entirely unlike tea");
  `DON'T PANIC` typed at the very start (the game takes your word for it); `LOOK AT THE UNIVERSE` on the Milliways terrace; `SING` in the pub; and `FROTZ`/`XYZZY` style words that only work once.
- **Where it lives:** `engine/eggs.h` (XYZZY, PLUGH, GRUE and friends, shared by both games) and per-game hooks for Floyd and place-specific eggs.
- **Tests:** a seeded run proves that XYZZY never breaks the score or locks a puzzle; the eggs list is documented here so nothing is a secret from the maintainers.

**M10 status (2026-11-16): `/MACRO` is built** (`engine/macro.h`). As designed, replay is not a skip button: each command is echoed and run normally, and replay stops at the first surprise (a command that did not parse,
a change in location, unexpected points, or a change in what is in the room or your hands compared with the recording). Differences from the sketch above: slots are 6 macros of 16 commands; commands are stored as typed; points
*not* gained on replay are not a surprise, only unexpected points are. Tests: `tests/mini/macro.cmd`, `macro_save.cmd`, `tests/full/macro.cmd`.

## 11a. M11 status (2026-11-16): newcomer overviews are built

`/INTRO` (what interactive fiction is and how to play it) and `/HHGG` (a spoiler-free account of the franchise, ending with where the game fits) live in `engine/newbie.h`; each game supplies one paragraph, `GameBlurb`. Both are marked
`[Out of the story: ...]`, mentioned in the opening banner and in `/HELP`, and use only original wording. Tests: `tests/full/newbie.cmd`, `tests/mini/newbie.cmd`.
