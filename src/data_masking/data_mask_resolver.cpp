#include "data_mask_resolver.h"
#include "../qr_color_constants.h"

MaskedImage get_best_data_mask(const CImg<unsigned char>& img, int version) {
    return {.img = img};
}
