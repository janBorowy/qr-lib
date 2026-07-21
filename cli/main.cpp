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
    auto qr =
        generate_qr("https://www.youtube.com/"
                    "watch?v=gpxy71WGzI4&list=RDgpxy71WGzI4&start_radio=1",
                    qr::L);
    qr.display("Your qr code");
    return 0;
}
