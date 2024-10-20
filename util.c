#include "util.h"

#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "complex.h"
#include "stdio.h"

const int DEFAULT_DECIMALS = 2;
const int FIXED_WIDTH = 6;

int bit_reverse(int x, const int bits) {
    int rev = 0;
    for (int i = 0; i < bits; i++) {
        rev = rev << 1 | x & 1;
        x >>= 1;
    }
    return rev;
}

struct Complex * bit_reverse_arr(const struct Complex *x, const int N) {
    struct Complex* temp = malloc_cplx_arr(N);
    const int bits = ceil(log2(N));
    for (int i = 0; i < N; i++) {
        temp[i] = x[bit_reverse(i, bits)];
    }
    return temp;
}

int next_power_of_two(const int x) {
    if (x <= 1) return 1;
    return (int) pow(2, ceil(log2(x)));
}

struct Complex* malloc_cplx_arr(const int N) {
    struct Complex* arr = malloc(N * sizeof(struct Complex));
    if (arr == NULL) {
        printf("malloc_cplx_arr failed\n");
    }
    return arr;
}

struct Complex* calloc_cplx_arr(const int N) {
    struct Complex* arr = calloc(N, sizeof(struct Complex));
    if (arr == NULL) {
        fprintf(stderr, "calloc_cplx_arr failed\n");
    }
    return arr;
}

struct Complex** malloc_2d_cplx_arr(const int height, const int width) {
    struct Complex** x = malloc(height * sizeof(struct Complex*));
    if (x == NULL) {
        fprintf(stderr, "malloc_2d_cplx_arr failed\n");
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        x[i] = (struct Complex*) malloc(width * sizeof(struct Complex));
        if (x[i] == NULL) {
            fprintf(stderr, "malloc_2d_cplx_arr failed at row initialization\n");
            return NULL;
        }
    }
    return x;
}

struct Complex** calloc_2d_cplx_arr(const int height, const int width) {
    struct Complex** x = calloc(height, sizeof(struct Complex*));
   if (x == NULL) {
       fprintf(stderr, "calloc_2d_cplx_arr failed\n");
       return NULL;
   }
    for (int i = 0; i < height; i++) {
        x[i] = (struct Complex*) calloc(width, sizeof(struct Complex));
        if (x[i] == NULL) {
            fprintf(stderr, "calloc_2d_cplx_arr failed at row initialization\n");
            return NULL;
        }
    }
    return x;
}

void free_2d(struct Complex** arr, const int height) {
    for (int i = 0; i < height; i++) {
        free(arr[i]);
    }
    free(arr);
}

struct Complex* to_cplx_arr(const double *to_convert, const int N) {
    struct Complex* complex_array = calloc_cplx_arr(N);
    for (int i = 0; i < N; i++) {
        complex_array[i].real = to_convert[i];
    }
    return complex_array;
}

struct Complex** to_2d_cplx_arr(const double *src, const int height, const int width) {
    struct Complex** x = calloc_2d_cplx_arr(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            x[i][j].real = src[i * width + j];
        }
    }
    return x;
}

// Taking the real component of the complex element only
double* to_double_arr(const struct Complex* src, const int N) {
    double* double_array = malloc(N * sizeof(double));
    if (double_array == NULL) {
        fprintf(stderr, "to_double_arr failed\n");
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        double_array[i] = src[i].real;
    }
    return double_array;
}

void print_cplx(const struct Complex *x) {
    printf("%s%.*f %s%.*fi",
        x->real >= 0 ? " " : "",
        DEFAULT_DECIMALS,
        x->real == -0.0 ? +0.0 : x->real,
        x->imag >= 0 ? " " : "",
        DEFAULT_DECIMALS,
        x->imag == -0.0 ? +0.0 : x->imag);
}

void print_double_arr(const double *x, const int N) {
    printf("[ ");
    for (int i = 0; i < N; i++) {
        printf("%.*f", DEFAULT_DECIMALS, x[i]);
        if (i != N - 1) {
            printf(", ");
        }
    }
    printf(" ]\nsize=%d\n", N);
}

void print_cplx_arr(const struct Complex* x, const int N) {
    printf("[");
    for (int i = 0; i < N; i++) {
        print_cplx(&x[i]);
        if (i != N - 1) {
            printf(", ");
        }
    }
    printf(" ]\nsize=%d\n", N);
}

void print_2d_cplx_arr(struct Complex** x, const int height, const int width) {
    assert(height >= 0 && width >= 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            print_cplx(&x[i][j]);
            if (j != width - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
    printf("shape=(%d, %d), size=%d\n", height, width, height * width);
}

double* to_grayscale(const unsigned char* img, const int height, const int width, const int ch) {
    double* grayscale = malloc((height * width) * sizeof(double));
    if (grayscale == NULL) {
        fprintf(stderr, "to_grayscale failed\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            const int idx = (i * width + j) * ch;
            const unsigned char r = img[idx];
            const unsigned char g = img[idx + 1];
            const unsigned char b = img[idx + 2];
            grayscale[i * width + j] = (0.299 * r + 0.587 * g + 0.114 * b) / 255.0;
        }
    }
    return grayscale;
}

double* to_amplitude_arr(struct Complex** x, const int height, const int width) {
    double* flat_amp_arr = malloc(height * width * sizeof(double));
    if (flat_amp_arr == NULL) {
        fprintf(stderr, "to_amplitude_arr failed\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            flat_amp_arr[i * width + j] = log(1 + amplitude_q(x[i][j]));
        }
    }
    return flat_amp_arr;
}

unsigned char* to_char_arr(const double* x, const int height, const int width) {
    unsigned char* image = malloc(height * width * sizeof(unsigned char));
    if (image == NULL) {
        fprintf(stderr, "to_char_arr failed\n");
        return NULL;
    }

    double min = x[0];
    double max = x[0];
    for (int i = 0; i < height * width; i++) {
        if (x[i] < min) {
            min = x[i];
        }
        if (x[i] > max) {
            max = x[i];
        }
    }
    for (int i = 0; i < height * width; i++) {
        image[i] = (unsigned char)((x[i] - min) / (max - min) * 255);
    }
    return image;
}