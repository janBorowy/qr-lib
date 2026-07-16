#include "qr_code_generator.h"
#include "steps/qr.h"

int main() {
    // ver 8
    // auto qr =
    //     generate_qr("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //                 "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
    //                 qr::ErrorCorrectionLevel::Q);
    //
    // ver 2
    // auto qr = generate_qr("Hello, world!", qr::Q);

    // ver 1
    auto qr = generate_qr("Hello", qr::Q);
    qr.display("Your qr code");
    return 0;
}
