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

CONSTHEADERS = saimetadatatypes.h saimetadatalogger.h saimetadatautils.h

xml: $(DEPS) Doxyfile $(CONSTHEADERS)
	doxygen Doxyfile 2>&1 | perl -npe '$$e=1 if /warning/i; END{exit $$e}';
	touch xml

saimetadata.c saimetadata.h: xml $(XMLDEPS) parse.pl saimetadatatypes.h
	perl parse.pl

HEADERS = saimetadata.h $(CONSTHEADERS)

COMMONSRC = saimetadata.c saimetadatautils.c

saisanitycheck: saisanitycheck.c $(HEADERS)
	gcc -o $@ saisanitycheck.c $(COMMONSRC) $(CFLAGS) $(WARNINGS)

saimetadatatest: saimetadatatest.c $(COMMONSRC) $(HEADERS)
	gcc -o $@ saimetadatatest.c $(COMMONSRC) $(CFLAGS) $(WARNINGS)

saidepgraphgen: saidepgraphgen.cpp $(HEADERS)
	g++ -o $@ saidepgraphgen.cpp $(COMMONSRC) $(CFLAGS) $(WARNINGS)

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
