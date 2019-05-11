/* client.hpp is the supporting image processing library for program that will run on the client side 
    in the public-key PHash scheme.
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

    For any questions, you may contact NCP-Hash Group via opening an issue on 
    https://github.com/ncp-hash/public-phash/issues
*/

#include "CImg.h"
#include <cstdlib>
#include <math.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <random>
#include <gmpxx.h>
#include <fstream>
#include <sstream>
#include "assert.h"
extern "C" {
    #include <gmp.h>
    #include <paillier.h>
    #include "tcp.c"
}
#include <chrono>
#include <thread>
#include <string>
#include <cstring>

#define cimg_display 0
#define BETA_SIZE 256 //each beta is 256 bytes
#define NUM_BETAS 256 //there are 256 betas in total
#define PAILLIER_KEY_SIZE 256 //this is 256 bytes

using namespace cimg_library;

CImg<float> get_grayscale(CImg<float> src) {
    CImg<float> img;

    if (src.spectrum() == 3) {
        img = src.RGBtoYCbCr().channel(0);
    } else if (src.spectrum() == 4) {
        int width = img.width();
        int height = img.height();
        int depth = img.depth();
        img = src.crop(0, 0, 0, 0, width - 1, height - 1, depth - 1, 2).RGBtoYCbCr().channel(0);
    } else {
        img = src.channel(0);
    }

    return img;
}

CImg<float> apply_filter(CImg<float> img, CImg<float> kernel) {
    return img.get_convolve(kernel);
}

int cdf(CImg<float> I, int x) {
    int p = 0;
    int cols = I.width();
    int rows = I.height();

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (I(i, j) <= x) p += 1;
        }
    }

    return p;
}

CImg<int> get_cdf_matrix(CImg<float> img) {
    int cols = img.width();
    int rows = img.height();
    CImg<int> cdf_matrix(cols, rows, 1, 1, 0);
    // CImg<float> meanfilter(7,7,1,1,1);

    int count = 0;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            cdf_matrix(i, j) = cdf(img, img(i, j));
            std::cout << count++ << "\t" << "Working...\n";
            
        }
    }

    return cdf_matrix;
}

int get_min(CImg<int> CDF) {
    int min = CDF(0, 0);

    int w = CDF.width();
    int h = CDF.height();

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            if (min > CDF(i, j)) min = CDF(i, j);
        }
    }

    return min;
}

// CImg<int> hist_equl(CImg<float> img) {
//     CImg<int> cdf_matrix = get_cdf_matrix(img);
//     int cdf_min = get_min(cdf_matrix);
//     int width = cdf_matrix.width();
//     int height = cdf_matrix.height();

//     CImg<int> res(width, height, 1, 1, 0);

//     for (int i = 0; i < width; ++i) {
//         for (int j = 0; j < height; ++j) {
//             int cdf_v = cdf_matrix(i, j);
//             float a = (float) ((cdf_v - cdf_min)*255);
//             float b = (float) ((width*height-cdf_min));

//             res(i, j) = round(a/b);
//         }
//     }
    
//     return res;
// }

float magnitude(float real, float imag) {
    float res = pow(real, 2) + pow(imag, 2);
    return sqrt(res);
}

CImg<float> polar_FFT(CImg<float> img) {
    int max_radius = fmin(img.height(), img.width()) / 2;
    CImg<float> polar(360, max_radius, 1, 1);
    CImgList<float> fft = img.get_FFT(false);

    double K = 360.0;
    for (int rho = 0; rho < 256; ++rho) {
        for (int i = 0; i < 360; ++i) {
            
            double theta = (2.0*i + 1) * M_PI / K;
            int h = round(rho * sin(theta));
            int w = round(rho * cos(theta));
            polar(i, rho) = magnitude( fft(0, 255+w, 255+h), fft(1, 255+w, 255+h) );

        }
    }

    return polar;
}

float* sum_along_rho1(CImg<float> polar_magnitude) {
    int rho = polar_magnitude.height(); // number of rho
    // float sum[rho];
    float *sum = (float*) calloc(rho, sizeof(float));
    for (int r = 0; r < rho; ++r) {
        for (int i = 0; i < 360; ++i) {
            sum[r] += polar_magnitude(i, r);
        }
    }

    return sum;
}

std::vector<int> sum_along_rho(CImg<float> polar_magnitude) {
    int rho = polar_magnitude.height(); // number of rho
    // float sum[rho];
    // float *sum = (float*) calloc(rho, sizeof(float));
    std::vector<int> res;
    for (int r = 0; r < rho; ++r) {
        float sum = 0;
        for (int i = 0; i < 360; ++i) {
            sum += polar_magnitude(i, r);
        }
        res.push_back( (int)sum );
    }

    return res;
}


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


// stuff to implement beta vector--> enc beta vector --> keyfile --> enc beta vector
// 3 functions two of which ana already defined in main.

