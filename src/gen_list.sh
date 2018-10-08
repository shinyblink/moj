#!/bin/sh
# Generator for the actual emoji list.
set -e

EMOJIS=$(wc -l < gen/emoji.csv)

cat <<HEADER
// THIS FILE IS GENERATED BY src/gen_list.sh.
// DO NOT TOUCH HERE.

#define NUM_EMOJI $EMOJIS
static const char* emoji_list[$EMOJIS] = {
HEADER

gen() {
	printf "\t\"%s\",\n" $1
}

cat gen/emoji.csv | sed 's/,/ /g' | while read -r input; do
	gen $(eval "echo $input")
done

cat <<FOOTER
};
FOOTER

