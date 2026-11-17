# Licensing

This repository contains original work by its authors, built on third-party tools. It does **not** license, and is not endorsed by, the estate of Douglas Adams or anyone else
who owns rights in *The Hitchhiker's Guide to the Galaxy*.

## What is licensed how

| Part | Licence | Why |
|---|---|---|
| `engine/`, `build.sh`, `tests/*.sh` | **MIT** (`LICENSE`) | The parser layer (flexible phrasing, typo forgiveness, compound commands, slash commands, hints) and the test tooling are meant to be reused in other games. |
| `mini/`, `full/` (the two games' source, including all story text and puzzles), `docs/`, `README.md`, `CREDITS.md`, `CHANGELOG.md`, `CONTRIBUTING.md`, the walkthroughs, and everything in `tests/` that is not a script (command lists and golden transcripts) | **Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International** (`LICENSE-CONTENT`) | The games are creative works adapting someone else's fiction as a non-commercial tribute; the licence keeps them that way. |
| `third_party/` | the original licences of those files | See `NOTICE`. |

In plain words: you may play, share and adapt the games for non-commercial purposes if you credit them and share your changes under the same terms. You may reuse the engine for anything,
including commercial work, provided you keep the MIT notice. Neither licence gives you any right to use the *Hitchhiker's* names, characters or marks beyond what the law already allows; that is
a matter between you and their owners.

## Compiled story files

The story files (`.z5`, `.z8`) contain the Inform 6 standard library, which is © Graham Nelson 1993–2004 and David Griffith 2012–2025 and is licensed under either the traditional Inform licence or the
Artistic License 2.0; the latter's text is in `third_party/`. The compiled games as a whole are distributed under the CC BY-NC-SA 4.0 terms above, with the library parts remaining under their own licence.

## Changing this

These choices can be changed by the repository owner. If you contribute (see `CONTRIBUTING.md`) you agree that your contribution is licensed on the terms above for the part of the repository it goes into.
