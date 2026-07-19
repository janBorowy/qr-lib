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
    GFPolynomial(const Coefs& coefs);
    GFPolynomial(Coefs&& coefs);
    void set_coef(unsigned char exponent, unsigned char value);
    bool operator==(const GFPolynomial& other) const;
    unsigned char operator[](unsigned char exponent) const;
    unsigned char get_alpha_exp(unsigned char x_exponent) const;

    void add_all_exp(int n);
    void subtract_all_exp(int n);
    GFPolynomial multiply(const GFPolynomial& other) const;
    void multiply(int multiplier);
    void reed_solomon_divide(const GFPolynomial& generator_poly,
                             GFPolynomial& reminder) const;
    unsigned char get_highest_exp() const;
};

GFPolynomial reed_solomon_divide(const GFPolynomial& message_poly,
                                 const GFPolynomial& generator_poly,
                                 int terms_num);
