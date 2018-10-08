// moj.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "emoji.h"

#define MODE_NORMAL 0
#define MODE_LOOKUP 1

void usage(char* name, int fail) {
	FILE* f = stdout;
	if (fail) f = stderr;
	fprintf(f, "Usage: %s [-e emoji] <:emoji: text>\n"
	           "  -e: lookup mode - interpret arguments as emoji names\n"
	           "    -s <sep>: use custom seperator in lookup mode\n", name);
	exit(fail);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		usage(argv[0], 1);
	}

	// parse opts
	int opt;
	int mode = MODE_NORMAL;
	char* sep = NULL;
	while ((opt = getopt(argc, argv, "hes:")) != -1) {
		switch (opt) {
		case 'h':
			usage(argv[0], 0);
			break;
		default:
		case '?':
			usage(argv[0], 1);
			break;
		case 'e':
			mode = MODE_LOOKUP;
			break;
		case 's':
			sep = strdup(optarg);
			break;
		}
	}

	if (optind >= argc)
		usage(argv[0], 1);

	int argi;
	switch (mode) {
	case MODE_LOOKUP:
		for (argi = optind; argi < argc; argi++) {
			char* found = emoji_lookup(argv[argi], strlen(argv[argi]));
			if (found) {
				printf("%s%s", found, sep ? sep : "\n");
			} else {
				printf("Couldn't find %s. :(\n", argv[argi]);
			}
		}
		if (sep) printf("\n");
		break;
	case MODE_NORMAL:
		fprintf(stderr, "NYI. Sorry.\n");
		return 1;
		break;
	}

	if (sep) free(sep);
	return 0;
}
