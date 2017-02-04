WARNINGS = \
	-ansi \
	-Wall \
	-Wcast-align \
	-Wcast-qual \
	-Wconversion \
	-Wdisabled-optimization \
	-Werror \
	-Wextra \
	-Wextra \
	-Wfloat-equal \
	-Wformat=2 \
	-Wformat-nonliteral \
	-Wformat-security \
	-Wformat-y2k \
	-Wimport \
	-Winit-self \
	-Winline \
	-Winvalid-pch \
	-Wlong-long \
	-Wmissing-field-initializers \
	-Wmissing-format-attribute \
	-Wmissing-include-dirs \
	-Wmissing-noreturn \
	-Wno-aggregate-return \
	-Wno-padded \
	-Wno-switch-enum \
	-Wno-unused-parameter \
	-Wpacked \
	-Wpointer-arith \
	-Wredundant-decls \
	-Wshadow \
	-Wstack-protector \
	-Wstrict-aliasing=2 \
	-Wswitch \
	-Wswitch-default \
	-Wunreachable-code \
	-Wunused \
	-Wvariadic-macros \
	-Wwrite-strings

CFLAGS += -I../inc

DEPS = $(wildcard ../inc/*.h)
XMLDEPS = $(wildcard xml/*.xml)

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS) $(WARNINGS)

all: saisanitycheck saimetadatatest saidepgraph.svg
	./saimetadatatest > /dev/null
	./saisanitycheck

xml: $(DEPS) Doxyfile saimetadatatypes.h
	doxygen Doxyfile 2>&1 | perl -npe '$$e=1 if /warning/i; END{exit $$e}';
	touch xml

saimetadata.c saimetadata.h: xml $(XMLDEPS) parse.pl saimetadatatypes.h
	perl parse.pl

saisanitycheck: saisanitycheck.c saimetadata.c saimetadata.h saimetadatatypes.h saimetadatautils.c saimetadatautils.h
	g++ -c saimetadatautils.c -o /dev/null $(CFLAGS) $(WARNINGS)
	g++ -c saisanitycheck.c -o /dev/null $(CFLAGS) $(WARNINGS)
	g++ -c saimetadata.c -o /dev/null $(CFLAGS) $(WARNINGS)
	gcc -o $@ saisanitycheck.c saimetadata.c saimetadatautils.c $(CFLAGS) $(WARNINGS)

saimetadatatest: saimetadatatest.c saimetadata.c saimetadata.h saimetadatatypes.h saimetadatautils.c saimetadatautils.h
	gcc -o $@ saimetadatatest.c saimetadata.c saimetadatautils.c $(CFLAGS) $(WARNINGS)

saidepgraphgen: saidepgraphgen.cpp
	g++ -o $@ saidepgraphgen.cpp saimetadata.c saimetadatautils.c $(CFLAGS) $(WARNINGS)

saidepgraph.gv: saidepgraphgen
	./saidepgraphgen > $@

saidepgraph.svg: saidepgraph.gv
	dot -Tsvg saidepgraph.gv > $@

.PHONY: clean

clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak sai*.gv sai*.svg
	rm -f saimetadata.h saimetadata.c saimetadatatest.c
	rm -f saisanitycheck saimetadatatest saidepgraphgen
	rm -rf xml html dist
