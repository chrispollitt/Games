# engine/ — shared by the MOSTLY HARMLESS games

| File | What it does |
|---|---|
| `smart.h` | Input clean-up (`BeforeParsing`), everyday-verb synonyms (`UnknownVerb`), friendlier errors (`ParserError`), questions (`Lore`, `QuestionSub`), loose `Talk`/`Hello`. Includes the three files below. |
| `smartgram.h` | Grammar: questions, tell/ask/talk, hello, `go to ...`, `/hint`, `/help`. |
| `typo.h` | Typo forgiveness (one slip per word, unambiguous only). |
| `slash.h` | `/SAVE`, `/HINT`, `/HELP` ... and the "did you want the slash version?" reply. |
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
