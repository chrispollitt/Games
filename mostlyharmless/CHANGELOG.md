# Changelog

## 1.1.0 — 2026-11-16

- `/MACRO RECORD|STOP|PLAY|LIST|SHOW|DELETE` (engine/macro.h).
- A few hidden surprises (see docs/PLAN.md section 12a), including a cameo robot in the full game.
- `/INTRO` and `/HHGG`: short out-of-story overviews for newcomers to interactive fiction and to the Hitchhiker franchise (engine/newbie.h).

## 1.0.0 — 2026-11-16 (first public release)

**MOSTLY HARMLESS: THE LONG WAY ROUND** (full game, Z-machine v8)
- Six acts, about 44 rooms, 400 points: Lamuella, Hub Station, the Guide's offices, a parallel London, the Dancing Gnu and the lawn, and the Restaurant at the End of the Universe.
- Two endings: the book's (Earth demolished, in two variants) and the happier BBC radio-play ending.
- The Guide Mk II as a courteous, menacing presence, with a "compliance" counter that decides whether the happy ending stays open.
- A progressive hint ladder for every puzzle, and three difficulty levels (easy, average, faithful).

**MOSTLY HARMLESS: LAST ORDERS ON LAMUELLA** (short game, Z-machine v5): the same story in about an hour, with both endings.

**Shared engine**
- Flexible input: filler words, punctuation, one-letter typos, loose phrasing, questions (`WHO WAS DOUGLAS ADAMS?`), compound commands (`TAKE PARCEL AND EXAMINE IT`).
- `/MACRO` records and replays command sequences (it stops if anything is different, and never skips the story).
- Out-of-story commands behind a slash: `/SAVE`, `/RESTORE` (optionally with a name, as a reminder), `/HINT`, `/DIFFICULTY`, `/HELP` and so on.

**Testing**
- Scripted walkthroughs with golden transcripts, random-input soak testing, release-build replay, and a continuous start-to-finish run that scores 400/400 (full) and 100/100 (mini).

Known limits: the Z-machine cannot choose a save file's name, so `/SAVE name` is a reminder only; the Faithful mode does not turn off the library's automatic "(first taking ...)" actions.
