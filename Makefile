CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = airport
SOURCES = main.c lab5.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = lab5.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run