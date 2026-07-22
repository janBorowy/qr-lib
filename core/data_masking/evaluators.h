#pragma once

#include "../struct/MutableQrCode.h"

#include <array>
#include <functional>

int evaluate_first_rule(const MutableQrCode& img);
int evaluate_second_rule(const MutableQrCode& img);
int evaluate_third_rule(const MutableQrCode& img);
int evaluate_fourth_rule(const MutableQrCode& img);

using QrEvaluator = std::function<int(const MutableQrCode&)>;

constexpr auto make_evaluators() {
    return std::array<QrEvaluator, 4>{evaluate_first_rule, evaluate_second_rule,
                                      evaluate_third_rule,
                                      evaluate_fourth_rule};
}

const auto QR_EVALUATORS = make_evaluators();
