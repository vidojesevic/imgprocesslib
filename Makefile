# Image Processing Library

CC = gcc
SRC = imgprocesslib.c
OUT = imgprocess

CFLAGS = -Wall
LDFLAGS = -lm
		 
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	rm -f $(OUT)
