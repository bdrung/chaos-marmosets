VERSION = 0.1.2

CC ?= gcc
CFLAGS ?= -std=gnu99 -Wall -Wextra -Werror -O3
PREFIX ?= /usr

all: divide-by-zero leak-memory seg-fault doc

%: %.c noinline.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^

check:
	./divide-by-zero-python 2>&1 | grep -q ^ZeroDivisionError

clean:
	rm -f divide-by-zero leak-memory seg-fault *.1

%.1: %.1.md
	pandoc -s -t man $^ -o $@

doc: divide-by-zero.1 divide-by-zero-python.1 leak-memory.1 seg-fault.1

install: divide-by-zero leak-memory seg-fault
	install -D -m 755 divide-by-zero $(DESTDIR)$(PREFIX)/bin/divide-by-zero
	install -D -m 755 divide-by-zero-python $(DESTDIR)$(PREFIX)/bin/divide-by-zero-python
	install -D -m 755 leak-memory $(DESTDIR)$(PREFIX)/bin/leak-memory
	install -D -m 755 seg-fault $(DESTDIR)$(PREFIX)/bin/seg-fault
	install -D -m 644 divide-by-zero.1 $(DESTDIR)$(PREFIX)/share/man/man1/divide-by-zero.1
	install -D -m 644 divide-by-zero-python.1 $(DESTDIR)$(PREFIX)/share/man/man1/divide-by-zero-python.1
	install -D -m 644 leak-memory.1 $(DESTDIR)$(PREFIX)/share/man/man1/leak-memory.1
	install -D -m 644 seg-fault.1 $(DESTDIR)$(PREFIX)/share/man/man1/seg-fault.1

version:
	@echo $(VERSION)

%.asc: %
	gpg --armor --batch --detach-sign --yes --output $@ $^

%.tar.xz: divide-by-zero.1.md divide-by-zero.c divide-by-zero-python.1.md divide-by-zero-python leak-memory.1.md leak-memory.c LICENSE Makefile NEWS noinline.h README.md seg-fault.1.md seg-fault.c
	tar -cJf $@ --transform 's,^,chaos-marmosets-$(VERSION)/,' $^

dist: chaos-marmosets-$(VERSION).tar.xz chaos-marmosets-$(VERSION).tar.xz.asc

.PHONY: all check clean dist doc install version
