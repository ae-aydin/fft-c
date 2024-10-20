#ifndef COMPLEX_H
#define COMPLEX_H

struct Complex {
    double real;
    double imag;
};

struct Complex add_q(struct Complex x, struct Complex y);

struct Complex sub_q(struct Complex x, struct Complex y);

struct Complex mul_q(struct Complex x, struct Complex y);

struct Complex div_q(struct Complex x, struct Complex y);

struct Complex exp_q(double theta);

struct Complex conj_q(struct Complex x);

double amplitude_q(struct Complex x);
#endif  // COMPLEX_H