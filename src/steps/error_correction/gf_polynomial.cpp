#include "gf_polynomial.h"
#include "alpha_notation_conversion.h"

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

GFPolynomial GFPolynomial::multiply(int multiplier) {
    for (auto& [exp, val] : coefs) {
        coefs[exp] = gf_multiplication(val, multiplier);
    }
}

void GFPolynomial::divide(const GFPolynomial& other,
                          GFPolynomial& reminder) const {
    GFPolynomial dividend;
    for (int i = 0; i < coefs.size(); i++) {
        GFPolynomial divisor(other);
        divisor.multiply(coefs.begin()->second);
    }
}

void GFPolynomial::multiply_all_exp(int n) {
    Coefs old_coefs(coefs);

    for (const auto& [exp, val] : old_coefs) {
        coefs.erase(exp);
        coefs[exp + n] = val;
    }
}
