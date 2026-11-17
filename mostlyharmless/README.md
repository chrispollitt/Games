# MOSTLY HARMLESS

An unofficial, non-commercial interactive-fiction fan adaptation of the plot of the fifth Hitchhiker's Guide novel,
written in Inform 6 with entirely original prose. The Hitchhiker's Guide to the Galaxy and its characters belong to the
estate of Douglas Adams.

Two games share one flexible-input engine:

| Folder | What |
|---|---|
| `mini/` | **Last Orders on Lamuella**: a short game (about an hour) with two endings: the book's and the BBC radio one. Playable now. |
| `full/` | The big version (design phase; see `docs/full/`). |
| `engine/` | The shared parser layer: loose phrasing, typo forgiveness, question answering, `/slash` out-of-story commands. See `engine/README.md`. |
| `tests/` | Walkthroughs and golden transcripts; `tests/check.sh mini` runs them in a few seconds. |
| `docs/` | `PLAN.md` (project plan and milestones) and `docs/full/` (story, map, puzzles, cast for the full game). |

## Build and play

Needs the Inform 6 compiler (6.4x) with the 6.12 standard library and a Z-machine interpreter.

```sh
./build.sh mini            # release  -> mini/mini.z5
./build.sh mini debug      # debug    -> mini/mini_debug.z5
frotz mini/mini.z5         # or nfrotz, dfrotz, any Z-machine interpreter
```

`build.sh` expects the library in `/usr/local/share/inform6/lib`; edit it if yours lives elsewhere.

## Test

```sh
tests/check.sh mini           # replay every walkthrough and diff against the golden transcripts
tests/check.sh mini --update  # accept new output as golden
```

Needs `dfrotz` (dumb frotz) on `PATH`, or set `DFROTZ=/path/to/dfrotz`. On Cygwin, build it from the Frotz source with
`CFLAGS=-D_GNU_SOURCE make dumb`.

## Playing notes

Commands outside the story use a slash (`/SAVE`, `/HINT`, `/HELP`). Everything else is plain English; the game tolerates
filler words, punctuation, and one-letter typos, and answers questions such as `WHO WAS DOUGLAS ADAMS?`.
