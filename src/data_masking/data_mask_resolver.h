#pragma once
#include "../../lib/CImg/CImg.h"

using namespace cimg_library;

struct MaskedImage {
    const CImg<unsigned char> img;
    unsigned char mask;
};

MaskedImage get_best_data_mask(const CImg<unsigned char>& img, int version);
