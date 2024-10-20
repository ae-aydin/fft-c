#include "fft.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "complex.h"
#include "util.h"

struct Complex* radix_2_base(struct Complex* x, const int N, const int inverse) {
    if (N <= 1) {
        return x;
    }

    const int half = N / 2;
    struct Complex* temp_even = malloc_cplx_arr(half);
    struct Complex* temp_odd = malloc_cplx_arr(half);

    for (int i = 0; i < half; i++) {
        temp_even[i] = x[2 * i];
        temp_odd[i] = x[2 * i + 1];
    }

    // Recursive call, cannot be freed -> error.
    struct Complex* even = radix_2_base(temp_even, half, inverse);
    struct Complex* odd = radix_2_base(temp_odd, half, inverse);
    assert(even != NULL && odd != NULL);

    struct Complex* X = malloc_cplx_arr(N);

    const struct Complex complex_2 = (struct Complex){2, 0};
    for (int k = 0; k < half; k++) {
        const double factor = inverse ? 2.0 : -2.0;
        const struct Complex t = mul_q(exp_q(factor * M_PI * k / N), odd[k]);
        if (inverse) {
            X[k] = div_q(add_q(even[k], t), complex_2);
            X[k + half] = div_q(sub_q(even[k], t), complex_2);
        } else {
            X[k] = add_q(even[k], t);
            X[k + half] = sub_q(even[k], t);
        }
    }

    free(temp_even);
    free(temp_odd);

    return X;
}

struct Complex* radix_2_fft(struct Complex* x, const int N) {
    return radix_2_base(x, N, 0);
}

struct Complex* radix_2_ifft(struct Complex* x, const int N) {
    return radix_2_base(x, N, 1);
}

struct Complex * dft_base(const struct Complex* x, const int N, const int inverse) {
    struct Complex* X = malloc_cplx_arr(N);

    for (int k = 0; k < N; k++) {
        X[k] = (struct Complex){0, 0};
        for (int n = 0; n < N; n++) {
            const double factor = inverse ? 2.0 : -2.0;
            const double theta = factor * M_PI * n * k / N;
            X[k] = add_q(X[k], mul_q(x[n], exp_q(theta)));
        }
        if (inverse) {
            X[k].real /= N;
            X[k].imag /= N;
        }
    }
    return X;
}

struct Complex* dft(const struct Complex* x, const int N) {
    return dft_base(x, N, 0);
}

struct Complex* idft(const struct Complex* x, const int N) {
    return dft_base(x, N, 1);
}

struct Complex * iter_fft_base(const struct Complex* x, const int N, const int inverse) {
    struct Complex* X = bit_reverse_arr(x, N);

    const int bits = ceil(log2(N));
    for (int s = 1; s <= bits; s++) {
        const int m = 1 << s;
        const double factor = inverse ? 2.0 : -2.0;
        const struct Complex w_m = exp_q(factor * M_PI / m);

        for (int k = 0; k < N; k += m) {
            struct Complex w = {1, 0};

            for (int j = 0; j < m / 2; j++) {
                const struct Complex t = mul_q(w, X[k + j + m / 2]);
                const struct Complex u = X[k + j];
                X[k + j] = add_q(u, t);
                X[k + j + m / 2] = sub_q(u, t);
                w = mul_q(w, w_m);
            }
        }
    }
    if (inverse) {
        for (int i = 0; i < N; i++) {
            X[i] = div_q(X[i], (struct Complex){N, 0});
        }
    }
    return X;
}

struct Complex * iter_fft(const struct Complex *x, const int N) {
    return iter_fft_base(x, N, 0);
}

struct Complex * iter_ifft(const struct Complex* x, const int N) {
    return iter_fft_base(x, N, 1);
}

struct Complex * bluestein_fft_base(const struct Complex* x, const int N, const int inverse) {
    const int M = next_power_of_two(2 * N - 1);

    struct Complex* chirp = malloc_cplx_arr(N);

    const double factor = inverse ? 1.0 : -1.0;
    for (int k = 0; k < N; k++) {
        const double theta = factor * M_PI * (k * k % (N * 2)) / N;
        chirp[k] = exp_q(theta);
    }

    struct Complex* a = calloc_cplx_arr(M);
    struct Complex* b = calloc_cplx_arr(M);

    for (int k = 0; k < N; k++) {
        a[k] = mul_q(x[k], chirp[k]);
        b[k] = conj_q(chirp[k]);
    }

    for (int k = N - 1; k > 0; k--) {
        b[M - k] = conj_q(chirp[k]);
    }

    a = iter_fft(a, M);
    b = iter_fft(b, M);

    for (int k = 0; k < M; k++) {
        a[k] = mul_q(a[k], b[k]);
    }

    a = iter_ifft(a, M);
    struct Complex* X = malloc_cplx_arr(N);

    for (int k = 0; k < N; k++) {
        X[k] = mul_q(a[k], chirp[k]);
    }

    if (inverse) {
        for (int i = 0; i < N; i++) {
            X[i] = div_q(X[i], (struct Complex){N, 0});
        }
    }

    free(chirp);
    free(a);
    free(b);
    return X;
}

struct Complex* bluestein_fft(const struct Complex* x, const int N) {
    return bluestein_fft_base(x, N, 0);
}

struct Complex* bluestein_ifft(const struct Complex* x, const int N) {
    return bluestein_fft_base(x, N, 1);
}

void fft_2d_col(struct Complex** X, struct Complex** x, const int height, const int width, const int inverse) {
    struct Complex* col_arr = malloc_cplx_arr(height);
    for (int j = 0; j < height; j++) {
        col_arr[j] = x[j][width];
    }

    if (inverse) {
        col_arr = bluestein_ifft(col_arr, height);
    } else {
        col_arr = bluestein_fft(col_arr, height);
    }

    for (int j = 0; j < height; j++) {
        X[j][width] = col_arr[j];
    }
    free(col_arr);
}

struct Complex** fft_2d_base(struct Complex** x, const int height, const int width, const int inverse) {
    struct Complex** X = malloc_2d_cplx_arr(height, width);
    struct Complex** temp_X = malloc_2d_cplx_arr(height, width);

    if (inverse) {
        for (int j = 0; j < width; j++) {
            fft_2d_col(X, x, height, j, inverse);
        }

        for (int i = 0; i < height; i++) {
            memcpy(temp_X[i], X[i],  width * sizeof(struct Complex));
        }

        for (int i = 0; i < height; i++) {
            X[i] = bluestein_ifft(X[i], width);
        }
    } else {
        for (int i = 0; i < height; i++) {
            X[i] = bluestein_fft(x[i], width);
        }

        for (int i = 0; i < height; i++) {
            memcpy(temp_X[i], X[i], width * sizeof(struct Complex));
        }

        for (int j = 0; j < width; j++) {
            fft_2d_col(X, X, height, j, inverse);
        }
    }

    free(temp_X);
    return X;
}

struct Complex** fft_2d(struct Complex** x, const int height, const int width) {
    return fft_2d_base(x, height, width, 0);
}

struct Complex** ifft_2d(struct Complex** x, const int height, const int width) {
    return fft_2d_base(x, height, width, 1);
}

struct Complex** fft_shift_2d(struct Complex** x, const int height, const int width) {
    struct Complex** shifted_x = malloc_2d_cplx_arr(height, width);

    const int half_height = height / 2;
    const int half_width = width / 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            const int new_i = (i + half_height) % height;
            const int new_j = (j + half_width) % width;
            shifted_x[i][j] = x[new_i][new_j];
        }
    }

    return shifted_x;
}