#include "../lib/cli11/include/CLI/CLI.hpp"
#include "QrCode.h"
#include "qrlib.h"
#include <cstdio>
#include <iostream>
#include <map>
#include <string>

void print_matrix(const qr::QrCode& qr) {
    std::cout << std::endl;
    for (int row = 0; row < qr.size(); row++) {
        for (int col = 0; col < qr.size(); col++) {
            std::cout << (qr[col, row] ? "⬜" : "⬛");
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    CLI::App app{"QR code generator"};
    argv = app.ensure_utf8(argv);

    std::string data;
    app.add_option("data", data, "The text/URL/data to encode into the QR code")
        ->required();

    qr::ErrorCorrectionLevel ec_level = qr::Q;
    const std::map<std::string, qr::ErrorCorrectionLevel> ec_map{
        {"L", qr::L}, {"M", qr::M}, {"Q", qr::Q}, {"H", qr::H}};
    app.add_option("-e,--ec-level", ec_level, "Error correction level")
        ->transform(CLI::CheckedTransformer(ec_map, CLI::ignore_case))
        ->capture_default_str();

    int version = 0;
    app.add_option(
           "-v,--qr-version", version,
           "QR version to use (1-40, 0 = auto-select smallest that fits)")
        ->check(CLI::Range(0, 40))
        ->capture_default_str();

    int mask = -1;
    app.add_option("-m,--mask", mask,
                   "Mask pattern to use (0-7, out-of-range number selects the "
                   "best mask)")
        ->check(CLI::Range(-1, 7))
        ->capture_default_str();

    bool add_quiet_zone = true;
    app.add_flag("--no-quiet-zone{false},--quiet-zone{true}", add_quiet_zone,
                 "Add a quiet zone border around the QR code")
        ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    try {
        print_matrix(
            qr::generate_qr(data, ec_level, version, mask, add_quiet_zone));
    } catch (const std::exception& e) {
        std::cerr << "Error generating QR code: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
