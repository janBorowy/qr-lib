#include "include/qrlib.h"
#include "include/QrCode.h"
#include "message_encoding/data_analysis.h"
#include "qr_code_generator.h"
#include "qr_color_constants.h"
#include "struct/MutableQrCode.h"
#include <stdexcept>

MutableQrCode with_quiet_zone(MutableQrCode& qr) {
    MutableQrCode result(qr.size() + 8);
    result.draw_rect(WHITE, 0, 0, result.size() - 1, result.size() - 1);
    for (int row = 4; row < result.size() - 4; row++) {
        for (int col = 4; col < result.size() - 4; col++) {
            result[col, row] = qr[col - 4, row - 4];
        }
    }
    return result;
}

qr::QrCode qr::generate_qr(const std::string& data,
                           qr::ErrorCorrectionLevel ec_level, int version,
                           int mask, bool add_quiet_zone) {
    auto encoding_mode = analyze_encoding_mode(data);
    if (version == 0) {
        version =
            determine_smallest_version(data.size(), encoding_mode, ec_level);
    } else if (version < 0 || version > 40) {
        throw std::invalid_argument("version must be in <0,40>");
    }
    auto qr = generate_qr_code(data, ec_level, encoding_mode, version, mask);
    if (add_quiet_zone) {
        return qr::QrCode((with_quiet_zone(qr)));
    }
    return qr::QrCode(MutableQrCode(qr));
}
