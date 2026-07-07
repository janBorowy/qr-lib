#pragma once

#include <functional>
#include <map>

constexpr int MAX_COEF = 256;
typedef std::map<unsigned char, unsigned char, std::greater<unsigned char>>
    Coefs;

class GFPolynomial {

    Coefs coefs;

  public:
    GFPolynomial();
    GFPolynomial(Coefs&& coefs);
    void set_coef(unsigned char exponent, unsigned char value);
    bool operator==(const GFPolynomial& other) const;
    unsigned char operator[](unsigned char exponent) const;
    unsigned char get_alpha_exp(unsigned char x_exponent) const;

    GFPolynomial multiply(const GFPolynomial& other) const;
    void divide(const GFPolynomial& other, GFPolynomial& reminder) const;
};
