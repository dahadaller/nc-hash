/* client.cpp is the program that will run on the client side in the public-key PHash scheme.
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

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include<vector>
#include <gmpxx.h>
#include<fstream>
#include"assert.h"
extern "C" {
    #include <gmp.h>
    #include <paillier.h>
}
#include "client.hpp"
#include <chrono>
#include <thread>

void mult_and_sum( paillier_pubkey_t* pu, paillier_ciphertext_t* sum, std::vector<paillier_ciphertext_t*> const &e_betas, std::vector<int> const &rho_sums) {
    /* PRE: Paillier AHE function accepts vector of ciphertexts(initialized with encryption of zero) for result, and betas, the corresponding vector of ints for rho sums, and the public key
        POST: Stores the encryption of sum of rho*beta products in the sum argument*/

    assert(e_betas.size() == rho_sums.size());              // assert all three input vectors are same size
    int limit = e_betas.size();
    for(int i = 0; i < limit; ++i) {                        // computing dot product = sum((ciphertext[i] * plaintext[i])):
        paillier_plaintext_t* plain_rho_sum = paillier_plaintext_from_ui((int)rho_sums[i]);
        paillier_ciphertext_t* enc_mult_res = paillier_create_enc_zero();
        paillier_exp( pu, enc_mult_res, e_betas[i], plain_rho_sum );
        paillier_mul(pu, sum, sum, enc_mult_res );

        /* CLEANUP */
        paillier_freeplaintext(plain_rho_sum);
        paillier_freeciphertext(enc_mult_res);
    }
}


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
    std::fstream pubKeyFile("pubkey.txt", std::fstream::in);
    std::string hexPubKey; 
    std::getline(pubKeyFile, hexPubKey); 
    paillier_pubkey_t* pu = paillier_pubkey_from_hex(&hexPubKey[0]);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));


    /* IMPORT FROM BYTESTRINGS */
    std::vector<paillier_ciphertext_t*> read_betas;          // prepare vector for read betas
    std::fstream ctxtFile2("ciphertext.txt", std::fstream::in|std::fstream::binary); // open the file in read mode
    for (int i = 0; i < arr_size; ++i) {
    
        // The length of the ciphertext is twice the length of the key
        char* byteCtxt2 = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        // Read a bytestring for each beta
        ctxtFile2.read(byteCtxt2, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        paillier_ciphertext_t* enc_beta = paillier_ciphertext_from_bytes((void*)byteCtxt2, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        // Push the encrypted beta to the vector
        read_betas.push_back(enc_beta);

    }
    ctxtFile2.close();

    // ========================================================================================
    // CLIENT'S ENCRYPTED MULTIPLICATION AND SUMMATION (= ENCRYPTION OF HASH)
    // ========================================================================================

    paillier_ciphertext_t* enc_sum_res = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
    mult_and_sum(pu, enc_sum_res, read_betas, rho_sums);

    /* Prepare/clean file for export */
    std::ofstream ctxtFile3;
    ctxtFile3.open("ciphertext.txt", std::ofstream::out | std::ofstream::trunc);
    ctxtFile3.close();

    /* EXPORT TO BYTESTRING */
    // Open the file in "append" mode
    std::fstream ctxtFile4("ciphertext.txt", std::fstream::out|std::fstream::app|std::fstream::binary);
    // The length of the ciphertext is twice the length of the key
    char* byteCtxt4 = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
    ctxtFile4.write(byteCtxt4, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile4.close();

     /* CLEANUP */

    paillier_freeciphertext(enc_sum_res);
    for (int i = 0; i < arr_size; ++i) {
        paillier_freeciphertext(read_betas[i]);
    }
    read_betas.clear();
    paillier_freepubkey(pu);

    return 0;

}