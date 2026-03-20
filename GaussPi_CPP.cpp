#include "digit_count.h"

#include <iostream>
#include <mpfr.h>
#include <cmath>
#include <cstring>
#include <limits>

void gauss_legendre(std::int64_t digits) {
    // Convert decimal digits to bits, with guard bits.
    mpfr_prec_t prec = static_cast<mpfr_prec_t>(std::ceil(digits * 3.3219280948873626)) + 64;

    mpfr_t a, b, t, p, a_next, b_next, tmp, pi;
    mpfr_init2(a, prec);
    mpfr_init2(b, prec);
    mpfr_init2(t, prec);
    mpfr_init2(p, prec);
    mpfr_init2(a_next, prec);
    mpfr_init2(b_next, prec);
    mpfr_init2(tmp, prec);
    mpfr_init2(pi, prec);

    mpfr_set_ui(a, 1, MPFR_RNDN);

    mpfr_sqrt_ui(b, 2, MPFR_RNDN);
    mpfr_ui_div(b, 1, b, MPFR_RNDN);

    mpfr_set_d(t, 0.25, MPFR_RNDN);
    mpfr_set_ui(p, 1, MPFR_RNDN);

    // Quadratic convergence: digits roughly double each iteration.
    int iterations = static_cast<int>(std::ceil(std::log2(static_cast<long double>(digits)))) + 2;

    for (int i = 0; i < iterations; ++i) {
        // a_next = (a + b) / 2
        mpfr_add(a_next, a, b, MPFR_RNDN);
        mpfr_div_ui(a_next, a_next, 2, MPFR_RNDN);

        // b_next = sqrt(a * b)
        mpfr_mul(b_next, a, b, MPFR_RNDN);
        mpfr_sqrt(b_next, b_next, MPFR_RNDN);

        // tmp = p * (a - a_next)^2
        mpfr_sub(tmp, a, a_next, MPFR_RNDN);
        mpfr_sqr(tmp, tmp, MPFR_RNDN);
        mpfr_mul(tmp, tmp, p, MPFR_RNDN);

        // t = t - tmp
        mpfr_sub(t, t, tmp, MPFR_RNDN);

        // a = a_next
        // b = b_next
        // p = 2 * p
        mpfr_set(a, a_next, MPFR_RNDN);
        mpfr_set(b, b_next, MPFR_RNDN);
        mpfr_mul_ui(p, p, 2, MPFR_RNDN);
    }

    // pi = (a + b)^2 / (4 * t)
    mpfr_add(pi, a, b, MPFR_RNDN);
    mpfr_sqr(pi, pi, MPFR_RNDN);

    mpfr_mul_ui(tmp, t, 4, MPFR_RNDN);
    mpfr_div(pi, pi, tmp, MPFR_RNDN);

    mpfr_printf("%.*Rf\n", static_cast<int>(digits), pi);

    mpfr_clear(a);
    mpfr_clear(b);
    mpfr_clear(t);
    mpfr_clear(p);
    mpfr_clear(a_next);
    mpfr_clear(b_next);
    mpfr_clear(tmp);
    mpfr_clear(pi);
}

void print_help() {
    std::cout << "Usage: ./pi_gauss_legendre <digits> [--help]\n";
    std::cout << "digits: Number of decimal digits after the decimal point\n";
    std::cout << "Accepted formats: 100, 1,000, 1_000, 1k, 1M, 1B\n";
    std::cout << "--help: Display this help message\n";
}

int main(int argc, char* argv[]) {
    std::int64_t digits = 10;

    if (argc > 1) {
        if (std::strcmp(argv[1], "--help") == 0) {
            print_help();
            return 0;
        }
        try {
            digits = parse_digit_count(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
            return 1;
        }
    }

    if (digits < 1) {
        std::cerr << "Error: digits must be >= 1\n";
        return 1;
    }

    if (digits > std::numeric_limits<int>::max()) {
        std::cerr << "Error: digit count is too large\n";
        return 1;
    }

    gauss_legendre(digits);
    return 0;
}
