# The Object Files we need
OBJECTS := server.o client.o libServer.o libTCP.o

# The position independent object files we need to compile libClientWrapper.so
PI_OBJECTS := libClient.o libNCPH.o

# Compiler Flags
Linux_Linker_Flags    := -lpaillier -lX11 -lgmp -lm -lpthread -lGraphicsMagick
Mac_Linker_Flags   := /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11

# Is the OS MAC or Linux?
OS := $(shell uname -s)

# Setting the JAVA HOME to know where the JVM is
# Also setting the jni linker flags that will be needed for libClientWrapper.so
ifeq ($(OS),Linux)
	JAVA_HOME := $(shell readlink -e "$$(dirname "$$(readlink -e "$$(which javac)")")"/..)
	JNI_Linker_Flags := -shared -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux" -fPIC 
endif
ifeq ($(OS),Darwin)
	JAVA_HOME :=  $(shell /usr/libexec/java_home)
	JNI_Linker_Flags := -shared -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/darwin" -I/opt/X11/include -fPIC 
endif



# Begin Build Process
.PHONY : all
all : libClientWrapper.so client server

# Build the JNI C header
com_nchash_view_CppHook.h: ../view/CppHook.java
	javac -h . ../view/CppHook.java

# Object code intended for use in a shared library must be ‘position-independent’, 
# meaning that it can execute without first being modified to account for where 
# it has been loaded in memory.

# Need these position independent objects for client libClientWrapper.so
$(PI_OBJECTS): %.o: %.cpp
ifeq ($(OS),Linux)
	g++ -fPIC -c -o $@ $^ 
endif
ifeq ($(OS),Darwin)
	g++ -fPIC -c -o $@ $^ -I/usr/X11R6/include 
endif

# Compile libClientWrapper.so this will allow java to be a GUI client
libClientWrapper.so: ClientWrapper.cpp com_nchash_view_CppHook.h $(PI_OBJECTS)
	g++ -shared -o $@ $(PI_OBJECTS) $(JNI_Linker_Flags) $(Linux_Linker_Flags)

# Link object files to create command line client (can be used instead of GUI client)
client: client.o libClient.o libTCP.o libNCPH.o 
ifeq ($(OS),Linux)
	g++ -o $@ $^ $(Linux_Linker_Flags) 
endif
ifeq ($(OS),Darwin)
	g++ -o $@ $^ $(Mac_Linker_Flags)
endif

# Link object files to create command line server
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
