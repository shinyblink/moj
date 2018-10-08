CC ?= cc

all: moj

# Generate lookup lists
gen/emoji.json:
	mkdir -p gen
	wget -O $@ https://raw.githubusercontent.com/github/gemoji/master/db/emoji.json

gen/emoji.csv: gen/emoji.json
	jq -r '.[] | select(.emoji) | [.emoji] + .aliases | @csv' -- \
		gen/emoji.json > $@

gen/emoji.gperf: src/gen_gperf.sh gen/emoji.csv
	./src/gen_gperf.sh > ./gen/emoji.gperf

gen/emoji_list.h: src/gen_list.sh gen/emoji.csv
	./src/gen_list.sh > ./gen/emoji_list.h

gen/lookup.c: gen/emoji.gperf
	gperf --global-table --readonly-tables \
		--struct-type --includes \
		--compare-lengths --compare-strncmp \
		--ignore-case -Q emoji_spool \
		-H emoji_name_hash -N emoji_lookup_ref -m 3 \
		--output-file=$@ gen/emoji.gperf

# Compile our code.
src/emoji.o: gen/lookup.c gen/emoji_list.h
	$(CC) -c -o $@ ${CFLAGS} ${CPPFLAGS} src/emoji.c

moj: src/main.c src/emoji.o
	$(CC) -o $@ ${CFLAGS} ${CPPFLAGS} ${LDFLAGS} src/main.c src/emoji.o

# Cleanup
clean:
	rm -f moj gen/emoji_list.h gen/lookup.c gen/emoji.gperf
