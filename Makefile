# Image Processing Library
# See LICENSE file for copyright and license details.

CC = gcc
SRC = ipl.c prompt.c cli.c resize.c crop.c rotate.c
OUT = ipl
INSTALL_DIR = /usr/local/bin
# MAN_DIR = $(shell mandir)

CFLAGS = -g -Wall
LDFLAGS = -lm
		 
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

install: $(OUT)
	install -m 755 $(OUT) $(INSTALL_DIR)
	cp -v ipl.1 /usr/share/man/man1/
	chmod 644 /usr/share/man/man1/ipl.1

uninstall:
	rm -fv $(INSTALL_DIR)/$(OUT)
	rm -fv /usr/share/man/man1/ipl.1

clean:
	rm -fv $(OUT)
