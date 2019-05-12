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
    write_paillier_key_file(hex_pk);
    printf("paillier.key initialized\n");

    
    // ========================================================================================
    // WRITE BETA ARRAY ENCRYPTION TO FILE (DO ONCE)
    // ========================================================================================

    std::vector<float> betas (arr_size, 0); // initialize a 0-vector and populate it with (rho_num) of betas
    get_rho_beta_arr(rho_num, betas, min_rho, max_rho, beta_mean, beta_std);
    std::vector<paillier_ciphertext_t*> enc_betas = encrypt_betas(betas, pu);
    write_enc_betas_to_key_file(enc_betas, pu);
    printf("betas.key initialized\n");


    // ========================================================================================
    // RECEIVE ENCRYPTED HASH AND RANDOM HASH (A) FROM CLIENT
    // ========================================================================================
        
    int listening_socket =  server_init();
    int tcp_socket = server_connect_to_client(listening_socket);

    char* enc_hash_string = receive_char_string(tcp_socket);
    printf("\nencrypted hash recieved: ");///
    print_bytes(enc_hash_string,256);///

    paillier_ciphertext_t* enc_hash = paillier_ciphertext_from_bytes((void*)enc_hash_string, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    paillier_plaintext_t* hash = paillier_dec(NULL, pu, pr, enc_hash);
    gmp_printf("\ndecrypted hash: %Zd\n", hash);///

    // RANDOM HASH (A) FROM CLIENT
    
    // The length of the ciphertext is twice the length of the key
    char* enc_A_string = receive_char_string(tcp_socket);
    printf("\n random hash recieved: ");///
    print_bytes(enc_A_string,256);///

    paillier_ciphertext_t* enc_A = paillier_ciphertext_from_bytes((void*)enc_A_string, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    paillier_plaintext_t* A = paillier_dec(NULL, pu, pr, enc_A);
    gmp_printf("\ndecrypted hash: %Zd\n", A);///


    /* CLEANUP */
    // free(enc_A_string);

    // // ========================================================================================
    // // === SERVER: ZKP OUTLINE ===
    // // ========================================================================================
    // /*
    // *   Create challenge request C:
    // */
    // gmp_randstate_t state;
    // gmp_randinit_mt(state);
    // mpz_class C;
    // mpz_urandomm(C.get_mpz_t(), state, pu->n);

    // // ========================================================================================
    // // === SERVER: ZKP ADDITIONAL IPC ===
    // // ========================================================================================
    // /*
    // *   Send C to client
    // */
    // FILE * zkp2_w = fopen("zkp2.txt", "w+");
    // if (mpz_out_raw (zkp2_w, C.get_mpz_t()) == 0) {
    //     std::cout << "Error occurs from zkp2( mpz_out_raw)\n";
    // }

    // mpz_clear(C.get_mpz_t()); 
    
    // fclose(zkp2_w);

    // // ========================================================================================
    // /*
    // *   Receive vector S in response to C
    // */
    // // ========================================================================================
    // // === ZKP OUTLINE ===
    // // ========================================================================================
    // /*
    // *   Do the check:
    // */
    // // hash generated based on s_i instead of rho_sums:
    // paillier_ciphertext_t* hash_S = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
    // mult_and_sum(pu, hash_S, enc_betas, S);
    // paillier_plaintext_t* LHS;
    // LHS = paillier_dec(NULL, pu, pr, hash_S);
    
    // paillier_ciphertext_t* z_C;
    // paillier_plaintext_t* plain_C = paillier_plaintext_from_ui(0);
    // mpz_set(plain_C->m, C.get_mpz_t());
    // z_C = paillier_create_enc_zero();
    // paillier_exp(pu, z_C, enc_hash, plain_C);

    // paillier_freeplaintext(plain_C);

    // // A*(z^C)modN^2:
    // paillier_ciphertext_t* prod = paillier_create_enc_zero();
    // paillier_mul(pu, prod, enc_A, z_C);
            
    // paillier_plaintext_t* RHS;
    // RHS = paillier_dec(NULL, pu, pr, prod);


    // if(!mpz_cmp (LHS->m, RHS->m)) {
    //     std::cout << "ZKP check passed!" << std::endl;
    // }
    // else {
    //     std::cout << "Did not pass ZKP check!" << std::endl;
    // }
    
    // // ========================================================================================


    // /* HASH DIFFERENCE */
    // if (i == 1) {                                       // save the original hash value
    //     mpf_set_z(orig_hash, dec_res->m);
    // }
    // else {                                              // compute difference with the original hash
    //     const int precision = 512;
    //     mpf_set_default_prec (precision);               // set defference precision
    //     mpf_t curr_hash;
    //     mpf_init(curr_hash);
    //     mpf_set_z(curr_hash, dec_res->m);
    //     mpf_t hash_diff;
    //     mpf_init(hash_diff);                            // initialize difference to 0
    //     // diff = |(old-new)/old|
    //     mpf_sub(hash_diff, orig_hash, curr_hash);
    //     mpf_div(hash_diff, hash_diff, orig_hash);
    //     mpf_abs(hash_diff, hash_diff);

    //     gmp_printf("\nNormalized difference with the original: %.5Ff", hash_diff);

    //     mpf_t percent_diff;
    //     mpf_init(percent_diff);
    //     mpf_mul_ui(percent_diff, hash_diff, 100);
    //     gmp_printf(" (%.1Ff %%)\n", percent_diff);
    // }


   
    /* CLEANUP AND SOCKET CLOSURE*/

    close(listening_socket);
    close(tcp_socket);

    paillier_freeciphertext(enc_hash);
    paillier_freeplaintext(hash);
    paillier_freeciphertext(enc_A);
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
