#pragma once
#include "../struct/MutableQrCode.h"

struct MaskedImage {
    MutableQrCode qr;
    unsigned char mask;
};

MaskedImage get_best_data_mask(const MutableQrCode& qr, int version);
MutableQrCode mask_data(const MutableQrCode& qr, int version, int mask);
