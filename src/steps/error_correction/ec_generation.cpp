#include "ec_generation.h"
#include "generator_polynomial.h"
#include "gf_polynomial.h"
#include <vector>

GFPolynomial data_to_polynomial(const std::vector<bool>& data) {
    GFPolynomial result;
    int current_value = 0;
    int bytes = data.size() / 8;
    for (int byte = 0; byte < bytes; byte++) {
        int bytes_remaining = bytes - byte;
        for (auto it = data.rbegin() + 8 * (bytes_remaining - 1);
             it < data.rbegin() + 8 * bytes_remaining; it++) {
            if (*it) {
                current_value +=
                    1 << (it - (data.rbegin() + 8 * (bytes_remaining - 1)));
            }
        }
        result.set_coef(bytes_remaining - 1, current_value);
        current_value = 0;
    }
    return result;
}

std::vector<bool> generate_ec_codewords(const std::vector<bool>& data,
                                        int ec_codewords_num) {
    auto message_poly = data_to_polynomial(data);
    int terms_num = data.size() / 8;
    message_poly.multiply_all_exp(ec_codewords_num);
    auto generator_poly = get_generator_polynomial(ec_codewords_num);
    // TODO: make sure about this
    generator_poly.multiply_all_exp(terms_num - 1);

    GFPolynomial reminder;
    message_poly.divide(generator_poly, reminder);
    return {};
}
