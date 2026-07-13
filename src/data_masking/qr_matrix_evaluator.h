#pragma once
#include "../../lib/CImg/CImg.h"
#include "../qr_color_constants.h"
using namespace cimg_library;


int evaluate_first_rule(const CImg<unsigned char>& img);
int evaluate_data_mask(const CImg<unsigned char>& img);
