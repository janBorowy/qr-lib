#include "../../qr.h"
#include <vector>

std::vector<qr::Codeword>
generate_ec_codewords(const std::vector<qr::Codeword>& data,
                      int ec_codewords_num);
