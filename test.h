#ifndef TEST_H
#define TEST_H

enum FFTType {RADIX_2, ITER_RADIX_2, DFT, BLUESTEIN, FFT_NONE};

enum TestType {FFT1, FFT2, FFT_IMAGE};

void test_fft(enum FFTType fft_type, const double* test_arr, int N);

void test_fft_2d(const double* test_arr, int height, int width);

void test_fft_image(const char* filename, const char* output_filename);

#endif //TEST_H