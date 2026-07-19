#include "qrlib.h"
#include "message_encoding/data_analysis.h"
#include "qr_code_generator.h"
#include <stdexcept>

CImg<unsigned char> with_quiet_zone(const CImg<unsigned char>& qr) {
    CImg<unsigned char> img(qr.width() + 8, qr.height() + 8);
    img.fill(255);
    for (int row = 4; row < img.height() - 4; row++) {
        for (int col = 4; col < img.width() - 4; col++) {
            img(col, row) = qr(col - 4, row - 4);
        }
    }
    return img;
}

CImg<unsigned char> generate_qr(const std::string& data,
                                qr::ErrorCorrectionLevel ec_level, int version,
                                int mask, bool add_quiet_zone) {
    auto encoding_mode = analyze_encoding_mode(data);
    if (version == 0) {
        version =
            determine_smallest_version(data.size(), encoding_mode, ec_level);
    } else if (version < 0 || version > 40) {
        throw std::invalid_argument("version must be in <0,40>");
    }
    auto qr = generate_qr(data, ec_level, encoding_mode, version, mask);
    if (add_quiet_zone) {
        return with_quiet_zone(qr);
    }
    return qr;
}
