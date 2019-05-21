#include <CImg.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include "client.h"

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

// Calculate the histogram for a grayscaled image.
std::vector<int> img_histogram(CImg<float> src) {
    std::vector<int> hist(256, 0);
    // CImg built in loop
    cimg_forXY(src, x, y) {
        ++hist[src(x, y)];
    }
    return hist;
}

CImg<float> equalizeHist(CImg<float> img) {
    int L = 256;
    int w = img.width();
    int h = img.height();
    int dimension = w * h;

    std::vector<int> hist = img_histogram(img);
    std::vector<int> equl;
    int count = 0;
    float temp;

    // Find the minimum non-zero value of the cdf
    int cdf_min;
    for (int i = 0, len = hist.size(); i < len; ++i) {
        if (hist[i] != 0) {
            cdf_min = hist[i];
        }
    }

    for (int i = 0, len = hist.size(); i < len; ++i) {
        count += hist[i];
        temp = (count - cdf_min)*(L-1)*1.0/ ((dimension - cdf_min) * 1.0);
        equl.push_back((int)temp);
    }

    CImg<float> res(w, h, 1, 1, 0);
    // CImg built in loop
    cimg_forXY(res, x, y) {
        // calculate histogram equalization result
		res(x, y, 0) = (float)equl[(int)img(x, y)];
    }

    if ( w != 512 || h != 512) {
        if (w > h) {
            int x0 = w/2 - h/2,
                y0 = 0,
                z0 = 0, c0 = 0,
                x1 = w/2 + h/2,
                y1 = h,
                z1 = 0, c1 = 0;
            res.crop(x0, y0, z0, c0, x1, y1, z1, c1);
        } else if (w < h) {
            int x0 = 0,
                y0 = h/2 - w/2,
                z0 = 0, c0 = 0,
                x1 = w,
                y1 = h/2 + 2/2,
                z1 = 0, c1 = 0;
            res.crop(x0, y0, z0, c0, x1, y1, z1, c1);
        }
        res.resize(512, 512);
    }
    
	return res;
}

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

vector<int> preproc_radial_sums(char* filename)
{
	CImg<float> src(filename);
	CImg<float> img = get_grayscale(src);
	img.blur(2.5);
	img = equalizeHist(img);
	CImg<float> polar = polar_FFT(img);
	return sum_along_rho(polar);
}
