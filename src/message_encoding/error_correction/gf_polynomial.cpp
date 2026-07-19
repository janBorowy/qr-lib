#include "gf_polynomial.h"
#include "alpha_notation_conversion.h"
#include <algorithm>
#include <cassert>

GFPolynomial::GFPolynomial() : coefs{} {}
GFPolynomial::GFPolynomial(const Coefs& coefs) : coefs(coefs) {};
GFPolynomial::GFPolynomial(Coefs&& coefs) : coefs{std::move(coefs)} {};

void GFPolynomial::set_coef(unsigned char exponent, unsigned char value) {
    if (value != 0) {
        coefs[exponent] = value;
    } else if (value == 0 && coefs.contains(exponent)) {
        coefs.erase(exponent);
    }
}

bool GFPolynomial::operator==(const GFPolynomial& other) const {
    return coefs == other.coefs;
};

unsigned char GFPolynomial::operator[](unsigned char exponent) const {
    return coefs.contains(exponent) ? coefs.at(exponent) : 0;
}

unsigned char GFPolynomial::get_alpha_exp(unsigned char x_exponent) const {
    return INT_TO_ALPHA[coefs.at(x_exponent)];
}

void substract(const GFPolynomial& first, const GFPolynomial& second,
               GFPolynomial& result) {
    for (int i = 0; i < MAX_COEF; i++) {
        result.set_coef(i, first[i] ^ second[i]);
    }
}

unsigned char gf_multiplication(unsigned char a, unsigned char b) {
    return ALPHA_TO_INT[(INT_TO_ALPHA[a] + INT_TO_ALPHA[b]) % 255];
}

GFPolynomial GFPolynomial::multiply(const GFPolynomial& other) const {
    GFPolynomial result;
    for (const auto& [exp_first, val_first] : coefs) {
        for (const auto& [exp_second, val_second] : other.coefs) {
            auto val_to_add = gf_multiplication(val_first, val_second);
            result.set_coef(exp_first + exp_second,
                            val_to_add ^ result[exp_first + exp_second]);
        }
    }
    return result;
}

void GFPolynomial::multiply(int multiplier) {
    for (auto& [exp, val] : coefs) {
        coefs[exp] = gf_multiplication(val, multiplier);
    }
}

unsigned char GFPolynomial::get_highest_exp() const {
    return coefs.begin()->first;
}

GFPolynomial xor_poly_values(const GFPolynomial& first,
                             const GFPolynomial& second) {

    GFPolynomial result(first);
    unsigned char highest_exp =
        std::max(first.get_highest_exp(), second.get_highest_exp());

    for (int exp = highest_exp; exp >= 0; exp--) {
        result.set_coef(exp, first[exp] ^ second[exp]);
    }
    return result;
}

GFPolynomial reed_solomon_divide(const GFPolynomial& message_poly,
                                 const GFPolynomial& generator_poly,
                                 int terms_num) {
    GFPolynomial dividend(message_poly);
    GFPolynomial divisor(generator_poly);
    int current_exp = message_poly.get_highest_exp();
    for (int i = 0; i < terms_num; i++) {
        GFPolynomial current_div(divisor);
        current_div.multiply(dividend[current_exp]);
        dividend = xor_poly_values(dividend, current_div);
        divisor.subtract_all_exp(1);
        current_exp--;
    }
    return dividend;
}

void GFPolynomial::add_all_exp(int n) {
    assert(n >= 0);

    Coefs old_coefs(coefs);

    for (const auto& [exp, val] : old_coefs) {
        coefs.erase(exp);
        coefs[exp + n] = val;
    }
}

void GFPolynomial::subtract_all_exp(int n) {
    assert(n >= 0);
    assert((--coefs.end())->second - n >= 0);

    Coefs old_coefs(coefs);

    for (auto it = old_coefs.rbegin(); it != old_coefs.rend(); it++) {
        auto& [exp, val] = *it;
        coefs.erase(exp);
        coefs[exp - n] = val;
    }
}
