CC=gcc
CCFLAGS=-Wall
LDFLAGS=
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=observe

all: 
	gcc -fPIC -shared config.h -o bin/rsrtic.so src/observe.c src/transmit.c src/models.c src/clientmanager.c observe.h transmit.h