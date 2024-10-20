#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "test.h"

/* Change these to test different cases. */
const double TEST_ARR[] = {1.0, 2.0, 1.0, -1.0, 1.5};
const int TEST_ARR_SIZE = 5; // should match array length

const double TEST_ARR_ALT[] = {0, 1, 2, 3, 4};
const int TEST_ARR_ALT_SIZE = 5; // should match array length

const double TEST_ARR_2P[] = {2.31, 5.4, 7.278, 0.76, 54.99, 6.81, 13.31, 9.67};
const int TEST_ARR_2P_SIZE = 8; // should match array length, should be power of 2

const double TEST_ARR_2P_ALT[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int TEST_ARR_2P_ALT_SIZE = 8; // should match array length, should be power of 2

const double TEST_ARR_2D[] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
const int TEST_ARR_2D_HEIGHT = 4; // height * width should match array length
const int TEST_ARR_2D_WIDTH = 4; // height * width should match array length

void usage() {
    printf("Usage:\n");
    printf("\tprogram_name [FFT1 | FFT2 | FFT_IMAGE] [algorithm | input_file output_file]\n");
    printf("\tFor FFT1, specify one of the algorithms: RADIX_2, DFT, ITER_RADIX_2, BLUESTEIN\n");
    printf("\tFor FFT_IMAGE, specify input and output filenames.\n");
}

int main(const int argc, char *argv[]) {
    enum TestType test_type;
    enum FFTType fft_type = FFT_NONE;
    const char *input_filename = NULL;
    const char *output_filename = NULL;
    const clock_t start_time = clock();

    if (argc < 2) {
        usage();
        return 1;
    }

    if (strcmp(argv[1], "FFT1") == 0) {
        test_type = FFT1;
    } else if (strcmp(argv[1], "FFT2") == 0) {
        test_type = FFT2;
    } else if (strcmp(argv[1], "FFT_IMAGE") == 0) {
        test_type = FFT_IMAGE;
    } else {
        printf("Invalid test specified.\n");
        usage();
        return 1;
    }

    if (test_type == FFT1) {
        if (argc != 3) {
            usage();
            return 1;
        }
        if (strcmp(argv[2], "RADIX_2") == 0) {
            fft_type = RADIX_2;
        } else if (strcmp(argv[2], "DFT") == 0) {
            fft_type = DFT;
        } else if (strcmp(argv[2], "ITER_RADIX_2") == 0) {
            fft_type = ITER_RADIX_2;
        } else if (strcmp(argv[2], "BLUESTEIN") == 0) {
            fft_type = BLUESTEIN;
        } else {
            printf("Invalid algorithm specified.\n");
            usage();
            return 1;
        }
    }

    if (test_type == FFT_IMAGE) {
        if (argc != 4) {
            usage();
            return 1;
        }
        input_filename = argv[2];
        output_filename = argv[3];
    }
    switch (test_type) {
        case FFT1:
            if (fft_type == RADIX_2) {
                test_fft(fft_type, TEST_ARR_2P_ALT, TEST_ARR_2P_ALT_SIZE);
            } else if (fft_type == ITER_RADIX_2) {
                test_fft(fft_type, TEST_ARR_2P, TEST_ARR_2P_SIZE);
            } else if (fft_type == DFT) {
                test_fft(fft_type, TEST_ARR_ALT, TEST_ARR_ALT_SIZE);
            } else if (fft_type == BLUESTEIN) {
                test_fft(fft_type, TEST_ARR, TEST_ARR_SIZE);
            } else {
                printf("Invalid algorithm specified.\n");
                return 1;
            }
            break;
        case FFT2:
            test_fft_2d(TEST_ARR_2D, TEST_ARR_2D_HEIGHT, TEST_ARR_2D_WIDTH);
            break;
        case FFT_IMAGE:
            test_fft_image(input_filename, output_filename);
            break;
    }

    const clock_t end_time = clock();
    const double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.4f seconds\n", elapsed_time);
    return 0;
}