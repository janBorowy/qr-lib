#pragma once

#include "../../lib/CImg/CImg.h"
#include <array>
#include <functional>

using namespace cimg_library;

int evaluate_first_rule(const CImg<unsigned char>& img);
int evaluate_second_rule(const CImg<unsigned char>& img);
int evaluate_third_rule(const CImg<unsigned char>& img);
int evaluate_fourth_rule(const CImg<unsigned char>& img);

using QrEvaluator = std::function<int(const CImg<unsigned char>&)>;

constexpr auto make_evaluators() {
    return std::array<QrEvaluator, 4>{evaluate_first_rule, evaluate_second_rule,
                                      evaluate_third_rule,
                                      evaluate_fourth_rule};
}

const auto QR_EVALUATORS = make_evaluators();
