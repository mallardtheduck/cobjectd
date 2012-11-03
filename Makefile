CC?=g++
CFLAGS=-c -Wall -std=gnu++0x
LDFLAGS=
LIBS=-lmeta -lboost_system -lboost_thread -lcobject-client -lprofiler
SOURCES=classregistry.cpp main.cpp runcall.cpp tcp_connection.cpp handlemessage.cpp objecthandle.cpp socketstream.cpp tcp_server.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=cobjectd
INSTALL_PATH?=/usr/local
INSTALL_DIR=$(INSTALL_PATH)/bin

all: $(SOURCES) $(TARGET)
	
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
install: all

clean: 
	rm -f $(TARGET) *.o

relink: rmtarget all

rmtarget:
	rm -f $(TARGET)
