/* ncphclient.cpp is the program that will run on the server side in the public-key PHash scheme.
    Copyright (C) <2019>  <NCP-Hash Group>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    For any questions, you may contact NCP-Hash Group via opening an issue on https://github.com/ncp-hash/public-phash/issues
*/

#include <assert.h>

#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <stdint.h>
#include <errno.h>

// #define MAX 513 //256, 2-byte betas with one /n character at end 
#define MAX 65537 // 65,536 bytes plus one terminating byte
#define PORT 8080

#define TRUE 1
#define FALSE 0

#define SUCCESS 0
#define ABORT 1

#define BETA_SIZE 256 //each beta is 256 bytes
#define NUM_BETAS 256 //there are 256 betas in total
#define PAILLIER_KEY_SIZE 256 //this is 256 bytes


int min(int first, int second){

	if(first<second) return first;
	else if(first>=second) return second;
}

int server_init(void){

	int listening_socket_fd; 
	struct sockaddr_in serverAddress;

	listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (listening_socket_fd == -1) { 
		printf("error: socket not opened.\n"); 
		exit(0); 
	} 
	else
		printf("socket opened\n"); 


	bzero(&serverAddress, sizeof(serverAddress)); 

	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
	serverAddress.sin_port = htons(PORT); 

	if ((bind(listening_socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != SUCCESS) { 
		printf("socket bind failure\n"); 
		exit(0); 
	} 
	else
		printf("socket bind success\n"); 


	if ((listen(listening_socket_fd, 5)) != SUCCESS) { 
		printf("listen failure\n"); 
		exit(0); 
	} 
	else
		printf("listening\n");


	return listening_socket_fd; //don't forget to close this in main
}


int server_connect_to_client(int listening_socket_fd){

	struct sockaddr_in clientAddress; 
	unsigned int len; 
	int client_socket_fd;

	len = sizeof(clientAddress); 
	client_socket_fd = accept(listening_socket_fd, (struct sockaddr*)&clientAddress, &len);

	if (client_socket_fd < 0) { 
		printf("client acccept failed\n"); 
		exit(0); 
	} 
	else
		printf("client accepted\n"); 

	return client_socket_fd; //don't forget to close this in main
}


int client_connect_to_server(void){

	int socketFD;
	struct sockaddr_in serverAddress;

	// open new socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketFD == -1) { 
		printf("error: socket not opened\n"); 
		exit(0); 
	} 
	else
		printf("socket opened\n");

	// set address information of socket
	bzero(&serverAddress, sizeof(serverAddress)); 
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //TODO: find how to change IP so I can send messeges between computers
	//serverAddress.sin_addr.s_addr = inet_addr("172.20.10.4"); 
	serverAddress.sin_port = htons(PORT); 

	// tell client socket to connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) { 
		printf("error: could not connect to server\n"); 
		exit(0); 
	} 
	else
		printf("connected\n"); 

	return socketFD;
}

void print_bytes(void *buf,int num_bytes){

	unsigned char* charbuf = (unsigned char*)buf;

	for (int i = 0; i < num_bytes; i++){
		printf("%02x", charbuf[i]);
	}

	printf("\n");
}

/* Like read(), but retry on EINTR and EWOULDBLOCK,
 * abort on other errors, and don't return early. */
void xread(int fd, void *buf, size_t nBytes)
{
	do {
		ssize_t n = read(fd, buf, nBytes);
		if (n < 0 && errno == EINTR) continue;
		if (n < 0 && errno == EWOULDBLOCK) continue;
		if (n < 0) perror("read"), abort();
		buf = (char *)buf + n;
		nBytes -= n;
	} while (nBytes);
}

/* Like write(), but retry on EINTR and EWOULDBLOCK,
 * abort on other errors, and don't return early. */
void xwrite(int fd, const void *buf, size_t nBytes)
{
	do {
		ssize_t n = write(fd, buf, nBytes);
		if (n < 0 && errno == EINTR) continue;
		if (n < 0 && errno == EWOULDBLOCK) continue;
		if (n < 0) perror("write"), abort();
		buf = (const char *)buf + n;
		nBytes -= n;
	} while (nBytes);
}


//sends message, blocks for response. 
// will loop indefinitely without no recipient response
//re implement with forks

void send_char_string(int recipient_socket_fd, char* message, int32_t message_len){

	xwrite(recipient_socket_fd,&message_len,4);
	xwrite(recipient_socket_fd,message,message_len);
}


