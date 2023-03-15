#include <iostream>
#include <mpfr.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

void gauss_legendre(int n) {
    mpfr_t a, b, t, p, a_next, b_next, pi;
    mpfr_init2(a, n + 2);
    mpfr_init2(b, n + 2);
    mpfr_init2(t, n + 2);
    mpfr_init2(p, n + 2);
    mpfr_init2(a_next, n + 2);
    mpfr_init2(b_next, n + 2);
    mpfr_init2(pi, n + 2);

    mpfr_set_d(a, 1, MPFR_RNDN);
    mpfr_sqrt_ui(b, 2, MPFR_RNDN);
    mpfr_ui_div(b, 1, b, MPFR_RNDN);
    mpfr_set_d(t, 0.25, MPFR_RNDN);
    mpfr_set_ui(p, 1, MPFR_RNDN);

    int iterations = static_cast<int>(std::ceil(log2(n)));

    for (int i = 0; i < iterations; ++i) {
        mpfr_add(a_next, a, b, MPFR_RNDN);
        mpfr_div_ui(a_next, a_next, 2, MPFR_RNDN);

        mpfr_mul(b_next, a, b, MPFR_RNDN);
        mpfr_sqrt(b_next, b_next, MPFR_RNDN);

        mpfr_sub(pi, a, a_next, MPFR_RNDN);
        mpfr_sqr(pi, pi, MPFR_RNDN);
        mpfr_mul(pi, pi, p, MPFR_RNDN);
        mpfr_sub(t, t, pi, MPFR_RNDN);

        mpfr_set(a, a_next, MPFR_RNDN);
        mpfr_set(b, b_next, MPFR_RNDN);
        mpfr_mul_ui(p, p, 2, MPFR_RNDN);
    }

    mpfr_add(pi, a, b, MPFR_RNDN);
    mpfr_sqr(pi, pi, MPFR_RNDN);
    mpfr_div_ui(pi, pi, 4, MPFR_RNDN);
    mpfr_div(pi, pi, t, MPFR_RNDN);
    mpfr_printf("Pi is approximately %.*Rf\n", n, pi);

    mpfr_clear(a);
    mpfr_clear(b);
    mpfr_clear(t);
    mpfr_clear(p);
    mpfr_clear(a_next);
    mpfr_clear(b_next);
    mpfr_clear(pi);
}


void print_help() {
    std::cout << "Usage: ./PI [n] [--help]" << std::endl;
    std::cout << "n: The number of decimal points you want for pi (default: 10)" << std::endl;
    std::cout << "--help: Display this help message" << std::endl;
}


int main(int argc, char* argv[]) {
    int n = 10;

    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            print_help();
            return 0;
        }
        n = std::atoi(argv[1]);
    }

    mpfr_set_default_prec(n * 2 + 10);
    gauss_legendre(n);
    return 0;
}
