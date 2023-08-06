# Image Processing Library

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

uninstall:
	rm -f $(INSTALL_DIR)/$(OUT)

clean:
	rm -f $(OUT)
