rm -rf aclocal.m4 autom4te.cache stamp-h1 libtool configure config.* Makefile.in Makefile *~ .*~ *.swp .*.swp
rm -rf config/*
rm -rf .deps
rm -rf m4/*

for a in src
do
	rm -rf $a/Makefile 2>&1
	rm -rf $a/Makefile.in 2>&1
done