// then need to implement send paillier cyphertext over TCP --> recieve paillier ciphertext over TCP 
// two functions use existing functions as a springboard

// this function takes an unencrypted betas vector and returns an encrpyted betas vector 
std::vector<paillier_ciphertext_t*> encrypt_betas(std::vector<float> betas, paillier_pubkey_t* pu){

    std::vector<paillier_ciphertext_t*> enc_betas;

    for (int i = 0; i < NUM_BETAS; ++i) {

        paillier_plaintext_t* plain_beta = paillier_plaintext_from_ui((int)abs(betas[i])); // currently, we only consider positive beta values
        paillier_ciphertext_t* enc_beta = paillier_enc(NULL, pu, plain_beta, paillier_get_rand_devurandom);
        enc_betas.push_back(enc_beta);
        
        paillier_freeplaintext(plain_beta);
    }

    return enc_betas;
}

//this function takes encrypted betas vector and writes it to a file  
void write_enc_betas_to_key_file(std::vector<paillier_ciphertext_t*> encrypted_betas, paillier_pubkey_t* pu){

    std::fstream betas_key_file("betas.key", std::fstream::out|std::fstream::trunc|std::fstream::binary);
    std::ostringstream export_str; // use sstream to create one string buffer for the whole vector

    for (int i = 0; i < NUM_BETAS; ++i) {
        
        // The length of the ciphertext is twice the length of the key
        char* char_beta = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, encrypted_betas[i]);
        // Append the bytestring for each beta to the string buffer
        export_str.write(char_beta, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    }

    // Convert the string buffer into char* and write it into the file
    betas_key_file.write(export_str.str().c_str(), PAILLIER_BITS_TO_BYTES(pu->bits)*2*NUM_BETAS);
    betas_key_file.close();
}


std::vector<paillier_ciphertext_t*> read_enc_betas_from_key_file(paillier_pubkey_t* pu){

    std::fstream betas_key_file("betas.key", std::fstream::in|std::fstream::binary); // open the file in read mode
    char* whole_ctxt = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2*NUM_BETAS); // allocate space to store the whole char* array
    betas_key_file.read(whole_ctxt, PAILLIER_BITS_TO_BYTES(pu->bits)*2*NUM_BETAS); // read the whole input at once from the filestream
    std::vector<paillier_ciphertext_t*> enc_betas;// prepare vector for read betas
    for (int i = 0; i < NUM_BETAS; ++i) {
        
        // The length of the ciphertext is twice the length of the key
        char* char_beta = (char*)malloc(PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        // Coppy one beta from the whole array into char_beta
        memcpy(char_beta, whole_ctxt + i*PAILLIER_BITS_TO_BYTES(pu->bits)*2/sizeof(char), PAILLIER_BITS_TO_BYTES(pu->bits)*2/sizeof(char));
        // Import the char* for one beta to paillier_ciphertext_t
        paillier_ciphertext_t* enc_beta = paillier_ciphertext_from_bytes((void*)char_beta, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
        // Push the encrypted beta to the vector
        enc_betas.push_back(enc_beta);

        free(char_beta);
    }

    betas_key_file.close();
    free(whole_ctxt);
    return enc_betas;
}

//encrypts betas from beta vectors and returns a c string for file/tcp output
const char* beta_vector_enc_to_c_str(std::vector<float> betas, paillier_pubkey_t* pu){

    // use sstream to create one string buffer for the whole vector
    std::ostringstream export_str; 
    std::vector<paillier_ciphertext_t*> enc_betas;

    for (int i = 0; i < 256; ++i) {
        paillier_plaintext_t* plain_beta = paillier_plaintext_from_ui((int)abs(betas[i])); // currently, we only consider positive beta values
        paillier_ciphertext_t* enc_beta = paillier_enc(NULL, pu, plain_beta, paillier_get_rand_devurandom);
        enc_betas.push_back(enc_beta);
        
        // The length of the ciphertext is twice the length of the key
        char* char_beta = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_beta);
        // Append the bytestring for each beta to the string buffer
        export_str.write(char_beta, PAILLIER_BITS_TO_BYTES(pu->bits)*2);

        /* CLEANUP */
        paillier_freeplaintext(plain_beta);

    }
    return export_str.str().c_str();
}

const char* enc_beta_vector_to_c_str(std::vector<paillier_ciphertext_t*> enc_betas, paillier_pubkey_t* pu){

    // use sstream to create one string buffer for the whole vector
    std::ostringstream export_str; 
    for (int i = 0; i < 256; ++i) {
        // The length of the ciphertext is twice the length of the key
        char* char_beta = (char*)paillier_ciphertext_to_bytes(PAILLIER_BITS_TO_BYTES(pu->bits)*2, enc_betas[i]);
        // Append the bytestring for each beta to the string buffer
        export_str.write(char_beta, PAILLIER_BITS_TO_BYTES(pu->bits)*2);
    }
    return export_str.str().c_str();
}
