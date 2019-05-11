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

#include "ncph.hpp"

int main() {
    /* PARAMETERS & CONSTRAINTS: 
        * beta_mean - real number
        * beta_std - real number, beta_std > 0
        * arr_size = how many possible rhos there can be based on the input image I size, integer; arr_size = len(I)/2> 0
        * rho_num = how many radii are used in the sum for Scheme 2, integer; rho_num >= 1
        * min_rho = minimal acceptable radius (in pixels) for uniform distribution of rho variable in the Scheme 2; min_rho >= 0
        * max_rho = maximal possible radius for uniform distribution; arr_size >= max_rho >= min_rho >= 0
        * paillier_n = security parameter, number os bits in the modulo, integer; currently, has to be multiple of 4 for ease of inter-process communication
    */

   float beta_mean = 50.0, beta_std = 10.0;
    int arr_size = 256;
    int max_rho = 255, min_rho = 60;
    int rho_num = 3;
    int paillier_n = 1024;

    paillier_pubkey_t* pu;                                  // the public key
    paillier_prvkey_t* pr;                                  // the private key 
    paillier_keygen(paillier_n, &pu, &pr, paillier_get_rand_devurandom);

    // ========================================================================================
    // WRITE THE PUBLIC PAILLIER KEY TO FILE (DO ONCE)
    // ========================================================================================
    char* hex_pk = paillier_pubkey_to_hex(pu);
    printf("public key written: %s\n",hex_pk);
    write_paillier_key_file(hex_pk);
    
    // ========================================================================================
    // WRITE BETA ARRAY ENCRYPTION TO FILE (DO ONCE)
    // ========================================================================================

    std::vector<float> betas (arr_size, 0); // initialize a 0-vector and populate it with (rho_num) of betas
    get_rho_beta_arr(rho_num, betas, min_rho, max_rho, beta_mean, beta_std);
    std::vector<paillier_ciphertext_t*> enc_betas = encrypt_betas(betas, pu);
    write_enc_betas_to_key_file(enc_betas, pu);

    // ========================================================================================
    // Receive Hash Over TCP
    // ========================================================================================
        
    int listening_socket =  server_init();
    int hash_socket = server_connect_to_client(listening_socket);

    char* enc_hash_string = receive_char_string(hash_socket);

    paillier_ciphertext_t* enc_hash = paillier_ciphertext_from_bytes((void*)enc_hash_string, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    paillier_plaintext_t* hash = paillier_dec(NULL, pu, pr, enc_hash);
    gmp_printf("Decrypted hash: %Zd\n", hash);

    // // ========================================================================================
    // // === ZKP ADDITIONAL IPC ===
    // // ========================================================================================
    // /*
    // *   Receive and import the A from bytestring along with the real hash.
    // */
    // // ========================================================================================
    // // === ZKP OUTLINE ===
    // // ========================================================================================
    // /*
    // *   Create challenge request C:
    // * 
    // *   int C = generate random integer from 0 to n;
    // */
    // // ========================================================================================
    // // === ZKP ADDITIONAL IPC ===
    // // ========================================================================================
    // /*
    // *   Send C to client
    // */
    // // ========================================================================================
    // /*
    // *   Receive vector S in response to C
    // */
    // // ========================================================================================
    // // === ZKP OUTLINE ===
    // // ========================================================================================
    // /*
    // *   Do the check:
    // *   int flag = 1;
    // *   while(flag) {
    // *        
    // *       for (int i = 0; i < arr_size; ++i) {
    // *      
    // *       LHS = decryption(hash generated based on s_i instead of rho_sums);
    // *   
    // *       // (z^C)modN^2:
    // *       paillier_plaintext_t* plain_C = paillier_plaintext_from_ui(C);
    // *       paillier_ciphertext_t* exponent = paillier_create_enc_zero();
    // *       paillier_exp(pu, exponent, enc_betas[i], plain_C);
    // * 
    // *       // A*(z^C)modN^2:
    // *       paillier_ciphertext_t* prod = paillier_create_enc_zero();
    // *       paillier_mul(pu, prod, A, exponent);
    // *       
    // *       RHS = decryption(prod);
    // * 
    // *       if (LHS != RHS) { 
    // *           flag = 0;
    // *           output error_message;
    // *           break;
    // *           }
    // *       }
    // *   }
    // *   output success_message;
    // * 
    // */
    // // ========================================================================================
    // // === ZKP TODO LATER ===
    // // ========================================================================================
    // /*
    // *
    // *   Verify how many C's can break ZKP and reveal rho_sums and test it
    // * 
    // */

   
    /* CLEANUP AND SOCKET CLOSURE*/

    close(listening_socket);
    close(hash_socket);

    paillier_freeciphertext(enc_hash);
    paillier_freeplaintext(hash);
    for (int i = 0; i < arr_size; ++i) {
        paillier_freeciphertext(enc_betas[i]);
    }
    enc_betas.clear();
    paillier_freepubkey(pu);
    paillier_freeprvkey(pr);
    
    return 0;

}

/* Compiling command for MacOS */
// g++ -o server ncphserver.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a
