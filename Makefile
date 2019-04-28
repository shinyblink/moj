# moj makefile
CC ?= cc

MOJILIST=https://raw.githubusercontent.com/milesj/emojibase/master/packages/data/en/raw.json

# Do not touch.
all: moj

# Generate lookup lists
gen/emoji.json:
	mkdir -p gen
	wget -O $@ ${MOJILIST}

gen/emoji.csv: gen/emoji.json
	jq -r '.[] | select(.emoji) | [.emoji] + .shortcodes | @csv' -- \
		gen/emoji.json | \
		sed 's/^"//;s/"$$//' | awk -f src/process.awk > $@

gen/emoji.gperf: src/gen_gperf.sh gen/emoji.csv
	./src/gen_gperf.sh > ./gen/emoji.gperf

gen/emoji_list.h: src/gen_list.sh gen/emoji.csv
	./src/gen_list.sh > $@

gen/lookup.c: gen/emoji.gperf
	gperf --global-table --readonly-tables \
		--struct-type --includes \
		--compare-lengths --compare-strncmp \
		--ignore-case -Q emoji_spool \
		-H emoji_name_hash -N emoji_lookup_ref -m 3 \
		--output-file=$@ $^

# Compile our code.
src/moj.o: gen/lookup.c gen/emoji_list.h src/moj.c
	$(CC) -c -o $@ ${CFLAGS} ${CPPFLAGS} src/moj.c

libmoj.so: src/moj.o
	$(CC) -shared -fPIC -o $@ ${LDFLAGS} $<

moj: src/main.c src/moj.o
	$(CC) -o $@ ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} $^

# Profanity plugin.
profanity_emoji.so: src/profanity_emoji.c src/moj.o
	$(CC) -shared -fPIC -o $@ ${LDFLAGS} $^

profanity: profanity_emoji.so

# Install
install: moj libmoj.so
	install moj $(DESTDIR)/$(PREFIX)/bin
	install libmoj.so $(DESTDIR)/$(PREFIX)/lib

uninstall:
	cd $(DESTDIR)/$(PREFIX)/ && rm -f bin/moj lib/libmoj.so
	

# Cleanup
clean:
	rm -f moj gen/lookup.c src/*.o profanity_emoji.so
