// moj: profanity plugin.
// this plugin intercepts text and replaces :emoji:.

#include <profapi.h>
#include <string.h>
#include "moj.h"

char* prof_pre_chat_message_send(const char* const barejid, const char* message) {
	return moj_replace(message, strlen(message)); // yep that is it.
}
