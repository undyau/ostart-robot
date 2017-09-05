CC=g++
CFLAGS=-Wall -g -std=gnu++0x -c
LDFLAGS=
SOURCES=$(wildcard *.cxx)
OBJECTS=$(SOURCES:.cxx=.o)

startrobot: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

all: startrobot

%.o: %.cxx
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f $(OBJECTS) *.gch
