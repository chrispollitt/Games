# MOSTLY HARMLESS

An unofficial, non-commercial interactive-fiction fan adaptation of the plot of the fifth Hitchhiker's Guide novel, written in Inform 6 with entirely original prose.
*The Hitchhiker's Guide to the Galaxy* and its characters belong to the estate of Douglas Adams. See [CREDITS.md](CREDITS.md).

There are two games, built on one flexible-input engine:

| Game | What it is | Length | Story file |
|---|---|---|---|
| **MOSTLY HARMLESS: THE LONG WAY ROUND** (`full/`) | The full game: six acts, about 44 rooms, 400 points. Lamuella, a spaceport, the Guide's offices, a parallel London, a pub with a laser pistol, and the Restaurant at the End of the Universe. Two endings (the book's, the BBC radio play's). | several hours | `full/full.z8` (Z-machine v8) |
| **MOSTLY HARMLESS: LAST ORDERS ON LAMUELLA** (`mini/`) | A short game of the same story: the sandwich hut, the parcel, the pub. Two endings. | about an hour | `mini/mini.z5` (Z-machine v5) |

## Playing

Build the story files (see below) and open one in any Z-machine interpreter (Frotz, Gargoyle, Lectrote, and so on):

```sh
frotz full/full.z8
```

You can type plain English: filler words, punctuation, typos and compound commands (`TAKE PARCEL AND EXAMINE IT`) are all fine, and you can ask questions (`WHO WAS DOUGLAS ADAMS?`).
Commands about the game itself start with a slash, as in a chat program:

| Command | |
|---|---|
| `/HINT`, `/HINT BACK` | a progressive hint for the puzzle you are probably stuck on |
| `/DIFFICULTY EASY` / `AVERAGE` / `FAITHFUL` | how much help you get |
| `/SAVE`, `/RESTORE`, `/UNDO`, `/QUIT` ... | the usual, with a slash |
| `/HELP`, `/ABOUT` | the list of commands; information about the game |

Spoiler-heavy walkthroughs: [full game](docs/WALKTHROUGH-full.md), [mini game](docs/WALKTHROUGH-mini.md).

## Building

Needs the Inform 6 compiler (6.4x) with the 6.12 standard library.

```sh
./build.sh mini            # release  -> mini/mini.z5
./build.sh full            # release  -> full/full.z8
./build.sh full debug      # debug build with tester verbs (ACT n, COMPLY n, PUZZLES ...)
```

`build.sh` expects the library in `/usr/local/share/inform6/lib`; edit it if yours lives elsewhere.

## Testing

Needs `dfrotz` (dumb frotz) on `PATH`, or `DFROTZ=/path/to/dfrotz`. On Cygwin, build it from the Frotz source with `CFLAGS=-D_GNU_SOURCE make dumb`.

```sh
tests/check.sh full          # replay every walkthrough, diff against golden transcripts
tests/check.sh mini
tests/release.sh             # release builds, sizes, release replay
tests/soak.sh full 3 40 90   # random-input soak: game, act, runs, commands per run
tests/package.sh             # build release story files and a distributable zip in dist/
```

`tests/full/complete.cmd` plays the whole full game, first command to last, for exactly 400 points; `tests/mini/complete.cmd` does the same for the mini-game (100 points).

## Layout

| Folder | |
|---|---|
| `engine/` | shared parser layer: loose phrasing, typo forgiveness, question answering, compound commands, `/slash` commands, progressive hints, difficulty, the Guide's "compliance" counter |
| `mini/`, `full/` | the two games |
| `tests/` | walkthroughs, golden transcripts, soak and release scripts |
| `docs/` | plan, design documents (`docs/full/`), walkthroughs |

## Status

All six acts of the full game and the whole mini-game are written and pass the automated tests, including a continuous start-to-finish run for full marks. What the tests cannot tell is whether it is *fun*: a human
playtest is the best next step. Future ideas (`/MACRO`, newcomer overviews, easter eggs) are described in `docs/PLAN.md`.

## Licence

- The reusable engine and tooling (`engine/`, `build.sh`, `tests/*.sh`) are under the **MIT licence** (`LICENSE`).
- The games, their text and the documentation are under **Creative Commons Attribution-NonCommercial-ShareAlike 4.0** (`LICENSE-CONTENT`).
- Third-party notices are in `NOTICE`; the details are in `LICENSING.md`.

This is an unofficial, non-commercial fan work and is not endorsed by the estate of Douglas Adams. See `CREDITS.md`.
