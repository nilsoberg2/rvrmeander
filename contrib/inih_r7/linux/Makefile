LIBNAME=libinih
VERSION=1
DBGFLAGS=
LIBRARY=$(LIBNAME).so.$(VERSION)

OUTPATH=.
VPATH=..
CC=g++
C=gcc
CFLAGS=-fPIC -c -Wall $(DBGFLAGS)
LDFLAGS=-shared -Wl,-export-dynamic -Wl,-soname,$(LIBRARY)
SOURCES=ini.c cpp/INIReader.cpp
HEADERS=ini.h cpp/INIReader.h
OBJECTS=ini.o INIReader.o

all: $(SOURCES) $(LIBRARY) link_lib

$(LIBRARY): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

cpp/INIReader.o : cpp/INIReader.cpp
	$(CC) $(CFLAGS) $< -o INIReader.o

ini.o : ini.c
	$(C) $(CFLAGS) $< -o $@

link_lib:
	ln -s $(LIBRARY) $(LIBNAME).so

clean:
	rm -rf *.o $(LIBRARY) $(LIBNAME).so

