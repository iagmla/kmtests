CC=gcc
CFLAGS=-O3 -std=c99 -w -lm
TARGET=kmtests
all: $(TARGET)

$(TARGET): kmtests.c
	$(CC) kmtests.c $(CFLAGS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