//recieves a character string and then returns a confirmation to sender
char* receive_char_string(int sender_socket_fd){

	int32_t message_len;
	xread(sender_socket_fd,&message_len,4);
	// char message[message_len];
	char* message = (char*)malloc((message_len) * sizeof(char));
	xread(sender_socket_fd,message,message_len);
	return message;
}

// ========================================================================================
// KeyFile Read and Write Utilities
// ========================================================================================

// These are written in c because there will be a parent process initialize the keyfiles and
// to start up the hash recieving server. This parent process will be written in c and 
// will use posix fork() calls.

// When the client and server are run on the same computer, the parent will first initialize
// the key files which both child processes (the hash recieving server and the client) will
// read from, but not write to. This solves a race condition issue experienced in a previous
// implementation, while also better simulating the separate nature of the client and server
// given that they'd be separate processes that will communicate exclusively through tcp.

// when client and server are on separate devices, both will still have access to the keyfiles.
// the client will have the keyfiles as part it's installation files and the server will hve a
// copy locally (since it generated the public keys in the first place.)

//what is the input type of the key itself? Is it void?
void write_paillier_key_file(void* key){

	char* file_path = "./paillier.key";

	FILE *fptr;
	fptr = fopen(file_path,"wb");

	if(fptr == NULL){
	  printf("Error!");   
	  exit(1);             
	}

	fwrite(key,1,PAILLIER_KEY_SIZE,fptr);
	
	//write null terminator to file
	fwrite("\0",1,1,fptr);
	fclose(fptr);
}


//to cast the betas returned from this function in the c++ code, do the following:
// std::vector<paillier_ciphertext_t> vector_name(ptr_to_return_key, ptr_to_return_key + number_of_elements);
// notice that the offset in the last argument is for the number of elements NOT the number of bytes.

//to cast the paillier public key returned from this function in c++ code, do this:
// paillier_ciphertext_t paillier_key = *ptr_to_return_key;

paillier_pubkey_t* read_paillier_key_file(void){

	int num_bytes = PAILLIER_KEY_SIZE + 1;
	char* file_path = "./paillier.key";

	//alocate memory for key to be returned
    FILE *key_file = fopen(file_path, "rb");  
    char *return_key = (char*)malloc(num_bytes); //allocates that many bytes

    //read key from file and close
    fread(return_key, 1, num_bytes, key_file);
    fclose(key_file); 

	return paillier_pubkey_from_hex(return_key);
}

// int send_char_string(int recipient_socket_fd, char* message, int message_len){

// 	char recipient_response[message_len];
// 	char* expected_response = (char*)malloc((message_len+1) * sizeof(char));
// 	strcpy(expected_response, "message received.\n"); //TODO: may need to use strncopy 
// 	//char expected_response[message_len] = "message received.\n";

// 	while(TRUE){

// 		write(recipient_socket_fd, message, message_len);
// 		printf("message sent:\n%s\n", message);

// 		bzero(recipient_response, message_len); 
// 		read(recipient_socket_fd, recipient_response, message_len);

// 		if ((strcmp(recipient_response, expected_response)) == 0) { 
// 			printf("message was confirmed as received.\n"); 
// 			return 0; 
// 		} 
// 	} 

// }


// //recieves a character string and then returns a confirmation to sender
// char* receive_char_string(int sender_socket_fd, int message_len){

// 	char* received_message = (char*)malloc((message_len+1) * sizeof(char));

// 	char* confirmation = (char*)malloc((message_len+1) * sizeof(char));
// 	strcpy(confirmation, "message received.\n"); //TODO: may need to use strncopy 

// 	char endOfTransmission;

// 	while (TRUE) { 

// 		bzero(received_message, message_len); 

// 		// get message from client; copy it to received_message buffer 
// 		read(sender_socket_fd, received_message, message_len); 
// 		printf("message received:\n%s\n", received_message);

// 		// and send that confirmation message to client 
// 		write(sender_socket_fd, confirmation, message_len); //may want to replace with rec or send avoid problems in large filetypes

// 		// if client string contains "\n" then server exit and chat ended.
// 		//endOfTransmission = received_message[strlen(received_message)-1]; 
// 		endOfTransmission = received_message[message_len-1]; 
// 		if (strcmp("\n", &endOfTransmission) == 0) { 
// 			printf("server quitting\n"); 
// 			break; 
// 		}
// 	} 

// 	free(confirmation);
// 	return received_message;
// }
