#include "qr_code_generator.h"
#include "steps/qr.h"

using cimg_library::CImg;

CImg<unsigned char> with_quiet_zone(const CImg<unsigned char>& qr) {
    CImg<unsigned char> img(qr.width() + 8, qr.height() + 8);
    img.fill(255);
    for (int row = 4; row < img.height() - 4; row++) {
        for (int col = 4; col < img.width() - 4; col++) {
            img(col, row) = qr(col - 4, row - 4);
        }
    }
    return img;
}

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
                    "watch?v=dQw4w9WgXcQ&list=RDdQw4w9WgXcQ&start_radio=1",
                    qr::Q);
    with_quiet_zone(qr).display("Your qr code");
    return 0;
}
