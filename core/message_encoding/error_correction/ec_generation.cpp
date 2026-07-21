#include "ec_generation.h"
#include "generator_polynomial.h"
#include "gf_polynomial.h"
#include <cassert>
#include <vector>

GFPolynomial codewords_to_polynomial(const std::vector<qr::Codeword>& data) {
    GFPolynomial result;
    for (int i = 0; i < data.size(); i++) {
        int bytes_remaining = data.size() - i;
        result.set_coef(bytes_remaining - 1, data[i]);
    }
    return result;
}

std::vector<qr::Codeword>
polynomial_to_codewords(const GFPolynomial& polynomial, int terms_num) {
    std::vector<qr::Codeword> result;
    for (int i = terms_num - 1; i >= 0; i--) {
        result.push_back(polynomial[i]);
    }
    return result;
}

std::vector<qr::Codeword>
generate_ec_codewords(const std::vector<qr::Codeword>& data,
                      int ec_codewords_num) {
    auto message_poly = codewords_to_polynomial(data);
    int terms_num = data.size();
    message_poly.add_all_exp(ec_codewords_num);
    auto generator_poly = get_generator_polynomial(ec_codewords_num);
    generator_poly.add_all_exp(terms_num - 1);

    auto division_result =
        reed_solomon_divide(message_poly, generator_poly, terms_num);
    return polynomial_to_codewords(division_result, ec_codewords_num);
}
