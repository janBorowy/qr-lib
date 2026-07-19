#include "qrlib.h"

int main() {

    // ver 8
    // auto qr =
    //     generate_qr("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //                 "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
    //                 qr::ErrorCorrectionLevel::Q);

    // ver 2
    // auto qr = with_quiet_zone(generate_qr("Hello, world!", qr::Q));

    // ver 1
    // auto qr = generate_qr("Hello", qr::Q);
    // TODO: fix this breaking! Some EC codewords are calculated incorrectly
    auto qr = generate_qr("https://www.youtube.com/watch?v=xR-E2pwgFJo", qr::L);
    qr.display("Your qr code");
    return 0;
}
