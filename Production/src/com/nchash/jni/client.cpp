/* ncphclient.cpp is the program that will run on the client side in the public-key PHash scheme.
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

// #include "ncph.h"
// #include "tcp.h"
#include "libClient.h"


//TODO: Where should I put this function? It uses c++ classes (gmp) and it uses the 
// c socket communications. So, I'm not sure where it should go. What really is the 
// difference between compiling c++ vs. c. Is it alright to mix and match in this way?

//TODO: Why is it that receive_char_byte_string is in scope when I use it in main, but if I declare another function
// as I have done here, that I get the error: 
    // client.cpp: In function ‘void receive_mpz_t(int, mpz_class)’:
    // client.cpp:36:71: error: ‘receive_char_byte_string’ was not declared in this scope
    //  r_bytes_read = *((size_t*)receive_char_byte_string(tcp_socket)); 

// TODO: If I put the receive_mpz_t() function in ncph.hpp, I also get the same error. 
// ncph.hpp:415:71: error: ‘receive_char_byte_string’ was not declared in this scope

// TODO: Function call on line 158 of this file.

// TODO: Actually, I used the mpz_class in the mpz_from_bytes() function in tcp.c
//  why am I allowed to use a C++ type, like mpz in a c file? Does g++ not differentiate
//  between files by their extensions (c or cpp.) How does it know the difference between
//  c code and c++ code?

// write an mpz_t value received over tcp_socket to the mpz_t_destination
// note: intialize the mpz_t value
// void receive_mpz_t(int tcp_socket, mpz_class mpz_t_destination){

//     //receive number of bytes read
//     size_t number_bytes_read;
//     number_bytes_read = *((size_t*)receive_char_byte_string(tcp_socket)); //TODO: need to convert from network byte order before reading
//     printf("\nnumber of bytes read:%zu\n", number_bytes_read);

//     // declare empty byte string to receive data
//     unsigned char* byte_string = (unsigned char*)malloc((number_bytes_read) * sizeof(unsigned char));

//     // receive data over tcp
//     byte_string = receive_char_byte_string(tcp_socket);

//     // convert bytes to mpz_t 
//     mpz_from_bytes(mpz_t_destination, byte_string, number_bytes_read);

//     // store in the struct mpz_t_destination 
//     gmp_printf("received challnge bit vector: %Zd\n",mpz_t_destination.get_mpz_t());

// }

using std::vector;

// int client(char* file_name[]) {

//     // ========================================================================================
//     // GENERATING RHO_SUMS
//     // ========================================================================================


//     // CImg<float> src("../img/GrayLenna.bmp");
//     // CImg<float> img = get_grayscale(src);
//     // CImg<float> low_pass_filter(9, 9, 1, 1, 1.0/9.0);
//     // img = apply_filter(img, low_pass_filter);

//     // CImg<float> polar = polar_FFT(img);

//     // // float *message1 = sum_along_rho1(polar);
//     // std::vector<int> rho_sums = sum_along_rho(polar);

//     vector<int> rho_sums = preproc_radial_sums(file_name[1]);


//     /* PARAMETERS & CONSTRAINTS: 
//         * arr_size = how many possible rhos there can be based on the input image I size, integer; arr_size = len(I)/2> 0
//     */
//     int arr_size = 256;

//     // ========================================================================================
//     // READ THE PUBLIC KEY AND ENCRYPTED BETAS
//     // ========================================================================================
//     // Read public key from disk and initialize it

//     std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//     paillier_pubkey_t* pu = read_paillier_key_file();

//     std::vector<paillier_ciphertext_t*> read_betas = read_enc_betas_from_key_file(pu);

//     // ========================================================================================
//     // CLIENT'S ENCRYPTED MULTIPLICATION AND SUMMATION (= ENCRYPTION OF HASH)
//     // ========================================================================================

//     paillier_ciphertext_t* enc_sum_res = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
//     mult_and_sum(pu, enc_sum_res, read_betas, rho_sums);

//     // ========================================================================================
//     // === ZKP OUTLINE ===
//     // ========================================================================================
   
//     // Compute A ~ Encryption of a hash based on random values instead of rho_sum
//     std::vector<mpz_class> rands;
//     for (int i = 0; i < arr_size; ++i) {
//         gmp_randstate_t state;
//         gmp_randinit_mt (state);
//         mpz_class r;
//         mpz_urandomm(r.get_mpz_t(), state, pu->n);
//         rands.push_back(r);
//         // mpz_clear(r.get_mpz_t()); 
//     }

//     paillier_ciphertext_t* A = paillier_create_enc_zero();
//     mult_and_sum(pu, A, read_betas, rands);

//     /* EXPORT TO BYTESTRING */
//     char* char_A_w = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, A);

    
//     // ========================================================================================
//     // === ZKP TCP ===
//     // ========================================================================================
  
//     // ========================================================================================
//     // SEND ENCRYPTED HASH AND RANDOM HASH A TO SERVER
//     // ========================================================================================

//     char* enc_hash_string = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
//     int tcp_socket =  client_connect_to_server();
//     send_char_string(tcp_socket, enc_hash_string, PAILLIER_BITS_TO_BYTES(pu->bits)*2);

