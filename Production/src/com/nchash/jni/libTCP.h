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
#include <gmp.h>
#include <gmpxx.h>
extern "C" {
	#include <paillier.h>
}

int min(int first, int second);

int server_init(void);

int server_connect_to_client(int listening_socket_fd);

int client_connect_to_server(void);

void print_bytes(void *buf,int num_bytes);

void xread(int fd, void *buf, size_t nBytes);

void xwrite(int fd, const void *buf, size_t nBytes);

void mpz_from_bytes(mpz_class& r, const unsigned char* inBuf, size_t l);

void bytes_from_mpz(unsigned char*& outBuf, size_t* l, const mpz_class& n);

void send_char_string(int recipient_socket_fd, char* message, int32_t message_len);

char* receive_char_string(int sender_socket_fd);

void write_paillier_key_file(void* key);

paillier_pubkey_t* read_paillier_key_file(void);
