TARGET = spa
LIBS = -lallegro -lallegro_color -lglib-2.0
CC = gcc
CFLAGS = -g -Wall -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -std=c99

.PHONY: clean all default

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
