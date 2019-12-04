# Variable Assignments
OBJECTS := server.o client.o libClient.o libServer.o libTCP.o libNCPH.o
Linux_Linker_Flags    := -lpaillier -lX11 -lgmp -lm -lpthread -lGraphicsMagick
Mac_Linker_Flags   := /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
OS := $(shell uname -s)
JAVA_HOME := $(shell readlink -e "$$(dirname "$$(readlink -e "$$(which javac)")")"/..)
JNI_Linker_Flags := -shared -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -fPIC

# Begin Build Process
.PHONY : all
all : ClientWrapper.so client server

# Build the JNI C header
com_nchash_view_CppHook.h: ../view/CppHook.java
	javac -h . ../view/CppHook.java

# Compile and link Hooks from CPP to Java
ClientWrapper.so: ClientWrapper.cpp com_nchash_view_CppHook.h
	g++ -o $@ $< $(JNI_Linker_Flags)

# Link object files to create executable client
client: client.o libClient.o libTCP.o libNCPH.o 
ifeq ($(OS),Linux)
	g++ -o $@ $^ $(Linux_Linker_Flags) 
endif
ifeq ($(OS),Darwin)
	g++ -o $@ $^ $(Mac_Linker_Flags)
endif

# Link object files to create executable server
server: server.o libServer.o libTCP.o libNCPH.o 
ifeq ($(OS),Linux)
	g++ -o $@ $^ $(Linux_Linker_Flags)
endif
ifeq ($(OS),Darwin)
	g++ -o $@ $^ $(Mac_Linker_Flags)
endif

# Compile tcp functions to object file
tcp.o: tcp.cpp
	g++ -c -o $@ $^

$(OBJECTS): %.o: %.cpp
ifeq ($(OS),Linux)
	g++ -c -o $@ $^
endif
ifeq ($(OS),Darwin)
	g++ -c -o $@ $^ -I/usr/X11R6/include 
endif

# Erase all changes
.PHONY : clean
clean :
	rm -f client server com_nchash_view_CppHook.h *.o *.key *.so
