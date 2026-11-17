# engine/ — shared by the MOSTLY HARMLESS games

| File | What it does |
|---|---|
| `smart.h` | Input clean-up (`BeforeParsing`), everyday-verb synonyms (`UnknownVerb`), friendlier errors (`ParserError`), questions (`Lore`, `QuestionSub`), loose `Talk`/`Hello`. Includes the three files below. |
| `smartgram.h` | Grammar: questions, tell/ask/talk, hello, `go to ...`, `/hint`, `/help`. |
| `typo.h` | Typo forgiveness (one slip per word, unambiguous only). |
| `macro.h` | `/MACRO RECORD name`, `STOP`, `PLAY name`, `LIST`, `SHOW`, `DELETE`: record and replay story commands (6 macros of up to 16 commands). Replay echoes and runs each command normally and stops at the first surprise. Needs `Replace KeyboardPrimitive;` before `Include "Parser";`. Faithful mode disables it. |
| `newbie.h` | `/INTRO` (how interactive fiction works) and `/HHGG` (the franchise, spoiler-free). A game supplies `GameBlurb`. |
| `compound.h` | Compound commands: `TAKE PARCEL AND EXAMINE IT`, `DROP ALL, LOOK`, `E; E` (the library mis-reads AND before a verb, so we turn it into THEN). |
| `slash.h` | `/SAVE`, `/HINT`, `/HELP` ... and the "did you want the slash version?" reply. `/SAVE name` and `/RESTORE name` accept a file name; see the note below. |
| `hints.h` | Progressive hints (`Puzzle` class, `/HINT`, `/HINT BACK`), `/DIFFICULTY` (easy / average / faithful), the cube compliance counter (`Comply`, `CubeAllowsRadio`) and `HintTick` (call once a turn; easy mode offers a nudge after a long stall). |
| `debug.h` | Tester verbs, compiled only with `-D`: `COMPLY n`, `PUZZLES`. |
| `lore_real.h` | Real-world facts (Douglas Adams, the series, Infocom ...), always marked "[Out of the story: ...]". |

## What a game must provide

Define these (routines may appear anywhere, but they must exist). The game file includes `smart`, `Grammar`, `smartgram`, its own grammar, then `debug`:

| Hook | Purpose |
|---|---|
| `Puzzle` objects | one per puzzle, with four hint rungs and an `active` test (see `hints.h`); the engine supplies `HintSub` |
| `AboutSub` and `Verb meta 'about' ... -> About` | action for `/ABOUT` |
| `WalkToSub` | action for `GO TO <place>` (uses `TopicAny(...)`) |
| `LoreSelf` | answers about the player and the game; runs before real-world facts |
| `LoreWorld` | in-story facts (Vogons, towels ...); runs after real-world facts |
| `TalkTopics(noun)` | prints a parenthetical of suggested topics |
| `ParserExamples` | prints sample commands for the "I didn't follow that" message |

Order in the game file: library `Parser` and `VerbLib`, the world, the hooks, then
`Include "smart"; Include "Grammar"; Include "smartgram";` and the game's own grammar.

## Helpers

`HintTick()`: call it from a per-turn daemon. `Comply(1)`: call it when the player accepts a cube shortcut.

`TopicAny(w1, ..., w7)` is true if any of those words appears in the typed topic.
Use it only inside topic actions (`Question`, `Ask`, `WalkTo`, ...), after setting `consult_from` / `consult_words`.

## Building

`../build.sh <game> [debug]` puts `engine/` on the include path.

## Notes

**`/SAVE name` and `/RESTORE name`.** The Z-machine does not let a story file choose the save file's name; the interpreter asks for it itself. So the name you type is
removed from the command and shown back as a reminder just before the interpreter's own prompt, and `/RESTORE` on its own reminds you of the last name used this session.
A real "save as NAME" would need a Glulx build or an interpreter that cooperates.

**Compound commands.** `compound.h` turns `AND`, a comma or a semicolon that is followed by a verb (or a compass direction) into `THEN`, which the library already understands.
`TAKE LOAF AND KNIFE` still takes both; `TAKE LOAF AND EXAMINE IT` takes, then examines. A comma after a person's name (`HOB, HELLO`) is still treated as addressing them.

**Z-machine limits.** A story file has at most 233 global variables, and the full game now uses about 227 of them. Future features should keep their state in arrays (as `macro.h` does with the `mv` array) rather than adding globals.
