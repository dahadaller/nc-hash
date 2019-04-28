//
//  scheme2.cpp
//  
//
//  Created by Anastasiia Timashova on 3/25/19.
//

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <random>
#include<vector>
#include <gmpxx.h>
#include<fstream>
#include"assert.h"
extern "C" {
    #include <gmp.h>
    #include <paillier.h>
}
#include "client.hpp"

int get_rand_index(int const &size) {
    /* PRE: accepts the size of the array for which random index is needed, must be positive int
        POST: returns a random index that follows uniform distribution */

    std::random_device rd;                                  // obtain a different random number from hardware
    std::mt19937 eng(rd());                                 // seed the generator
    std::uniform_int_distribution<> distr(0, size-1);       // instance of class std::uniform_int_distribution with specific min and max
    return distr(eng);                                      // obtain a random index for the sample array
}

float get_beta(float mean, float std) {
    /* PRE: Accepts the mean and std for normal distribution
        POST: Returns a single normally distributed variable beta */

    std::random_device rd;                                  // obtain a random number from hardware
    std::mt19937 gen(rd());                                 // Mersenne twister PRNG, initialized with seed from previous random device instance
    
    float sample[1000];
    for (int i = 0; i < 1000; ++i) {
        std::normal_distribution<float> d(mean, std);       // instance of class std::normal_distribution with specific mean and stddev
        sample[i] = d(gen);                                 // get random number with normal distribution using gen as random source
    }
    
    int index = get_rand_index(1000);
    return sample[index];
}

int get_rho(int min_rho, int max_rho) {
    /* PRE: Accepts min and max values for uniform distribution
        POST: Returns a uniformly distributed variable min <= rho <= max */

    std::random_device rd;                                  // obtain a random number from hardware
    std::mt19937 gen(rd());                                 // Mersenne twister PRNG, initialized with seed from previous random device instance
    
    float sample[1000];
    for (int i = 0; i < 1000; ++i) {
        std::uniform_int_distribution<> d(min_rho, max_rho);// instance of class std::normal_distribution with specific mean and stddev
        sample[i] = d(gen);                                 // get random number with normal distribution using gen as random source
    }
    
    int index = get_rand_index(1000);
    return sample[index];
}

void get_rho_beta_arr (int num_rhos, std::vector<float> &betas, int min_rho, int max_rho, float beta_mean, float beta_std) {
    /* PRE: Accepts number or radii to copute corresponding betas for
        POST: Returns 256 integers long array of corresponding betas with beta(i) if there exists rho==i and 0 o.w. */

    std::vector<int> rhos;                                  // initialize vector to store rhos
    int rho = get_rho(min_rho, max_rho);                    // generate first rho - push it to rhos vector and get beta for index=rho in betas vector
    rhos.push_back(rho);
    betas[rho] = get_beta(beta_mean, beta_std);
    
    for (int i = 1; i < num_rhos; ++i) {                    // repeat the above with check for uniqueness of rhos:
        while(1) { 
            rho = get_rho(min_rho, max_rho);                // get random rho
            if (std::find(rhos.begin(), rhos.end(), rho) != rhos.end()) { continue; } // if rho is not unique, try again
            rhos.push_back(rho);                            // otherwise, push to rho vector and get beta for index=rho in betas vector
            betas[rho] = get_beta(beta_mean, beta_std);
            break;
        }
    }
}

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

long long int check_sum(std::vector<float> const &betas, std::vector<int> const &rho_sums) {
    /* PRE: Accepts vector of flots for beta and vector of integers for sums of pixels for all rhos, must be same size
        POST: Returns a hash value based on scheme 2 with all computations in the clear */
    
    assert(betas.size() == rho_sums.size());
    int limit = betas.size();
    long long int sum = 0;
    for (int i = 0; i < limit; ++i) {
        sum += ((int)abs(betas[i])) * rho_sums[i];
    }
    return sum;
}

