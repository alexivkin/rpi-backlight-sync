
PREFIX	?= /usr/local
BINDIR	?= $(PREFIX)/bin
CFLAGS	+= -Wall -Wextra -Werror
LDLIBS	= -lX11 -lXss

PROG	= blsync
OBJS	= $(PROG).o

all: $(PROG)

install: all
	install $(PROG) $(DESTDIR)$(BINDIR)

clean:
	-rm -f $(PROG) $(OBJS) core $(PROG).core
