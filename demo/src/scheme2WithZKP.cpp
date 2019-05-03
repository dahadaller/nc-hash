/* scheme2WithZKP.hpp is the combined server-client program implementing publicallly evaluable PHash with Zero-Knowledge Proof.
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
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <random>
#include <time.h>
#include <vector>
using std::vector;
#include <gmpxx.h>
#include <gmp.h>
#include <fstream>
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
    /* PRE: Accepts number or radii to compute corresponding betas for
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


void mult_and_sum( paillier_pubkey_t* pu, paillier_ciphertext_t* sum, std::vector<paillier_ciphertext_t*> const &e_betas, std::vector<int> const &exponent) {
    /* PRE: Paillier AHE function accepts vector of ciphertexts(initialized with encryption of zero) for result, and betas, the corresponding vector of ints for exponent, and the public key
        POST: Stores the encryption of sum of exponent*beta products in the sum argument*/

    assert(e_betas.size() == exponent.size());              // assert all three input vectors are same size
    int limit = e_betas.size();
    for(int i = 0; i < limit; ++i) {                        // computing dot product = sum((ciphertext[i] * plaintext[i])):
        paillier_plaintext_t* plain_exponent = paillier_plaintext_from_ui((int)exponent[i]);
        paillier_ciphertext_t* enc_mult_res = paillier_create_enc_zero();
        paillier_exp( pu, enc_mult_res, e_betas[i], plain_exponent );
        paillier_mul(pu, sum, sum, enc_mult_res );

        /* CLEANUP */
        paillier_freeplaintext(plain_exponent);
        paillier_freeciphertext(enc_mult_res);
    }
}

void mult_and_sum(paillier_pubkey_t* pu, paillier_ciphertext_t* sum, std::vector<paillier_ciphertext_t*> const &e_betas, std::vector<mpz_class> const &exponent) {
    /* PRE: Paillier AHE function accepts vector of ciphertexts(initialized with encryption of zero) for result, and betas, the corresponding vector of mpz_class for the exponent, and the public key
        POST: Stores the encryption of sum of exponent*beta products in the sum argument*/

    assert(e_betas.size() == exponent.size());              // assert all three input vectors are same size
    int limit = e_betas.size();
    for(int i = 0; i < limit; ++i) {                        // computing dot product = sum((ciphertext[i] * plaintext[i])):

        paillier_plaintext_t* plain_exponent = paillier_plaintext_from_ui(0);
        mpz_set (plain_exponent->m,exponent[i].get_mpz_t());
        paillier_ciphertext_t* enc_mult_res = paillier_create_enc_zero();
        paillier_exp( pu, enc_mult_res, e_betas[i], plain_exponent );
        paillier_mul(pu, sum, sum, enc_mult_res );


        /* CLEANUP */
        paillier_freeplaintext(plain_exponent);
        paillier_freeciphertext(enc_mult_res);
    }
}

