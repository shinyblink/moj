#!/usr/bin/awk -f
# This file takes in our CSV, emits simpler stuff.
# It also fixes up some things.

function didsee(name) {
	if (name in seen)
		return 1;
	return 0;
}

function hasnew() {
	for (i = 2; i <= NF; i++)
		if (didsee($i))
			return 0;
	return 1;
}

function newname(name) {
	if (didsee(name))
		return 0;

	seen[name] = 1;
	return 1;
}

BEGIN {
	FS="\",\""
	seen[0] = ""
}

{
	if (hasnew())
		printf "%s", $1;

	for (i = 2; i <= NF; i++) {
		if (newname($i)) {
			printf ",%s", $i;
			if (index($i, "person_") == 1) {
				replaced = substr($i, 8);
				if (newname(replaced)) {
					printf ",%s", replaced;
				}
			}
		}
	}
	printf "\n";
}