//     printf("\nencrypted hash sent: ");///
//     print_bytes(enc_hash_string,256);///

//     // ========================================================================================    
//     // SEND A TO SERVER
//     // ========================================================================================

//     send_char_string(tcp_socket, char_A_w, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
//     printf("\n random hash (A) sent: ");///
//     print_bytes(char_A_w,256);///

//     // ========================================================================================
//     // === ZKP ADDITIONAL IPC ===
//     // ========================================================================================
    
//     // Receive challenge integer C from server
//     mpz_class read_C;
//     mpz_init(read_C.get_mpz_t());

//     //receive_mpz_t(tcp_socket,read_C);

//     size_t number_bytes_read;
//     number_bytes_read = *((size_t*)receive_char_string(tcp_socket)); //TODO: need to convert from network byte order before reading
//     printf("\nnumber of bytes read:%zu\n", number_bytes_read);

//     // allocate C as char string and receive challenge bit
//     unsigned char* C_string = (unsigned char*)malloc((number_bytes_read) * sizeof(unsigned char));
//     C_string = (unsigned char*)receive_char_string(tcp_socket);
//     mpz_from_bytes(read_C, C_string, number_bytes_read);

//     gmp_printf("received challnge bit vector: %Zd\n",read_C.get_mpz_t());

    
// ////////

//     // ========================================================================================
//     // === ZKP OUTLINE ===
//     // ========================================================================================
//     /*
//     *   Generate vector S:
//     */
//     printf("generating S vector (line 180)\n");
//     std::vector<mpz_class> S;
//     for (int i = 0; i < arr_size; ++i) {

//         mpz_class rho_mpz;
//         mpz_set_ui (rho_mpz.get_mpz_t(),  rho_sums[i]);

//         mpz_class C_rho;
//         mpz_init(C_rho.get_mpz_t());
//         mpz_mul(C_rho.get_mpz_t(), read_C.get_mpz_t(), rho_mpz.get_mpz_t());

//         // mpz_clear(read_C.get_mpz_t()); 
//         // mpz_clear(rho_mpz.get_mpz_t());

//         mpz_class sum_mpz;
//         mpz_init(sum_mpz.get_mpz_t());
//         mpz_add(sum_mpz.get_mpz_t(), rands[i].get_mpz_t(), C_rho.get_mpz_t());

//         // mpz_clear(C_rho.get_mpz_t());

//         mpz_class s_i;
//         mpz_init(s_i.get_mpz_t());
//         mpz_mod(s_i.get_mpz_t(), sum_mpz.get_mpz_t(), (pu->n));
//         S.push_back(s_i);
//     }

//     // ========================================================================================
//     // === ZKP ADDITIONAL IPC ===
//     // ========================================================================================

//     /*
//     *   Send vector S to server
//     */

//     printf("sending S vector (line 214)\n");
//     for (int i = 0; i < arr_size; ++i){

//         //find number of bytes needed to allocate S[i]
//         size_t S_i_string_length = mpz_size(S[i].get_mpz_t()) * sizeof(mp_limb_t); 
//         size_t * len_ptr = &S_i_string_length;



//         // printf("\nnumber of bytes sent: %lu",S_i_string_length);
//         // gmp_printf("sending challenge bit vector: %Zd\n",S[i].get_mpz_t()); 

//         //allocate S[i] as char string  
//         unsigned char* S_i_string = (unsigned char*)malloc((S_i_string_length) * sizeof(unsigned char));
//         bytes_from_mpz(S_i_string, len_ptr, S[i]);///

//         send_char_string(tcp_socket, (char*)len_ptr, sizeof(size_t));//need to convert to network byte order before sending
//         send_char_string(tcp_socket, (char*)S_i_string, S_i_string_length);

//         // free(S_i_string);
//     }
    
    


// ////////

//      /* CLEANUP AND SOCKET CLOSE*/

//     close(tcp_socket);

//     paillier_freeciphertext(enc_sum_res);
//     paillier_freeciphertext(A);
//     for (int i = 0; i < arr_size; ++i) {
//         paillier_freeciphertext(read_betas[i]);
//     }
//     paillier_freepubkey(pu);
//     free(enc_hash_string);

//     return 0;

// }
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {

    // ========================================================================================
    // GENERATING RHO_SUMS
    // ========================================================================================

    // if (argc < 2) {
    //     fprintf(stderr, "usage: %s <image_1> ... <image_n>\n", argv[0]);
    //     exit(1);
    // }

    std::string image_path;
    char const *txt_file = "../image_path.txt";
    while (!get_image_path(txt_file, image_path)) {
        std::cout << "Sleeping...\n";
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    const char* img_path = image_path.c_str();
    // std::cout << "img_path = " << image_path << std:: endl;
    std::cout << client(img_path) << std::endl;

    // if (get_image_path(txt_file, image_path)) {
    //     // std::cout << client(argv[1]) << std::endl;
    //     const char* img_path = image_path.c_str();
    //     // std::cout << "img_path = " << image_path << std:: endl;
    //     std::cout << client(img_path) << std::endl;
    // } else {
    //     std::cout << "File not found...\n";
    // }

    return 0;
}
