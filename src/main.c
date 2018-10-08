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
	fprintf(f, "Usage: %s [-s sep] [-e] <:emoji: text>\n"
	           "  -e: lookup mode - interpret arguments as emoji names\n"
	           "  -s <sep>: use custom seperator\n", name);
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
			sep = optarg;
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
				return 1;
			}
		}
		if (sep) printf("\n");
		break;
	case MODE_NORMAL:
		for (argi = optind; argi < argc; argi++) {
			char* s = argv[argi];
			char c;
			int ci;
			int len = strlen(s);
			int start = 0;
			int attention = 0;
			for (ci = 0; ci <= len; ci++) {
				c = s[ci];

				if (attention) { // in :
					if (c == ' ') {
						attention = 0;
						s[ci] = 0;
						fprintf(stdout, "%s ", s + start);
					} else if (c == ':') {
						// check if start to ci - start -1 is a valid emoji.
						s[ci] = 0;
						char* found = emoji_lookup(s + start + 1, ci - start - 1);
						if (found) {
							fprintf(stdout, "%s", found);
						} else {
							// not an emoji? oops.
							fprintf(stdout, "%s:", s + start);
							start = ci;
							// the next might be one, though..
						}
						attention = 0;
					}
				} else if (c == ':' && ci < len) {
					attention = 1;
					start = ci;
				} else {
					putc(c, stdout);
				}
			}
			if (attention && start < len)
				fprintf(stdout, "%s", s + start + 1);

			if (argi < (argc - 1))
				fprintf(stdout, sep ? sep : " ");
		}
		fprintf(stdout, "\n");
		break;
	}

	return 0;
}
