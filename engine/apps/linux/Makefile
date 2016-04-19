LIBNAME=librvrmeander
APPNAME=rm-meander
VERSION=1
DBGFLAGS=
LIBRARY=$(LIBNAME).so.$(VERSION)

VPATH=..
CC=g++
LIBS=-lrmprep -lrmio -lgsl -lgslcblas -lconfig++ -lrmmigration -lrmbankerosion -lrmhydrodynamics -lrmfilter -lrmutil
CFLAGS=-fPIC -c -Wall $(DBGFLAGS) -I../../../../gsl-1.10/include/ -I../../../../libconfig-1.3.1/include/ -I../../../../inih_r7/cpp
#OLD_LDFLAGS=-L../../rmprep/linux/ -L../../rmio/linux/ -L../../rmmigration/linux/ -L../../rmbankerosion/linux/ -L../../rmhydrodynamics/linux/ -L../../rmfilter/linux/ -L../../rmplotting/linux/ -L../../rmutil/linux/
LDFLAGS=-shared -Wl,-export-dynamic -Wl,-soname,$(LIBRARY) -L../../../../gsl-1.10/lib/ -L../../../../libconfig-1.3.1/lib/ -L../../../../inih_r7/lib -L.
SOURCES=config_file.cpp initialization.cpp interface.cpp method_ikeda.cpp printing.cpp rvrmeander.cpp
HEADERS=config_file.h enums.h error-codes.h interface.h method_ikeda.h rvrmeander.h
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(LIBRARY) $(APPNAME)

$(LIBRARY): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $@

$(APPNAME): console.o
	$(CC) $(LDFLAGS) $(LIBS) console.o -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(LIBRARY) $(APPNAME)

