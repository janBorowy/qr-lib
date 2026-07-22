# qr-lib

A C++23 library for generating QR codes from scratch, along with a command-line tool for turning text/URLs into QR codes right in your terminal.

The project is split into a few pieces:

| Path   | Description                                                                 |
|--------|------------------------------------------------------------------------------|
| `core` | The `qr-lib` library itself — QR encoding, error correction, masking, etc.   |
| `cli`  | `qr-cli`, a command-line tool built on top of `qr-lib`                      |
| `gui`  | A GUI front-end (work in progress, not yet wired into the default build)    |
| `lib`  | Third-party dependencies, pulled in as git submodules                       |

## Cloning

This repo uses **git submodules** (in `lib/`) for its dependencies (Catch2, CLI11, CImg). Make sure to clone with `--recurse-submodules` so those get pulled down too:

```bash
git clone --recurse-submodules https://github.com/janBorowy/qr-lib.git
cd qr-lib
```

If you already cloned the repo without that flag, you can fetch the submodules afterwards with:

```bash
git submodule update --init --recursive
```

## Building

Requirements:

- CMake 3.23+
- A C++23-capable compiler (recent GCC or Clang)

Configure and build with CMake:

```bash
cmake -S . -B build
cmake --build build
```

This produces, among other things:

- `build/core/libqr-lib.a` — the static library
- `build/cli/qr-cli` — the command-line tool
- `build/core/qr-lib-tests` — the unit test suite (Catch2)

To run the tests:

```bash
ctest --test-dir build
```

## Using the library

The public API is exposed through two headers: `core/include/qrlib.h` and `core/include/QrCode.h`. Link your target against the `qr-lib` CMake target to pick up the include paths automatically.

```cmake
add_subdirectory(qr-lib)
target_link_libraries(your-target PRIVATE qr-lib)
```

The main entry point is `qr::generate_qr`:

```cpp
namespace qr {

enum ErrorCorrectionLevel { L, M, Q, H };

QrCode generate_qr(const std::string& data,
                    ErrorCorrectionLevel ec_level = Q,
                    int version = 0,        // 0 = auto-select smallest version that fits
                    int mask = -1,          // -1 = auto-select best mask
                    bool add_quiet_zone = true);

} // namespace qr
```

It returns a `qr::QrCode`, a small, non-copyable matrix wrapper:

```cpp
class QrCode {
public:
    size_t size() const;              // width/height of the matrix, in modules
    bool operator[](int x, int y) const; // true = dark module, false = light module
};
```

### Example

```cpp
#include "QrCode.h"
#include "qrlib.h"
#include <iostream>

int main() {
    qr::QrCode code = qr::generate_qr("https://github.com/janBorowy/qr-lib",
                                       qr::Q /* error correction */,
                                       0     /* auto version */,
                                       -1    /* auto mask */,
                                       true  /* quiet zone */);

    for (size_t y = 0; y < code.size(); ++y) {
        for (size_t x = 0; x < code.size(); ++x) {
            std::cout << (code[x, y] ? "⬜" : "⬛");
        }
        std::cout << '\n';
    }
}
```

Errors (e.g. data too large for the chosen version) are reported via exceptions, so wrap calls to `generate_qr` in a `try`/`catch` if the input isn't trusted.

## Using the CLI

Once built, `qr-cli` reads a piece of text/URL/data and prints the resulting QR code as a block-character matrix in your terminal:

```bash
./build/cli/qr-cli "https://github.com/janBorowy/qr-lib"
```

Options:

| Flag                              | Description                                                        |
|------------------------------------|----------------------------------------------------------------------|
| `-e, --ec-level <L\|M\|Q\|H>`       | Error correction level (default: `Q`)                                |
| `-v, --qr-version <0-40>`          | QR version to use; `0` auto-selects the smallest one that fits (default) |
| `-m, --mask <-1 to 7>`             | Mask pattern to use; out-of-range value auto-selects the best mask   |
| `--quiet-zone` / `--no-quiet-zone` | Add or remove the quiet-zone border around the code (default: on)    |

Example with explicit options:

```bash
./build/cli/qr-cli --ec-level H --qr-version 5 --no-quiet-zone "Hello, world!"
```

## License

MIT — see [LICENSE](LICENSE) for details.
