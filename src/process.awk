#!/usr/bin/awk -f
# This file takes in our CSV, emits simpler stuff.
# It also fixes up some things.

# Bunch of convenience functions.
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

function char(str, idx) {
	return substr(str, idx, 1);
}

function prefixed(name, prefixes) {
	for (prefix in prefixes)
		if (index(name, prefixes[prefix]) == 1)
			return 1;
	return 0;
}

function noing(name) {
	# yep thats the name.
	len = length(name);
	if (len > 5 && substr(name, len - 2, 3) == "ing" && !prefixed(name, persons)) {
		if (char(name, len - 4) == char(name, len - 3)) {
			# double letter combo, gg, nn, etc..
			short = substr(name, 0, len - 4);
		} else {
			short = substr(name, 0, len - 3);
		}
		if (newname(short))
			printf ",%s", short;
	}
}

BEGIN {
	FS="\",\""
	persons[1] = "person_";
	persons[2] = "women_";
	persons[3] = "woman_"
	persons[4] = "men_";
	persons[5] = "man_";
}

{
	if (hasnew())
		printf "%s", $1;

	for (i = 2; i <= NF; i++) {
		if (newname($i)) {
			printf ",%s", $i;
			noing($i);
			if (index($i, "person_") == 1) {
				replaced = substr($i, 8);
				if (newname(replaced)) {
					printf ",%s", replaced;
				}
				noing(replaced);
			}
		}
	}
	printf "\n";
}
