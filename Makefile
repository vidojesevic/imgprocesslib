# Image Processing Library
# See LICENSE file for copyright and license details.

CC = gcc
SRC = ipl.c prompt.c cli.c resize.c
OUT = ipl
INSTALL_DIR = /usr/local/bin

CFLAGS = -Wall
LDFLAGS = -lm
		 
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

install: $(OUT)
	install -m 755 $(OUT) $(INSTALL_DIR)
	cp ipl.1 /usr/local/share/man/man1/
	chmod 644 /usr/local/share/man/man1/ipl.1

uninstall:
	rm -f $(INSTALL_DIR)/$(OUT)
	rm -f /usr/local/share/man/man1/ipl.1

clean:
	rm -f $(OUT)
