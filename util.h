#ifndef UTIL_H
#define UTIL_H

/* FFT UTILITIES */
int bit_reverse(int x, int bits);

struct Complex* bit_reverse_arr(const struct Complex* x, int N);

int next_power_of_two(int x);

/* MEMORY ALLOCATION */
struct Complex* malloc_cplx_arr(int N);

struct Complex* calloc_cplx_arr(int N);

struct Complex** malloc_2d_cplx_arr(int height, int width);

struct Complex** calloc_2d_cplx_arr(int height, int width);

void free_2d(struct Complex** arr, int height);

/* CONVERSIONS */
struct Complex* to_cplx_arr(const double *to_convert, int N);

struct Complex** to_2d_cplx_arr(const double *src, int height, int width);

double* to_double_arr(const struct Complex* src, int N);

/* OUTPUT FORMATTING */
void print_cplx(const struct Complex *x);

void print_double_arr(const double *x, int N);

void print_cplx_arr(const struct Complex* x, int N);

void print_2d_cplx_arr(struct Complex** x, int height, int width);

double* to_grayscale(const unsigned char* img, int height, int width, int ch);

double* to_amplitude_arr(struct Complex** x, int height, int width);

unsigned char* to_char_arr(const double* x, int height, int width);

#endif //UTIL_H
