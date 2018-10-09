# moj
'moj is for those of us that want something more minimal than emojify.

Reasons to use moj instead of emojify:
- only 3 letters(!), huge reduction compared to 7 of emojify.
- written in C, compile time hash table generation, no interpreted language.
- VERY FAST. How does 0.002s to generate the average commit messages of this project sound? This is on a ThinkPad T61. A potato.
- data not mixed with code, only generated data/glue and handwritten code. (seperate files)

# Requirements
- Make (compile time)
- C99 compiler(?). (compile time)
- gperf (compile time)

No runtime deps.

To regen the emoji list you also need:
- wget
- jq
- awk

# Credits
[milesj/emojibase](https://github.com/milesj/emojibase) for the JSON Emoji list. Many thanks

vifino for doing everything else.

# License
ISC.
