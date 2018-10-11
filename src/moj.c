// moj: Small Emoji library
// Based on our generated things.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../gen/lookup.c"
#include "../gen/emoji_list.h"

const char* moj_lookup(const char* name, size_t len) {
	const struct emoji_ref *found = emoji_lookup_ref(name, len);
	if (found) {
		return emoji_list[found->ref - 1];
	}
	return NULL;
}

char* moj_replace(const char* text, size_t len) {
	if (!len)
		return strdup(text);

	char c;
	// count size delta to alloc a string just the right size.
	int size_delta = 0;

	size_t ci;
	size_t start = 0;
	size_t strend;
	int attention = 0;
	for (ci = 0; ci <= len; ci++) {
		c = text[ci];

		if (attention) { // in :
			if (c == ' ') {
				attention = 0;
			} else if (c == ':') {
				// check if start to ci - start -1 is a valid emoji.
				size_t elen = ci - start - 1;
				const char* found = moj_lookup(text + start + 1, elen);
				if (found) {
					size_delta += elen - strlen(found);
				} else {
					start = ci;
				}
				attention = 0;
			}
		} else if (c == ':' && ci < len) {
			attention = 1;
			start = ci;
		}
	}

	// allocate buffer the right size.
	char* buf = malloc(len + 1 + size_delta);
	if (!buf)
		return NULL;

	start = 0;
	attention = 0;
	size_t offset = 0;
	for (ci = 0; ci <= len; ci++) {
		c = text[ci];

		if (attention) { // in :
			if (c == ' ') {
				attention = 0;
				memcpy(buf + offset, text + start, ci - start + 1);
				offset += ci - start + 1;
			} else if (c == ':') {
				// check if start to ci - start -1 is a valid emoji.
				const char* found = moj_lookup(text + start + 1, ci - start - 1);
				if (found) {
					offset += sprintf(buf + offset, "%s", found);
				} else {
					// not an emoji? oops.
					memcpy(buf + offset, text + start, ci - start + 1);
					offset += ci - start  + 1;
					start = ci;
					// the next might be one, though..
				}
				attention = 0;
			}
		} else if (c == ':' && ci < len) {
			attention = 1;
			start = ci;
		} else {
			buf[offset] = c;
			offset++;
		}
	}
	return buf;
}
