#include "../qr.h"
#include <string>
#include <vector>
constexpr int MODE_INDICATOR_SIZE = 4;

// for BYTE mode, input string data MUST be in ISO-8859-1
std::vector<qr::Codeword> encode_data(const std::string& data,
                                      qr::EncodingMode mode,
                                      unsigned char version,
                                      qr::ErrorCorrectionLevel ec_level);
