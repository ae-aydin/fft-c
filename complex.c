#include "complex.h"

#include <math.h>

struct Complex add_q(const struct Complex x, const struct Complex y) {
    struct Complex z;
    z.real = x.real + y.real;
    z.imag = x.imag + y.imag;
    return z;
}

struct Complex sub_q(const struct Complex x, const struct Complex y) {
    struct Complex z;
    z.real = x.real - y.real;
    z.imag = x.imag - y.imag;
    return z;
}

struct Complex mul_q(const struct Complex x, const struct Complex y) {
    struct Complex z;
    z.real = x.real * y.real - x.imag * y.imag;
    z.imag = x.real * y.imag + x.imag * y.real;
    return z;
}

struct Complex div_q(const struct Complex x, const struct Complex y) {
    struct Complex z;
    const double denominator = pow(y.real, 2) + pow(y.imag, 2);
    z.real = (x.real * y.real + x.imag * y.imag) / denominator;
    z.imag = (x.imag * y.real - x.real * y.imag) / denominator;
    return z;
}

struct Complex exp_q(const double theta) {
    struct Complex z;
    z.real = cos(theta);
    z.imag = sin(theta);
    return z;
}

struct Complex conj_q(const struct Complex x) {
    struct Complex z;
    z.real = x.real;
    z.imag = -1.0 * x.imag;
    return z;
}

double amplitude_q(const struct Complex x) {
    return sqrt(x.real * x.real + x.imag * x.imag);
}