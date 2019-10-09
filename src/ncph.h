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

CImg<float> get_grayscale(CImg<float> src);

CImg<float> apply_filter(CImg<float> img, CImg<float> kernel);

int cdf(CImg<float> I, int x);

CImg<int> get_cdf_matrix(CImg<float> img);

int get_min(CImg<int> CDF);

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

float magnitude(float real, float imag);

CImg<float> polar_FFT(CImg<float> img);

float* sum_along_rho1(CImg<float> polar_magnitude);

std::vector<int> sum_along_rho(CImg<float> polar_magnitude);


int get_rand_index(int const &size);

float get_beta(float mean, float std);

int get_rho(int min_rho, int max_rho);

void get_rho_beta_arr (int num_rhos, std::vector<float> &betas, int min_rho, int max_rho, float beta_mean, float beta_std);

long long int check_sum(std::vector<float> const &betas, std::vector<int> const &rho_sums);

void mult_and_sum( paillier_pubkey_t* pu, paillier_ciphertext_t* sum, std::vector<paillier_ciphertext_t*> const &e_betas, std::vector<int> const &exponent);

void mult_and_sum(paillier_pubkey_t* pu, paillier_ciphertext_t* sum, std::vector<paillier_ciphertext_t*> const &e_betas, std::vector<mpz_class> const &exponent);

// stuff to implement beta vector--> enc beta vector --> keyfile --> enc beta vector
// 3 functions two of which ana already defined in main.

// then need to implement send paillier cyphertext over TCP --> recieve paillier ciphertext over TCP 
// two functions use existing functions as a springboard

// this function takes an unencrypted betas vector and returns an encrpyted betas vector 
std::vector<paillier_ciphertext_t*> encrypt_betas(std::vector<float> betas, paillier_pubkey_t* pu);

//this function takes encrypted betas vector and writes it to a file  
void write_enc_betas_to_key_file(std::vector<paillier_ciphertext_t*> encrypted_betas, paillier_pubkey_t* pu);

std::vector<paillier_ciphertext_t*> read_enc_betas_from_key_file(paillier_pubkey_t* pu);

//encrypts betas from beta vectors and returns a c string for file/tcp output
const char* beta_vector_enc_to_c_str(std::vector<float> betas, paillier_pubkey_t* pu);

const char* enc_beta_vector_to_c_str(std::vector<paillier_ciphertext_t*> enc_betas, paillier_pubkey_t* pu);