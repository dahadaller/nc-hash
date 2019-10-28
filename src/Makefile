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

# possible problems to address with Bon
# 1. maybe uname -s doens't return Darwin on MacOS catalina
# 2. -I/usr/X11R6/include  might need to be put in at a later stage; 
	# it doesn't make sense that it should be invoked when creating the 
	# OBJECT file.
# javac -h . native.java // generate header
# 3. Add the following to the makefile. You will need autoconf to include java
# // copy function prototype from header into .cpp file
# // compile cpp file as shared object library
# g++ -shared -I/usr/lib/jvm/java-12-openjdk-amd64/include/linux -I/usr/lib/jvm/java-12-openjdk-amd64/include -o libNative.so -fPIC libNative.cp