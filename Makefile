# Image Processing Library
# See LICENSE file for copyright and license details.

CC = gcc
SRC = imgprocesslib.c
OUT = imgprocess
INSTALL_DIR = /usr/local/bin

CFLAGS = -Wall
LDFLAGS = -lm
		 
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

install: $(OUT)
	install -m 755 $(OUT) $(INSTALL_DIR)
	cp imgprocess.1 /usr/local/share/man/man1/
	chmod 644 /usr/local/share/man/man1/imgprocess.1

uninstall:
	rm -f $(INSTALL_DIR)/$(OUT)

clean:
	rm -f $(OUT)
