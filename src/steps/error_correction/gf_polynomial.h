#pragma once

#include <functional>
#include <map>

constexpr int MAX_COEF = 256;

class GFPolynomial {

    std::map<unsigned char, unsigned char, std::greater<unsigned char>> coefs;

  public:
    GFPolynomial();
    void set_coef(unsigned char exponent, unsigned char value);
    unsigned char operator[](unsigned char exponent) const;
    unsigned char get_alpha_exp(unsigned char x_exponent) const;

    void multiply(const GFPolynomial& other, GFPolynomial& result) const;
    void divide(const GFPolynomial& other, GFPolynomial& reminder) const;
};