int main(int argc, char* argv[]) {
    
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
    // SERVER'S BETA ARRAY ENCRYPTION
    // ========================================================================================

    std::vector<float> betas (arr_size, 0);                 // initialize a 0-vector and populate it with (rho_num) of betas
    get_rho_beta_arr(rho_num, betas, min_rho, max_rho, beta_mean, beta_std);

    std::vector<paillier_ciphertext_t*> enc_betas;          // declare a vector for encryptions of betas 

    /* Prepare/clean file for export */
    std::ofstream ctxtFile0;
    ctxtFile0.open("ciphertext.txt", std::ofstream::out | std::ofstream::trunc);
    ctxtFile0.close();


    /* EXPORT TO BYTESTRING */
    // Open the file in "append" mode
    std::fstream ctxtFile1("ciphertext.txt", std::fstream::out|std::fstream::app|std::fstream::binary);

    for (int i = 0; i < arr_size; ++i) {
        paillier_plaintext_t* plain_beta = paillier_plaintext_from_ui((int)abs(betas[i])); // currently, we only consider positive beta values
        paillier_ciphertext_t* enc_beta = paillier_enc(NULL, pu, plain_beta, paillier_get_rand_devurandom);
        enc_betas.push_back(enc_beta);
        
        /* CLEANUP */
        paillier_freeplaintext(plain_beta);

        
        // The length of the ciphertext is twice the length of the key
        char* byteCtxt1 = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_beta);
        // append the bytestring for each beta
        ctxtFile1.write(byteCtxt1, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    }
    ctxtFile1.close();

    // ========================================================================================
    // CLIENT'S RHO SUMS COMPUTATION:
    // ========================================================================================

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

    /* DEBUGGING: */
    // std::cout << "\n" << i << "\tSuccess!\t";
    // paillier_plaintext_t* dec_beta;
    // dec_beta = paillier_dec(NULL, pu, pr, enc_beta);
    // gmp_printf("Decrypted read beta: %Zd\n", dec_beta);
    }
    ctxtFile2.close();

    // CImg<float> src("/Users/timyan/Documents/Research/ahe/GrayLenna.bmp");
    // CImg<float> img = get_grayscale(src);
    // CImg<float> low_pass_filter(9, 9, 1, 1, 1.0/9.0);
    // img = apply_filter(img, low_pass_filter);

    CImg<float> img(argv[1]);
    CImg<float> img2(argv[2]);

    CImg<float> polar = polar_FFT(img);
    CImg<float> polar2 = polar_FFT(img2);

    float *message1 = sum_along_rho1(polar);
    std::vector<int> rho_sums = sum_along_rho(polar);
    std::vector<int> rho_sums2 = sum_along_rho(polar2);


    // ========================================================================================
    // CLIENT'S ENCRYPTED MULTIPLICATION AND SUMMATION (= ENCRYPTION OF HASH)
    // ========================================================================================

    paillier_ciphertext_t* enc_sum_res = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
    paillier_ciphertext_t* enc_sum_res2 = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
    mult_and_sum(pu, enc_sum_res, read_betas, rho_sums);
    mult_and_sum(pu, enc_sum_res2, read_betas, rho_sums2);


    /* Prepare/clean file for export */
    std::ofstream ctxtFile3;
    ctxtFile3.open("ciphertext.txt", std::ofstream::out | std::ofstream::trunc);
    ctxtFile3.close();

    std::ofstream ctxtFile3_2;
    ctxtFile3_2.open("ciphertext2.txt", std::ofstream::out | std::ofstream::trunc);
    ctxtFile3_2.close();

    /* EXPORT TO BYTESTRING */
    // Open the file in "append" mode
    std::fstream ctxtFile4("ciphertext.txt", std::fstream::out|std::fstream::app|std::fstream::binary);
    // The length of the ciphertext is twice the length of the key
    char* byteCtxt4 = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
    ctxtFile4.write(byteCtxt4, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile4.close();

    std::fstream ctxtFile4_2("ciphertext2.txt", std::fstream::out|std::fstream::app|std::fstream::binary);
    // The length of the ciphertext is twice the length of the key
    char* byteCtxt4_2 = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res2);
    ctxtFile4_2.write(byteCtxt4_2, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile4_2.close();



    // ========================================================================================
    // SERVER'S HASH DECRYPTION
    // ========================================================================================

    /* IMPORT FROM BYTESTRINGS */
    std::fstream ctxtFile5("ciphertext.txt", std::fstream::in|std::fstream::binary); // open the file
    // The length of the ciphertext is twice the length of the key
    char* byteCtxt5 = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile5.read(byteCtxt5, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    paillier_ciphertext_t* read_res = paillier_ciphertext_from_bytes((void*)byteCtxt5, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile5.close();

    std::fstream ctxtFile5_2("ciphertext2.txt", std::fstream::in|std::fstream::binary); // open the file
    // The length of the ciphertext is twice the length of the key
    char* byteCtxt5_2 = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile5_2.read(byteCtxt5_2, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    paillier_ciphertext_t* read_res_2 = paillier_ciphertext_from_bytes((void*)byteCtxt5_2, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    ctxtFile5_2.close();

    paillier_plaintext_t* dec_res;
    dec_res = paillier_dec(NULL, pu, pr, read_res);
    gmp_printf("Decrypted hash: %Zd\n", dec_res);

    /* CHECK W/O ENCRYPTION */
    long long int check_hash = check_sum(betas, rho_sums);
    std::cout << "W/o encryption: " << check_hash << std::endl;

    paillier_plaintext_t* dec_res2;
    dec_res2 = paillier_dec(NULL, pu, pr, read_res_2);
    gmp_printf("Decrypted hash: %Zd\n", dec_res2);

    long long int check_hash2 = check_sum(betas, rho_sums2);
    std::cout << "W/o encryption: " << check_hash2 << std::endl;

    // ========================================================================================
    
    /* CLEANUP */

    paillier_freeciphertext(enc_sum_res);
    paillier_freeciphertext(enc_sum_res2);
    for (int i = 0; i < arr_size; ++i) {
        paillier_freeciphertext(enc_betas[i]);
        paillier_freeciphertext(read_betas[i]);
    }
    enc_betas.clear();
    paillier_freepubkey(pu);
    paillier_freeprvkey(pr);
    
    return 0;
}

/* Compiling commands for MacOS */
// g++ -o scheme2 scheme2.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a

// With #include "client.hpp"
// g++ -o scheme2 scheme2.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
