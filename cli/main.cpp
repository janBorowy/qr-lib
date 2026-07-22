// #include "CLI/App.hpp"
#include "QrCode.h"
#include "qrlib.h"

cimg_library::CImg<unsigned char> convert(const qr::QrCode& qr) {
    cimg_library::CImg<unsigned char> result(qr.size(), qr.size());
    for (int row = 0; row < qr.size(); row++) {
        for (int col = 0; col < qr.size(); col++) {
            result(col, row) = qr[row, col];
        }
    }
    return result;
}

int main() {
    convert(
        qr::generate_qr("https://www.youtube.com/watch?v=xR-E2pwgFJo", qr::L))
        .display();
    return 0;
}
