#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "complex.h"
#include "util.h"
#include "test.h"
#include "fft.h"

void test_fft(const enum FFTType fft_type, const double* test_arr, const int N) {
    struct Complex* test_cplx_arr = to_cplx_arr(test_arr, N);

    printf("TESTING ARRAY:\n");
    print_double_arr(test_arr, N);
    printf("\n");

    printf("CONVERTING TO COMPLEX ARRAY:\n");
    print_cplx_arr(test_cplx_arr, N);
    printf("\n");

    struct Complex* fft_cplx_arr = NULL;
    switch (fft_type) {
        case RADIX_2:
            printf("RADIX_2 ");
            fft_cplx_arr = radix_2_fft(test_cplx_arr, N);
            break;
        case DFT:
            printf("BASIC ");
            fft_cplx_arr = dft(test_cplx_arr, N);
            break;
        case ITER_RADIX_2:
            printf("ITER_RADIX_2 ");
            fft_cplx_arr = iter_fft(test_cplx_arr, N);
            break;
        case BLUESTEIN:
            printf("BLUESTEIN ");
            fft_cplx_arr = bluestein_fft(test_cplx_arr, N);
            break;
        default:
            printf("WRONG FFT SPECIFIED \n");
            break;
    }

    printf("FFT RESULT:\n");
    print_cplx_arr(fft_cplx_arr, N);
    printf("\n");

    struct Complex* ifft_cplx_arr = NULL;
    switch (fft_type) {
        case RADIX_2:
            printf("RADIX_2 ");
            ifft_cplx_arr = radix_2_ifft(fft_cplx_arr, N);
            break;
        case DFT:
            printf("BASIC ");
            ifft_cplx_arr = idft(fft_cplx_arr, N);
            break;
        case ITER_RADIX_2:
            printf("ITER_RADIX_2 ");
            ifft_cplx_arr = iter_ifft(fft_cplx_arr, N);
            break;
        case BLUESTEIN:
            printf("BLUESTEIN ");
            ifft_cplx_arr = bluestein_ifft(fft_cplx_arr, N);
            break;
        default:
            printf("WRONG FFT SPECIFIED \n");
            break;
    }

    printf("INVERSE FFT RESULT:\n");
    print_cplx_arr(ifft_cplx_arr, N);
    printf("\n");

    double* final_arr = to_double_arr(ifft_cplx_arr, N);
    printf("CONVERTING TO DOUBLE ARRAY:\n");
    print_double_arr(final_arr, N);
    printf("\n");

    free(test_cplx_arr);
    free(fft_cplx_arr);
    free(ifft_cplx_arr);
    free(final_arr);
}

void test_fft_2d(const double* test_arr, const int height, const int width) {

    struct Complex** test_cplx_arr_2d = to_2d_cplx_arr(test_arr, height, width);
    printf("2D ARRAY\n");
    print_2d_cplx_arr(test_cplx_arr_2d, height, width);
    printf("\n");

    struct Complex** fft_cplx_arr_2d = fft_2d(test_cplx_arr_2d, height, width);
    printf("2D FFT\n");
    print_2d_cplx_arr(fft_cplx_arr_2d, height, width);
    printf("\n");

    struct Complex** ifft_cplx_arr_2d = ifft_2d(fft_cplx_arr_2d, height, width);
    printf("2D IFFT\n");
    print_2d_cplx_arr(ifft_cplx_arr_2d, height, width);
    printf("\n");

    struct Complex** fft_cplx_arr_2d_shift = fft_shift_2d(fft_cplx_arr_2d, height, width);
    printf("FFT SHIFT\n");
    print_2d_cplx_arr(fft_cplx_arr_2d_shift, height, width);
    printf("\n");

   free_2d(test_cplx_arr_2d, height);
   free_2d(fft_cplx_arr_2d, height);
   free_2d(ifft_cplx_arr_2d, height);
   free_2d(fft_cplx_arr_2d_shift, height);
}

void test_fft_image(const char* filename, const char* output_filename) {
    int width, height, ch;
    printf("Image name: %s\n", filename);
    unsigned char* img = stbi_load(filename, &width, &height, &ch, 0);
    printf("Image size: %dx%d\n", width, height);

    printf("\nConverting image to grayscale...\n");
    double* gray_img = to_grayscale(img, height, width, ch);
    printf("Converting image array to complex type...\n");
    struct Complex** cplx_img = to_2d_cplx_arr(gray_img, height, width);

    printf("Calculating FFT...\n");
    struct Complex** fft_img = fft_2d(cplx_img, height, width);
    printf("Shifting FFT...\n");
    struct Complex** fft_img_shifted = fft_shift_2d(fft_img, height, width);
    printf("Calculating amplitude...\n");
    double* flat_amp_arr = to_amplitude_arr(fft_img_shifted, height, width);
    printf("Preparing the FFT array for saving...\n");
    unsigned char* write_ready_arr = to_char_arr(flat_amp_arr, height, width);

    printf("\nSaving image...\n");
    stbi_write_jpg(output_filename, width, height, 1, write_ready_arr, 100);
    printf("\nImage \n");

    stbi_image_free(img);
    free(gray_img);
    free(cplx_img);
    free(fft_img);
    free(fft_img_shifted);
    free(flat_amp_arr);
    free(write_ready_arr);
}