#include "qr_matrix_evaluator.h"
#include "evaluators.h"
#include <algorithm>

int evaluate_data_mask(const CImg<unsigned char>& img) {
    int penalty = 0;
    std::for_each(
        QR_EVALUATORS.begin(), QR_EVALUATORS.end(),
        [&penalty, &img](QrEvaluator evaluator) { penalty += evaluator(img); });
    return penalty;
}
