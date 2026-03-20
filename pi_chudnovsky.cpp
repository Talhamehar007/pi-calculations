#include "digit_count.h"

#include <gmpxx.h>

#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

struct BSTerm {
    mpz_class P;
    mpz_class Q;
    mpz_class T;
};

static mpz_class C3_OVER_24;

static BSTerm bs(std::int64_t a, std::int64_t b) {
    if (b - a == 1) {
        if (a == 0) {
            return {mpz_class(1), mpz_class(1), mpz_class(13591409)};
        }

        mpz_class ak(static_cast<long>(a));

        // P = (6a-5)(2a-1)(6a-1)
        mpz_class p1 = 6 * ak - 5;
        mpz_class p2 = 2 * ak - 1;
        mpz_class p3 = 6 * ak - 1;
        mpz_class P = p1 * p2 * p3;

        // Q = a^3 * (640320^3 / 24)
        mpz_class Q = ak * ak * ak * C3_OVER_24;

        // T = P * (13591409 + 545140134*a)
        mpz_class T = P * (13591409 + 545140134 * ak);

        if (a & 1) {
            T = -T;
        }

        return {std::move(P), std::move(Q), std::move(T)};
    }

    std::int64_t m = (a + b) / 2;

    BSTerm left = bs(a, m);
    BSTerm right = bs(m, b);

    BSTerm out;
    out.P = left.P * right.P;
    out.Q = left.Q * right.Q;
    out.T = left.T * right.Q + left.P * right.T;
    return out;
}

static mpz_class compute_pi_scaled(std::int64_t digits) {
    if (digits < 1) {
        return mpz_class(3);
    }

    // Compute one extra decimal digit so the final result can be rounded.
    const std::int64_t work_digits = digits + 1;

    // Each Chudnovsky term contributes about 14.1816474627 decimal digits.
    std::int64_t terms = static_cast<std::int64_t>(
        std::ceil(static_cast<long double>(work_digits) / 14.181647462725477L)
    );
    if (terms < 1) {
        terms = 1;
    }

    BSTerm res = bs(0, terms);

    // sqrt(10005 * 10^(2*work_digits))
    mpz_class pow10;
    mpz_ui_pow_ui(
        pow10.get_mpz_t(),
        10,
        static_cast<unsigned long>(2 * work_digits)
    );

    mpz_class sqrt_input = 10005 * pow10;

    mpz_class sqrt_val;
    mpz_sqrt(sqrt_val.get_mpz_t(), sqrt_input.get_mpz_t());

    // pi_scaled_extra = floor(pi * 10^(digits+1))
    mpz_class numerator = 426880 * sqrt_val * res.Q;
    mpz_class pi_scaled_extra = numerator / res.T;

    if (pi_scaled_extra < 0) {
        pi_scaled_extra = -pi_scaled_extra;
    }

    // Round to requested number of digits.
    mpz_class last_digit = pi_scaled_extra % 10;
    mpz_class pi_scaled = pi_scaled_extra / 10;
    if (last_digit >= 5) {
        pi_scaled += 1;
    }

    return pi_scaled;
}

static std::string format_pi(const mpz_class& pi_scaled, std::int64_t digits) {
    std::string s = pi_scaled.get_str();

    if (digits == 0) {
        return s;
    }

    if (static_cast<std::int64_t>(s.size()) <= digits) {
        s.insert(0, static_cast<std::size_t>(digits - s.size() + 1), '0');
    }

    std::string out;
    out.reserve(s.size() + 1);
    out.append(s, 0, s.size() - digits);
    out.push_back('.');
    out.append(s, s.size() - digits, digits);

    return out;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <digits>\n";
            std::cerr << "Accepted formats: 100, 1,000, 1_000, 1k, 1M, 1B\n";
            return 1;
        }

        if (std::string(argv[1]) == "--help") {
            std::cout << "Usage: " << argv[0] << " <digits>\n";
            std::cout << "digits: Number of decimal digits after the decimal point\n";
            std::cout << "Accepted formats: 100, 1,000, 1_000, 1k, 1M, 1B\n";
            return 0;
        }

        std::int64_t digits = parse_digit_count(argv[1]);
        if (digits < 1) {
            throw std::runtime_error("digits must be >= 1");
        }

        // C3_OVER_24 = 640320^3 / 24
        mpz_class c = 640320;
        mpz_class c3;
        mpz_pow_ui(c3.get_mpz_t(), c.get_mpz_t(), 3);
        C3_OVER_24 = c3 / 24;

        mpz_class pi_scaled = compute_pi_scaled(digits);
        std::string out = format_pi(pi_scaled, digits);

        std::cout << out << '\n';

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}
