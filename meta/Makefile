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
	-Wstrict-aliasing=3 \
	-Wswitch \
	-Wswitch-default \
	-Wunreachable-code \
	-Wunused \
	-Wvariadic-macros \
	-Wwrite-strings

CFLAGS += -I../inc $(WARNINGS)

DEPS = $(wildcard ../inc/*.h)
XMLDEPS = $(wildcard xml/*.xml)

OBJ = saimetadata.o saimetadatautils.o saiserialize.o

SYMBOLS = $(OBJ:=.symbols)

all: saisanitycheck saimetadatatest saiserializetest saidepgraph.svg $(SYMBOLS)
	./saimetadatatest >/dev/null
	./saiserializetest >/dev/null
	./saisanitycheck

CONSTHEADERS = saimetadatatypes.h saimetadatalogger.h saimetadatautils.h saiserialize.h

xml: $(DEPS) Doxyfile $(CONSTHEADERS)
	doxygen Doxyfile 2>&1 | perl -npe '$$e=1 if /warning/i; END{exit $$e}'
	touch xml

EXTRA = acronyms.txt aspell.en.pws *.pm *.cap

saimetadatatest.c saimetadata.c saimetadata.h: xml $(XMLDEPS) parse.pl $(CONSTHEADERS) $(EXTRA)
	perl parse.pl

HEADERS = saimetadata.h $(CONSTHEADERS)

%.o: %.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS)

saisanitycheck: saisanitycheck.o $(OBJ)
	gcc -o $@ $^

saimetadatatest: saimetadatatest.o $(OBJ)
	gcc -o $@ $^

saiserializetest: saiserializetest.o $(OBJ)
	gcc -o $@ $^

saidepgraphgen: saidepgraphgen.o $(OBJ)
	g++ -o $@ $^

%.o.symbols: %.o
	nm $^ | ./checksymbols.pl

saidepgraph.gv: saidepgraphgen
	./saidepgraphgen > $@

saidepgraph.svg: saidepgraph.gv
	dot -Tsvg saidepgraph.gv > $@

.PHONY: clean

clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak sai*.gv sai*.svg *.o.symbols
	rm -f saimetadata.h saimetadata.c saimetadatatest.c
	rm -f saisanitycheck saimetadatatest saiserializetest saidepgraphgen
	rm -rf xml html dist
