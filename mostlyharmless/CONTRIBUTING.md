# Contributing

Thanks for thinking about it. This is a small fan project; bug reports, typo fixes and playtest notes are all welcome.

## Ground rules

- **Original prose only.** Do not paste text from the Hitchhiker's novels, radio or television scripts, or Infocom's game. Short in-story references and homages are fine; copied passages are not.
- **Nothing that can lose the game.** Every puzzle must stay solvable whatever the player has dropped, given away or broken. If you add a way to lose an item, add a safety net.
- **Keep it kind to players.** Typos, odd phrasing and compound commands should work. If a sensible phrasing fails, that is a bug.
- By contributing you agree that your contribution is licensed as described in `LICENSING.md` (MIT for `engine/` and the scripts; CC BY-NC-SA 4.0 for the games and documents).

## Building and testing

```sh
./build.sh full debug && ./build.sh mini debug      # debug builds (tester verbs: ACT n, COMPLY n, PUZZLES)
tests/check.sh full && tests/check.sh mini          # replay every walkthrough against golden transcripts
tests/release.sh                                    # release builds, sizes, replay on the release files
tests/soak.sh full 3 40 90                          # random-input soak test: game, act, runs, length
```

You need the Inform 6 compiler and the 6.12 library, and `dfrotz` (see the README). A change that alters output on purpose should update the golden transcripts
(`tests/check.sh <game> --update`) and say so in the pull request; a change that alters them by accident is a bug.

## Where things are

`engine/README.md` explains the shared layer and what a game must provide; `docs/full/` holds the design documents (map, puzzles with hint ladders, cast); `docs/PLAN.md` has the milestone history and the ideas
for future features (`/MACRO`, newcomer overviews, easter eggs).
