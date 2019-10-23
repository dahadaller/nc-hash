# Variable Assignments
OBJECTS := client.o server.o ncph.o #OBJECTS is list of all object files by text replacement (Section 6.3)
LDADD_Linux    := -lpaillier -lX11 -lgmp -lm -lpthread -lGraphicsMagick
LDADD_Darwin   := /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
UNAME_S := $(shell uname -s)

.PHONY : all
all : client server

client: client.o tcp.o ncph.o 
ifeq ($(UNAME_S),Linux)
	g++ -o $@ $^ $(LDADD_Linux)
endif
ifeq ($(UNAME_S),Darwin)
	g++ -o $@ $^ $(LDADD_Darwin)
endif

server: server.o tcp.o ncph.o 
ifeq ($(UNAME_S),Linux)
	g++ -o $@ $^ $(LDADD_Linux)
endif
ifeq ($(UNAME_S),Darwin)
	g++ -o $@ $^ $(LDADD_Darwin)
endif

tcp.o: tcp.cpp
	g++ -c -o $@ $^

$(OBJECTS): %.o: %.cpp
ifeq ($(UNAME_S),Linux)
	g++ -c -o $@ $^
endif
ifeq ($(UNAME_S),Darwin)
	g++ -c -o $@ $^ -I/usr/X11R6/include 
endif

.PHONY : clean
clean :
	rm -f client server *.o *.key