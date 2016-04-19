LIBNAME=librmio
VERSION=1
DBGFLAGS=
LIBRARY=$(LIBNAME).so.$(VERSION)

OUTPATH=../../apps/linux/
VPATH=..
CC=g++
CFLAGS=-fPIC -c -Wall $(DBGFLAGS)
LDFLAGS=-shared -Wl,-export-dynamic -Wl,-soname,$(LIBRARY)
SOURCES=rmfile.cpp  rmio.cpp  rmseries.cpp rmlog.cpp
HEADERS=rmfile.h  rmio.h  rmseries.h rmlog.h
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(LIBRARY) link_lib copy_lib

$(LIBRARY): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

link_lib:
	ln -s $(LIBRARY) $(LIBNAME).so

copy_lib:
	cp $(LIBNAME).so $(OUTPATH)

clean:
	rm -rf *.o $(LIBRARY) $(LIBNAME).so

