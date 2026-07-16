#include "ec_generation.h"
#include "generator_polynomial.h"
#include "gf_polynomial.h"
#include <cassert>
#include <iostream>
#include <vector>

// GFPolynomial data_to_polynomial(const std::vector<bool>& data) {
//     GFPolynomial result;
//     int current_value = 0;
//     int bytes = data.size() / 8;
//     for (int byte = 0; byte < bytes; byte++) {
//         int bytes_remaining = bytes - byte;
//         for (auto it = data.rbegin() + 8 * (bytes_remaining - 1);
//              it < data.rbegin() + 8 * bytes_remaining; it++) {
//             if (*it) {
//                 current_value +=
//                     1 << (it - (data.rbegin() + 8 * (bytes_remaining - 1)));
//             }
//         }
//         result.set_coef(bytes_remaining - 1, current_value);
//         current_value = 0;
//     }
//     return result;
// }

GFPolynomial codewords_to_polynomial(const std::vector<qr::Codeword>& data) {
    GFPolynomial result;
    for (int i = 0; i < data.size(); i++) {
        int bytes_remaining = data.size() - i;
        result.set_coef(bytes_remaining - 1, data[i]);
    }
    return result;
}

std::vector<qr::Codeword>
polynomial_to_codewords(const GFPolynomial& polynomial) {
    std::vector<qr::Codeword> result;
    for (int i = polynomial.get_highest_exp(); i >= 0; i--) {
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
    assert(ec_codewords_num == division_result.get_word_count());
    return polynomial_to_codewords(division_result);
}