long long int check_sum(std::vector<float> const &betas, std::vector<int> const &rho_sums) {
    /* PRE: Accepts vector of floats for beta and vector of integers for sums of pixels for all rhos, must be same size
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
    // ============
    //  Preprocess
    // ============
    int child = fork();
    pid_t status;
    while (child == 0) {
        int myargc = argc + 1;
        char* myargv[100];
        myargv[0] = "python";
        myargv[1] = "preprocess.py";
        int i;
        for (i = 2; i < myargc; ++i) {
            myargv[i] = argv[i-1];
        }
        myargv[i] = NULL;
        execvp("python", myargv);
        printf("EXECVP FAILED\n");
    }
    waitpid(child, &status, 0);


    float beta_mean = 50.0, beta_std = 10.0;
    int arr_size = 256;
    int max_rho = 255, min_rho = 60;
    int rho_num = 3;
    int paillier_n = 1024;

    paillier_pubkey_t* pu;                                  // the public key
    paillier_prvkey_t* pr;                                  // the private key 
	struct timespec ts0,ts1;
	clock_gettime(CLOCK_REALTIME,&ts0);
    paillier_keygen(paillier_n, &pu, &pr, paillier_get_rand_devurandom);
	clock_gettime(CLOCK_REALTIME,&ts1);
	// printf("time for keygen: %li ns\n",(ts1.tv_sec - ts0.tv_sec)*1000000000 + ts1.tv_nsec - ts0.tv_nsec);
	printf("time for keygen: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

	clock_gettime(CLOCK_REALTIME,&ts0); /* begin clock for servers hash keygen work */


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
	clock_gettime(CLOCK_REALTIME,&ts1);
	printf("time for server's hash keygen: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

    // ========================================================================================
    // CLIENT'S RHO SUMS COMPUTATION:
    // ========================================================================================

	clock_gettime(CLOCK_REALTIME,&ts0); /* start clock for client reading encrypted betas */
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
	clock_gettime(CLOCK_REALTIME,&ts1); /* stop clock for client reading encrypted betas */
	printf("time to read encrypted betas: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

	for (size_t i = 1; i < argc; i++) {
		// ========================================================================================
		// CLIENT'S ENCRYPTED MULTIPLICATION AND SUMMATION (= ENCRYPTION OF HASH)
		// ========================================================================================
		clock_gettime(CLOCK_REALTIME,&ts0); /* start clock for client inner product */
		CImg<float> img(argv[i]);
		CImg<float> polar = polar_FFT(img);
		vector<int> rho_sums = sum_along_rho(polar);
		paillier_ciphertext_t* enc_sum_res = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
		mult_and_sum(pu, enc_sum_res, read_betas, rho_sums);
		/* EXPORT TO BYTESTRING */
		std::ofstream ctxtFile3;
		ctxtFile3.open("ciphertext.txt", std::ofstream::out | std::ofstream::trunc);
		ctxtFile3.close();
		std::fstream ctxtFile4("ciphertext.txt", std::fstream::out|std::fstream::app|std::fstream::binary);
        
		char* byteCtxt4 = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
		ctxtFile4.write(byteCtxt4, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
		ctxtFile4.close();
		clock_gettime(CLOCK_REALTIME,&ts1); /* stop clock for client inner product */
		printf("time for client work on image %i: %li ms\n",i,
				(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

		// ========================================================================================
		// SERVER'S HASH DECRYPTION
		// ========================================================================================
		clock_gettime(CLOCK_REALTIME,&ts0); /* start clock for server decryption */
		std::fstream ctxtFile5("ciphertext.txt", std::fstream::in|std::fstream::binary); // open the file
		// The length of the ciphertext is twice the length of the key
		char* byteCtxt5 = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
		ctxtFile5.read(byteCtxt5, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
		paillier_ciphertext_t* read_res = paillier_ciphertext_from_bytes((void*)byteCtxt5, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
		ctxtFile5.close();
		printf("Image: %s\n", argv[i]);
		paillier_plaintext_t* dec_res;
		dec_res = paillier_dec(NULL, pu, pr, read_res);
		clock_gettime(CLOCK_REALTIME,&ts1); /* stop clock for server decryption */
		printf("time for server decryption of image %i: %li ms\n",i,
				(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);
		gmp_printf("Decrypted hash: %Zd\n", dec_res);

        clock_gettime(CLOCK_REALTIME,&ts0); /* start clock for server decryption in the clear*/
		/* CHECK W/O ENCRYPTION */
		long long int check_hash = check_sum(betas, rho_sums);
		std::cout << "W/o encryption: " << check_hash << std::endl;
        clock_gettime(CLOCK_REALTIME,&ts1); /* stop clock for server decryption in the clear*/
        printf("time for server decryption in the clear of image %i: %li ns\n",i,
				(ts1.tv_sec - ts0.tv_sec)*1000000000 + (ts1.tv_nsec - ts0.tv_nsec));
		// enc_betas.clear();

        // ========================================================================================
        // === CLIENT: ZKP OUTLINE ===
        // ========================================================================================

        // Compute A ~ Encryption of a hash based on random values instead of rho_sum
        clock_gettime(CLOCK_REALTIME,&ts0);
        vector<mpz_class> rands;
        for (int i = 0; i < arr_size; ++i) {
            gmp_randstate_t state;
            gmp_randinit_mt (state);
            mpz_class r;
            mpz_urandomm(r.get_mpz_t(), state, pu->n);
            rands.push_back(r);
        }

        paillier_ciphertext_t* A = paillier_create_enc_zero();
        mult_and_sum(pu, A, read_betas, rands);
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for computing A: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // ========================================================================================
        // === CLIENT: ZKP ADDITIONAL IPC CLIENT ===
        // ========================================================================================
        /*
        *   Send A to Server
        */
        /* EXPORT TO BYTESTRING */
        // Open the file in "append" mode
        // clock_gettime(CLOCK_REALTIME,&ts0);
        std::fstream zkp1_w("zkp1.txt", std::fstream::out|std::fstream::trunc|std::fstream::binary);
        // The length of the ciphertext is twice the length of the key
        char* char_A_w = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, A);
        zkp1_w.write(char_A_w, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        zkp1_w.close();
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for sending A to Server: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);
        // ========================================================================================

        // ========================================================================================
        // === CLIENT: SEND ENCRYPTED HASH TO SERVER ===
        // ========================================================================================
        /* EXPORT TO BYTESTRING */
        // Open the file in "append" mode
        // clock_gettime(CLOCK_REALTIME,&ts0);
        std::fstream ipc3_w("ipc3.txt", std::fstream::out|std::fstream::trunc|std::fstream::binary);
        // The length of the ciphertext is twice the length of the key
        char* char_result_w = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_sum_res);
        ipc3_w.write(char_result_w, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        ipc3_w.close();
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for sending Encrypted Hash to Server: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // ========================================================================================
        // SERVER: READ AND DECRYPT HASH
        // ========================================================================================

        /* IMPORT FROM BYTESTRINGS */
        // clock_gettime(CLOCK_REALTIME,&ts0);
        std::fstream ipc3("ipc3.txt", std::fstream::in|std::fstream::binary); // open the file
        // The length of the ciphertext is twice the length of the key
        char* char_result = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        ipc3.read(char_result, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        paillier_ciphertext_t* read_res_zkp = paillier_ciphertext_from_bytes((void*)char_result, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        ipc3.close();

        /* CLEANUP */
        free(char_result);

        paillier_plaintext_t* dec_res_zkp;
        dec_res_zkp = paillier_dec(NULL, pu, pr, read_res_zkp);
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for read anddecrypt hash: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);
        gmp_printf("Decrypted hash: %Zd\n", dec_res_zkp);


        // ========================================================================================
        // === SERVER: ZKP ADDITIONAL IPC ===
        // ========================================================================================
        /*
        *   Receive and import the A from bytestring along with the real hash.
        */

        /* IMPORT FROM BYTESTRINGS */
        // clock_gettime(CLOCK_REALTIME,&ts0);
        std::fstream zkp1("zkp1.txt", std::fstream::in|std::fstream::binary); // open the file
        // The length of the ciphertext is twice the length of the key
        char* char_A = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        zkp1.read(char_A, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        paillier_ciphertext_t* read_A = paillier_ciphertext_from_bytes((void*)char_A, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        zkp1.close();
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for Receive and import the A from bytestring along with the real hash: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        /* CLEANUP */
        free(char_A);

        // ========================================================================================
        // === SERVER: ZKP OUTLINE ===
        // ========================================================================================
        /*
        *   Create challenge request C:
        */
        // int C = get_rand_index(pu->n);
        // clock_gettime(CLOCK_REALTIME,&ts0);
        gmp_randstate_t state;
        gmp_randinit_mt(state);
        mpz_class C;
        mpz_urandomm(C.get_mpz_t(), state, pu->n);
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for creating challenge request C: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // ========================================================================================
        // === SERVER: ZKP ADDITIONAL IPC ===
        // ========================================================================================
        /*
        *   Send C to client
        */
        // clock_gettime(CLOCK_REALTIME,&ts0);
        FILE * zkp2_w = fopen("zkp2.txt", "w+");
        if (mpz_out_raw (zkp2_w, C.get_mpz_t()) == 0) {
            std::cout << "Error occurs from zkp2( mpz_out_raw)\n";
        }
        fclose(zkp2_w);
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for sending C to client: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // ========================================================================================
        // === CLIENT: ZKP ADDITIONAL IPC ===
        // ========================================================================================
        /*
        *   Receive challenge integer C from server
        */

        // clock_gettime(CLOCK_REALTIME,&ts0);
        mpz_class read_C;
        mpz_init(read_C.get_mpz_t());
        size_t number_bytes_read;
        FILE* zkp2 = fopen("zkp2.txt","r");
        number_bytes_read = mpz_inp_raw ( read_C.get_mpz_t(), zkp2 );
        fclose(zkp2);
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for receiving C from server: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // ========================================================================================
        // === ZKP OUTLINE ===
        // ========================================================================================
        /*
        *   Generate vector S:
        */
        // clock_gettime(CLOCK_REALTIME,&ts0);
        std::vector<mpz_class> S;
        for (int i = 0; i < arr_size; ++i) {
    
            mpz_class rho_mpz;
            mpz_set_ui (rho_mpz.get_mpz_t(),  rho_sums[i]);

            mpz_class C_rho;
            mpz_init(C_rho.get_mpz_t());
            mpz_mul(C_rho.get_mpz_t(), read_C.get_mpz_t(), rho_mpz.get_mpz_t());

            mpz_class sum_mpz;
            mpz_init(sum_mpz.get_mpz_t());
            mpz_add(sum_mpz.get_mpz_t(), rands[i].get_mpz_t(), C_rho.get_mpz_t());

            mpz_class s_i;
            mpz_init(s_i.get_mpz_t());
            mpz_mod(s_i.get_mpz_t(), sum_mpz.get_mpz_t(), (pu->n));
            
            S.push_back(s_i);
        }
        // clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for generating vector S: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);

        // // ========================================================================================
        // // === ZKP ADDITIONAL IPC ===
        // // ========================================================================================
        
        // /*
        // *   Send S to server
        // */
        // FILE * zkp3_clean = fopen("zkp3.txt", "w");
        // fclose(zkp3_clean);
        // FILE * zkp3_w = fopen("zkp3.txt", "a");
        // for (int i = 0; i < arr_size; ++i){
        //     if (mpz_out_str(zkp3_w, 10, S[i].get_mpz_t()) == 0) {
        //         std::cout << "Error occurs from zkp3( mpz_out_raw)\n";
        //         fputc('\0', zkp3_w);
        //     }
        //     // char delim[]= {'\n'};
        //     // fwrite(delim, sizeof(char), sizeof(delim), zkp3_w);
        // }
        // fclose(zkp3_w);

        // // ========================================================================================
        // // SERVER: ADDITIONAL IPC
        // // ========================================================================================
        
        // /*
        // *   Receive vector S in response to C
        // */
        // // std::vector<int> read_S;
        // // std::string line;
        // // std::ifstream zkp3("zkp3.txt");
        // // if (zkp3.is_open()) {
        // //      while ( getline(zkp3,line) ) {
        // //         read_S.push_back(std::stoi(line));
        // //     }
        // //     zkp3.close();
        // // }
        // std::vector<mpz_class> read_S;
        // FILE * zkp3 = fopen("zkp3.txt", "r");
        // for (int i = 0; i < arr_size; ++i) {
        //     mpz_class read_S_i;
        //     mpz_init(read_S_i.get_mpz_t());
        //     if (mpz_inp_raw(read_S_i.get_mpz_t(), zkp3) == 0) {
        //         std::cout << i << "\tError occurs from zkp3( mpz_inp_raw)\n";
        //     }
        //     read_S.push_back(read_S_i);
        // }
        // fclose(zkp3);

        // ========================================================================================
        // === SERVER: ZKP OUTLINE ===
        // ========================================================================================
        /*
        *   Do the check:
        */
        // hash generated based on s_i instead of rho_sums:
        // clock_gettime(CLOCK_REALTIME,&ts0);
        paillier_ciphertext_t* hash_S = paillier_create_enc_zero(); // initiate a zero-valued ciphertext to hold the result 
        // mult_and_sum(pu, hash_S, enc_betas, read_S);
        mult_and_sum(pu, hash_S, enc_betas, S);
        paillier_plaintext_t* LHS;
        LHS = paillier_dec(NULL, pu, pr, hash_S);
        
        paillier_ciphertext_t* z_C;
        paillier_plaintext_t* plain_C = paillier_plaintext_from_ui(0);
        mpz_set(plain_C->m, C.get_mpz_t());
        z_C = paillier_create_enc_zero();
        paillier_exp(pu, z_C, read_res_zkp, plain_C);

        paillier_freeplaintext(plain_C);

        // A*(z^C)modN^2:
        paillier_ciphertext_t* prod = paillier_create_enc_zero();
        paillier_mul(pu, prod, read_A, z_C);
                
        paillier_plaintext_t* RHS;
        RHS = paillier_dec(NULL, pu, pr, prod);



        // Need to implement comparison = convert plaintext to strings
        // gmp_printf("LHS: %Zd\n", LHS);
        // gmp_printf("RHS: %Zd\n", RHS);
        if(!mpz_cmp (LHS->m, RHS->m)) {
            std::cout << "ZKP check passed!" << std::endl;
        }
        else {
            std::cout << "Did not pass ZKP check!" << std::endl;
        }
        clock_gettime(CLOCK_REALTIME,&ts1);
        // printf("time for checking: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);
        printf("time for ZKP: %li ms\n",(ts1.tv_sec - ts0.tv_sec)*1000 + (ts1.tv_nsec - ts0.tv_nsec)/1000000);
        
        // ========================================================================================
        // === ZKP TODO LATER ===
        // ========================================================================================
        /*
        *
        *   Verify how many C's can break ZKP and reveal rho_sums and test it
        * 
        */

       /* CLEANUP */
        paillier_freeciphertext(enc_sum_res);
        paillier_freeciphertext(read_res);
        paillier_freeplaintext(dec_res);
        paillier_freeciphertext(read_res_zkp);
        paillier_freeplaintext(dec_res_zkp);
        paillier_freeciphertext(A);
        paillier_freeciphertext(read_A);

	}
    for (int i = 0; i < arr_size; ++i) {
        paillier_freeciphertext(enc_betas[i]);
        paillier_freeciphertext(read_betas[i]);
    }
    enc_betas.clear();
    read_betas.clear();
    paillier_freepubkey(pu);
    paillier_freeprvkey(pr);
	return 0;
}

/* Compiling commands for MacOS */
// g++ -o scheme2 scheme2WithZKP.cpp /usr/local/opt/gmp/lib/libgmp.a /usr/local/lib/libpaillier.a -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
