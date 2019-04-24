// functions to implement
// client_connect_to_server
// client_
// client_recieve_public_key (paillier)

// server_send_betas
// client_recieve_betas

// client_send_enc_hash
// server_recieve_enc_hash

//******************************************************************************************


#include <stdio.h> 
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

#define SUCCESSS 0
#define ABORT 1


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

	if ((bind(listening_socket_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) { 
		printf("socket bind failure\n"); 
		exit(0); 
	} 
	else
		printf("socket bind success\n"); 


	if ((listen(listening_socket_fd, 5)) != 0) { 
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


//sends message, blocks for response. 
// will loop indefinitely without no recipient response
//re implement with forks
//returns 0 on success
int send_char_string(int recipient_socket_fd, char* message, int message_len){

	char recipient_response[message_len];
	char expected_response[message_len] = "message received.\n";

	while(TRUE){

		write(recipient_socket_fd, message, message_len);
		printf("message sent:\n%s\n", message);

		bzero(recipient_response, message_len); 
		read(socketFD, recipient_response, message_len);

		if ((strcmp(recipient_response, expected_response)) == 0) { 
			printf("message received.\n"); 
			return 0; 
		} 
	} 

}


void receive_char_string(int sender_socket_fd, char* message){

	char clientEncHash[MAX]; 
	char confirmation[MAX] = "message received.\n";
	char endOfTransmission;

	while (TRUE) { 

		bzero(clientEncHash, MAX); 

		// get message from client; copy it to clientEncHash buffer 
		read(sender_socket_fd, clientEncHash, MAX); 
		
		// and send that confirmation message to client 
		write(sender_socket_fd, confirmation, MAX); //may want to replace with rec or send avoid problems in large filetypes

		// if client string contains "\n" then server exit and chat ended.
		//endOfTransmission = clientEncHash[strlen(clientEncHash)-1]; 
		endOfTransmission = clientEncHash[MAX-1]; 
		if (strcmp("\n", &endOfTransmission) == 0) { 
			printf("server quitting\n"); 
			break; 
		}
	} 
	/************************* 
		need to return client
		input here
	**************************/

}

//returns 0 on success (positive confirmation)
int server_send_paillier_pubkey(int client_socket_fd, char* paillier_pubkey, int paillier_pubkey_len){

	ret_val = send_char_string(client_socket_fd, paillier_pubkey, paillier_pubkey_lenl);
	return ret_val;
} 

int send_bytes_chunk(int recipient_socket_fd, void *chunk_buffer, int chunk_len){

    unsigned char *pbuf = (unsigned char *) chunk_buffer;

    while (chunk_len > 0){

        int num = send(recipient_socket_fd, pbuf, chunk_len, 0);
        if (num == SOCKET_ERROR){

            if (WSAGetLastError() == WSAEWOULDBLOCK){

                // optional: use select() to check for timeout to fail the send
                continue;
            }
            return ABORT;
        }

        pbuf += num;
        chunk_len -= num;
    }

    return SUCCESS;
}


int send_bytes_size(int recipient_socket_fd, long bytes_size){

    bytes_size = htonl(bytes_size);
    return send_bytes_chunk(recipient_socket_fd, &bytes_size, sizeof(bytes_size));
}


int send_bytes_all(int recipient_socket_fd, void* all_bytes, int all_bytes_len){

    unsigned char *bytes_buffer = (unsigned char *) all_bytes;


    if (send_bytes_len(recipient_socket_fd, all_bytes_len) == ABORT)
        return ABORT;

    if (all_bytes_len > 0)
    {
        char chunk_buffer[1024];
        do
        {
            unsigned int num_bytes_read = min(all_bytes_len, sizeof(chunk_buffer));

            if (num_bytes_read < 1)
                return ABORT;
            
            strncpy(chunk_buffer, bytes_buffer, num_bytes_read);

            if (send_bytes_chunk(recipient_socket_fd, chunk_buffer, num_bytes_read) == ABORT)
                return ABORT;

            all_bytes_len -= num_bytes_read;
            all_bytes += num_bytes_read;
        }
        while (all_bytes_len > 0);

    }
    return SUCCESS;
}



int receive_bytes_chunk(int sender_socket_fd, void *chunk_buffer, int chunk_buffer_len){

    unsigned char *pchunk_buffer = (unsigned char *) chunk_buffer;

    while (chunk_buffer_len > 0){

        int num_bytes_received = recv(sender_socket_fd, pchunk_buffer, chunk_buffer_len, 0);
        if (num_bytes_received == SOCKET_ERROR){

            if (WSAGetLastError() == WSAEWOULDBLOCK){

                // optional: use select() to check for timeout to fail the read
                continue;
            }
            return ABORT;
        }
        else if (num_bytes_received == 0)
            return ABORT;

        pchunk_buffer += num_bytes_received;
        chunk_buffer_len -= num_bytes_received;
    }

    return SUCCESS;
}


int receive_bytes_len(int sender_socket_fd, long *bytes_len){

    if (receive_bytes_chunk(sender_socket_fd, bytes_len, sizeof(bytes_len)) == ABORT)
        return ABORT;

    *bytes_len = ntohl(*bytes_len);
    return SUCCESS;
}

int receive_bytes_all(int sender_socket_fd, void* all_bytes){

    unsigned char *bytes_buffer = (unsigned char *) all_bytes;

    long all_bytes_len;
    long offset = 0;

    if (receive_bytes_len(sender_socket_fd, &all_bytes_len) == ABORT)
        return ABORT;

    if (all_bytes_len > 0){

        char chunk_buffer[1024];

        do{
            int num_bytes_received = min(all_bytes_len, sizeof(chunk_buffer));

            if (receive_bytes_chunk(sender_socket_fd, chunk_buffer, num_bytes_received) == ABORT)
                return ABORT;

            memcopy(all_bytes + offset, chunk_buffer, num_bytes_received)
            offset += num_bytes_received
            all_bytes_len -= num_bytes_received;

        } while (all_bytes_len > 0);
    }
    return SUCCESS;
}