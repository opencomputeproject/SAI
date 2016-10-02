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

all: saisanitycheck
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

.PHONY: clean

clean:
	rm -f *.o *~ .*~ *.tmp .*.swp .*.swo *.bak saisanitycheck saimetadata.h saimetadata.c
	rm -rf xml html dist
