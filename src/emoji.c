// Small Emoji library
// Based on our generated things.

#include "../gen/lookup.c"
#include "../gen/emoji_list.h"

const char* emoji_lookup(const char* name, size_t len) {
	const struct emoji_ref *found = emoji_lookup_ref(name, len);
	if (found) {
		return emoji_list[found->ref - 1];
	}
	return NULL;
}
