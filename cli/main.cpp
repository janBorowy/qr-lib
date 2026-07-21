#include "CLI/App.hpp"
#include "qrlib.h"

int main(int argc, char **argv) {
    CLI::App app{"qr-cli"};
    argv = app.ensure_utf8(argv);
    return 0;
}
