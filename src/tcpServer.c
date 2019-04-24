#include <stdio.h> //need this for fwrite
#include <unistd.h>

#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

// #define MAX 513 //256, 2-byte betas with one /n character at end 
#define MAX 65537 // 65,536 bytes plus one terminating byte
#define PORT 8080

#define TRUE 1
#define FALSE 0

// Function designed for chat between client and server. 
void getClientInput(int socketFD) 
{ 
	char clientEncHash[MAX]; 
	char confirmation[MAX] = "hash recieved.\n";
	char endOfTransmission;

	// infinite loop 
	while (TRUE) { 

		bzero(clientEncHash, MAX); 

		// get message from client; copy it to clientEncHash buffer 
		read(socketFD, clientEncHash, MAX); 

		// print buffer which contains the client contents 
		printf("client's encrypted hash: %s\t", clientEncHash); 
		
		// and send that confirmation message to client 
		write(socketFD, confirmation, MAX); //may want to replace with rec or send avoid problems in large filetypes

		// if client hash contains "\n" then server exit and chat ended.
//		endOfTransmission = clientEncHash[strlen(clientEncHash)-1]; 
		endOfTransmission = clientEncHash[MAX-1]; 
		if (strcmp("\n", &endOfTransmission) == 0) { 
			printf("server quitting\n"); 
			break; 
		}
	} 
} 

// Driver getClientInputtion 
int main() {

	int socketFD; //socket file descriptor
	int connectionFD; //connection file descriptor
	unsigned int len; 

	// sockaddr_in: a structure, defined in <netinet/in.h> stores information 
	// about the IPv4 socket we're creating including the
	// - Address family     
	// - Port number         
	// - IPv4 address  
	// - Padding (not sure what this part is for)
	
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress; 

	// AF_INET: Short for "Address Family, Internet." This constant refers 
	// to the IPv4 address family (eg 127.0.0.1 is an IPv4 address, while
	// IPv6 addresses look like 2001:db8:85a3:8d3:1319:8a2e:370:7348)

	// SOCK_STREAM: sockets declared with this constant send data as a stream
	// of bytes using TCP protocol. Sockets can also be declared to transport 
	// data as discrete datagrams, which would require the UDP protocol, and 
	// and the use of the SOCK_DGRAM defintion. Another alternative is to interface
	// directly with the network layer directly with SOCKET_RAW (although this 
	// means no transport layer TCP or UDP to protect against transmission problems 
	// or duplication.)

	// sockets are the c networking objects
	// ports are actual places in memory that are are used to store data for transmission
	// ports have numbers associated with them (eg port 80)
	// we assign a port to our socket 

	//open new socket and return file descriptor
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketFD == -1) { 
		printf("error: socket not opened.\n"); 
		exit(0); 
	} 
	else
		printf("socket opened\n"); 

	//bzero copies n=sizeof(serverAddress) zeros starting at pointer &serverAddress
	bzero(&serverAddress, sizeof(serverAddress)); 

	// now assign IP address and PORT number to serverAddress 
	serverAddress.sin_family = AF_INET; //address family set to ipv4
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
	// htonl stands for "host to network long." This translates the  NADDR_ANY 
	// ip from whatever format is used on your local machine  to big-endian 
	// network integer format.  

	// INADDR_ANY is a wildcard ip address that means any IP is acceptable. This 
	// makes our socket a listening socket. note that long here means 32 bit 
	// even though long usually means 64 bit. This is because this getClientInputtion was made in 
	// a time when long integers were 32 bits.

	// TODO: Ask Skeith this question: in this stack overflow post, it says 
	// https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
	// that INADDR_ANY "binds the socket to all available interfaces." What does this mean?
	// can the socket read and write data from any given port on the system?

	// TODO: why do we leave the ip address as a wildcard, but we set the port to the well-known 8080?
	// from the ip(7) man page: When INADDR_ANY is specified in the bind call,
       // the socket will be bound to all local interfaces.  When listen(2) is
       // called on an unbound socket, the socket is automatically bound to a
       // random free port with the local address set to INADDR_ANY.  When
       // connect(2) is called on an unbound socket, the socket is
       // automatically bound to a random free port or to a usable shared port
       // with the local address set to INADDR_ANY.

	// TODO: also who's ip is this anyway? Does this mean that the socket will talk with any client ip 
	// (not waiting for a specific ip) or that the socket being declared will take act on any message 
	// addressed to any ip that reaches this machine?

	//TODO: don't forget about the todo's in client.c

	//I defined PORT as 8080 earlier. 
	serverAddress.sin_port = htons(PORT); //htons stands for "host to network short." Short means 16 bit.

	// binding socket to IP address
	// this is so the client has a fixed
	// well-known address (ip + port 8080)
	if ((bind(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) { 
		printf("socket bind failure\n"); 
		exit(0); 
	} 
	else
		printf("socket bind success\n"); 

	// listen() tells the kernel we're willing to 
	// accept communications on the socket
	if ((listen(socketFD, 5)) != 0) { 
		printf("listen failure\n"); 
		exit(0); 
	} 
	else
		printf("listening\n"); 

	

	// accept() will accept data  from client 
	// accept() will block until it recieves data from client

	// note that accept() creates a NEW socket and returns a file descriptor; 
	// it's this new socket that is connected to the client socket that 
	// ran connect() in client.c .

	// so our application is listening to socketFD to get information to create
	// connectionFD's socket. It's through connectionFD that we'll recieve the 
	// actual message.

	// the argument len passes the length of the clientAddress accept()
	// but len is passed by reference because after accept() is run, 
	// len is reassigned to the number of bytes of data actually copied into the
	// into the clientAddress buffer

	//also note that, if we're not interested in keeping the client address data
	// (for privacy's sake) we can replace (struct sockaddr*)&clientAddress with null
	// and replace &len with 0 (although I haven't tested this yet)

	len = sizeof(clientAddress); 
	connectionFD = accept(socketFD, (struct sockaddr*)&clientAddress, &len); //while true accept connections, then fork (or make a new thread) to run getCLientInput
	if (connectionFD < 0) { 
		printf("client acccept failed\n"); 
		exit(0); 
	} 
	else
		printf("client accepted\n"); 

	// Function for chatting between client and server 
	getClientInput(connectionFD); 

	// After chatting close the socket 
	close(socketFD); 
} 
//TODO: find a new way to terminate server
//figure out the serialization for gmp types
//you need to consider the potential sources of
// error and consider them in a switch statement in my code when serializing data
//don't dive too deep, there are lots of error possibilites.