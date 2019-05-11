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

#include "ncph.hpp"

int main() {

    // ========================================================================================
    // GENERATING RHO_SUMS
    // ========================================================================================

    CImg<float> src("../img/GrayLenna.bmp");
    CImg<float> img = get_grayscale(src);
    CImg<float> low_pass_filter(9, 9, 1, 1, 1.0/9.0);
    img = apply_filter(img, low_pass_filter);

    CImg<float> polar = polar_FFT(img);

    float *message1 = sum_along_rho1(polar);
    std::vector<int> rho_sums = sum_along_rho(polar);


    /* PARAMETERS & CONSTRAINTS: 
        * arr_size = how many possible rhos there can be based on the input image I size, integer; arr_size = len(I)/2> 0
    */
    int arr_size = 256;

    // ========================================================================================
    // READ THE PUBLIC KEY AND ENCRYPTED BETAS
    // ========================================================================================
    // Read public key from disk and initialize it

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    paillier_pubkey_t* pu = read_paillier_key_file();

    std::vector<paillier_ciphertext_t*> read_betas = read_enc_betas_from_key_file(pu);

    // ========================================================================================
    // CLIENT'S ENCRYPTED MULTIPLICATION AND SUMMATION (= ENCRYPTION OF HASH)
    // ========================================================================================

    paillier_ciphertext_t* enc_sum_res = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
    mult_and_sum(pu, enc_sum_res, read_betas, rho_sums);

    // ========================================================================================
    // === ZKP OUTLINE ===
    // ========================================================================================
   
    // Compute A ~ Encryption of a hash based on random values instead of rho_sum
    std::vector<mpz_class> rands;
    for (int i = 0; i < arr_size; ++i) {
        gmp_randstate_t state;
        gmp_randinit_mt (state);
        mpz_class r;
        mpz_urandomm(r.get_mpz_t(), state, pu->n);
        rands.push_back(r);
        mpz_clear(r.get_mpz_t()); 
    }

    paillier_ciphertext_t* A = paillier_create_enc_zero();
    mult_and_sum(pu, A, read_betas, rands);
    
    // // ========================================================================================
    // // === ZKP IPC ===
    // // ========================================================================================
    // /*
    // *   Send A to Server
    // */
    // /* EXPORT TO BYTESTRING */
    // // Open the file in "append" mode
    // std::fstream zkp1_w("zkp1.txt", std::fstream::out|std::fstream::trunc|std::fstream::binary);
    // // The length of the ciphertext is twice the length of the key
    // char* char_A_w = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, A);
    // zkp1_w.write(char_A_w, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    // zkp1_w.close();
  
    // ========================================================================================
    // SEND ENCRYPTED HASH TO SERVER
    // ========================================================================================

    char* enc_hash_string = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
    int hash_socket =  client_connect_to_server();
    printf("paillier bytes: %d\n",PAILLIER_BITS_TO_BYTES(pu->bits)*2 );
    send_char_string(hash_socket, enc_hash_string, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    close(hash_socket);

    // // ========================================================================================
    // // === ZKP ADDITIONAL IPC ===
    // // ========================================================================================
    
    // *   Receive challenge integer C from server
    

    // mpz_class read_C;
    // mpz_init(read_C.get_mpz_t());
    // size_t number_bytes_read;
    // FILE* zkp2 = fopen("zkp2.txt","r");
    // number_bytes_read = mpz_inp_raw ( read_C.get_mpz_t(), zkp2 );
    // fclose(zkp2);

    // // ========================================================================================
    // // === ZKP OUTLINE ===
    // // ========================================================================================
    // /*
    // *   Generate vector S:
    // */
    // std::vector<mpz_class> S;
    // for (int i = 0; i < arr_size; ++i) {

    //     mpz_class rho_mpz;
    //     mpz_set_ui (rho_mpz.get_mpz_t(),  rho_sums[i]);

    //     mpz_class C_rho;
    //     mpz_init(C_rho.get_mpz_t());
    //     mpz_mul(C_rho.get_mpz_t(), read_C.get_mpz_t(), rho_mpz.get_mpz_t());

    //     mpz_clear(read_C.get_mpz_t()); 
    //     mpz_clear(rho_mpz.get_mpz_t());

    //     mpz_class sum_mpz;
    //     mpz_init(sum_mpz.get_mpz_t());
    //     mpz_add(sum_mpz.get_mpz_t(), rands[i].get_mpz_t(), C_rho.get_mpz_t());

    //     mpz_clear(C_rho.get_mpz_t());

    //     mpz_class s_i;
    //     mpz_init(s_i.get_mpz_t());
    //     mpz_mod(s_i.get_mpz_t(), sum_mpz.get_mpz_t(), (pu->n));
        
    //     S.push_back(s_i);
    // }
    // // ========================================================================================
    // // === ZKP ADDITIONAL IPC ===
    // // ========================================================================================
    // /*
    // *   Send vector S to server
    // */


     /* CLEANUP AND SOCKET CLOSE*/

    close(hash_socket);

    paillier_freeciphertext(enc_sum_res);
    paillier_freeciphertext(A);
    for (int i = 0; i < arr_size; ++i) {
        paillier_freeciphertext(read_betas[i]);
        // mpz_clear(S[i].get_mpz_t()); 
        mpz_clear(rands[i].get_mpz_t());        
    }
    read_betas.clear();
    paillier_freepubkey(pu);

    return 0;

}

// Compiling command for MacOS:
// g++ -o client ncphclient.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread