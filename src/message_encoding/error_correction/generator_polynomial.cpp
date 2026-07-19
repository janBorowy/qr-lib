#include "generator_polynomial.h"
#include "alpha_notation_conversion.h"
#include "gf_polynomial.h"

GFPolynomial get_generator_polynomial(unsigned char n) {
    GFPolynomial result;
    result.set_coef(0, 1);
    result.set_coef(1, 1);
    for (int i = 1; i < n; i++) {
        GFPolynomial multplier;
        multplier.set_coef(1, 1);
        multplier.set_coef(0, ALPHA_TO_INT[i]);
        result = result.multiply(multplier);
    }
    return result;
}
