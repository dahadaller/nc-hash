# Variable Assignments
SOURCES := $(wildcard *.cpp)  # SOURCES is a list of all our .cpp files
OBJECTS := $(OBJECTS:.cpp=.o) #OBJECTS is list of all object files by text replacement (Section 6.3)
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

# # Implicitly finds each .cpp file with matching name 
# #  and compiles into object file (no linking) with
# # "cc -c blah.cpp -o blah.o"
# tcp.o client.o server.o:

$(OBJECTS): $(SOURCES)
ifeq ($(UNAME_S),Linux)
	g++ -c $^
endif
ifeq ($(UNAME_S),Darwin)
	g++ -I/usr/X11R6/include -c $^
endif

.PHONY : clean
clean :
	rm -f client server *.o *.key