#ifndef FFT_H
#define FFT_H
# include "complex.h"

/* RADIX-2 DIT FFT */
struct Complex* radix_2_base(struct Complex* x, int N, int inverse);

struct Complex* radix_2_fft(struct Complex* x, int N);

struct Complex* radix_2_ifft(struct Complex* x, int N);

/* DFT */
struct Complex* dft_base(const struct Complex* x, int N, int inverse);

struct Complex* dft(const struct Complex* x, int N);

struct Complex* idft(const struct Complex* x, int N);

/* ITERATIVE RADIX-2 FFT */
struct Complex* iter_fft_base(const struct Complex* x, int N, int inverse);

struct Complex* iter_fft(const struct Complex* x, int N);

struct Complex* iter_ifft(const struct Complex* x, int N);

/* BLUESTEIN FFT */
struct Complex * bluestein_fft_base(const struct Complex *x, int N, int inverse);

struct Complex* bluestein_fft(const struct Complex* x, int N);

struct Complex* bluestein_ifft(const struct Complex* x, int N);

/* 2D FFT */
void fft_2d_col(struct Complex** X, struct Complex** x, int height, int width, int inverse);

struct Complex** fft_2d_base(struct Complex** x, int height, int width, int inverse);

struct Complex** fft_2d(struct Complex** x, int height, int width);

struct Complex** ifft_2d(struct Complex** x, int height, int width);

struct Complex** fft_shift_2d(struct Complex** x, int height, int width);

#endif //FFT_H