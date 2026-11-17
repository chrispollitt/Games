# Games

A place for fun and distraction: small games and game-related experiments, mostly written for the terminal, mostly written together with an AI assistant, and always for the fun of it.

Each project lives in its own folder with its own README, its own licence and its own tags (`<project>-vX.Y.Z`).

## Projects

| Project | What it is | Language | Licence |
|---|---|---|---|
| [**maze4**](maze4/) | A fully autonomous terminal game: genetically-enhanced Fruitbots race through a procedurally generated maze, fighting Veggie Monsters for the title of Supreme Produce. No input required; sit back and watch the chaos. Includes a man page, a browser version and a `ttyd` launcher. | C++ (plus a JavaScript web build) | Produce Public License (see `maze4/LICENSE`) |
| [**mostlyharmless**](mostlyharmless/) | An unofficial, non-commercial interactive-fiction adaptation of the fifth *Hitchhiker's Guide* novel, *Mostly Harmless*: a full six-act game (about 44 rooms, 400 points, two endings), a short one-hour version, and a shared parser engine that forgives typos, accepts plain English and compound commands, answers questions, and offers progressive hints. | Inform 6 (Z-machine) | MIT for the engine and tooling; CC BY-NC-SA 4.0 for the games and documents |

## Quick start

**maze4** (needs a C++ compiler and `ncurses`):

```sh
cd maze4
make
./maze4
```

**mostlyharmless** (needs the Inform 6 compiler with the 6.12 library, and any Z-machine interpreter such as Frotz):

```sh
cd mostlyharmless
./build.sh full          # -> full/full.z8
frotz full/full.z8
```

Each project's README has the full instructions, options, and (for *mostlyharmless*) spoiler-heavy walkthroughs.

## Repository layout

```
Games/
├── README.md            this file
├── maze4/               the maze game (source, man page, web build, tools)
└── mostlyharmless/      the interactive fiction (engine/, mini/, full/, tests/, docs/)
```

Build products (object files, story files, test transcripts, `dist/` packages) are not committed; see `.gitignore`.

## How these were made

These projects are built collaboratively with an AI assistant (Claude from Anthropic) under the repository owner's direction: the ideas, decisions and testing are human, much of the code and prose is machine-written, and everything is checked by automated tests where possible. *mostlyharmless* in particular has a
scripted test suite that plays both games from the first command to the last. Each project's `CREDITS` or README says more.

## Releases and tags

Tags are per project so that unrelated work does not collide, for example `mostlyharmless-v1.0.0`. Release notes live in each project (`mostlyharmless/CHANGELOG.md`).

## Contributing

Bug reports, typo fixes and playtest notes are welcome. See `mostlyharmless/CONTRIBUTING.md` for that project's ground rules (original prose only, nothing that can make the game unwinnable, run the tests). For `maze4`, open an issue or a pull request with a short description of the change.

## Disclaimer

*mostlyharmless* is an unofficial fan work. *The Hitchhiker's Guide to the Galaxy* and its characters belong to the estate of Douglas Adams; this repository is not affiliated with or endorsed by them. See `mostlyharmless/NOTICE`.
