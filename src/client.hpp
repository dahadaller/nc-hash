/* client.hpp is the supporting image processing library for program that will run on the client side in the public-key PHash scheme.
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
#define cimg_display 0
#include "CImg.h"
#include <cstdlib>
#include <math.h>
#include <vector>

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