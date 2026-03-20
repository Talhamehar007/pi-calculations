# pi-calculations

High-precision π calculators in C++ using two different algorithms:

- `GaussPi_CPP.cpp` — Gauss-Legendre / Brent-Salamin iteration using MPFR
- `pi_chudnovsky.cpp` — Chudnovsky series with binary splitting using GMP

Both programs print π to the requested number of digits after the decimal point.

## Algorithms

### 1. Gauss-Legendre

- Uses arbitrary-precision floating-point arithmetic via MPFR
- Converges quadratically, so the number of correct digits roughly doubles each iteration
- Good general-purpose high-precision approach

### 2. Chudnovsky

- Uses the Chudnovsky series with binary splitting
- Uses arbitrary-precision integer arithmetic via GMP / GMPXX
- Typically faster for very large digit counts

## Requirements

Install the libraries required by the implementation you want to build.

### macOS (Homebrew)

```bash
brew install gmp mpfr pkg-config
```

### Ubuntu / Debian

```bash
sudo apt-get update
sudo apt-get install -y libgmp-dev libmpfr-dev pkg-config g++
```

### Fedora

```bash
sudo dnf install gmp-devel mpfr-devel pkgconf-pkg-config gcc-c++
```

### Arch Linux

```bash
sudo pacman -S gmp mpfr pkgconf gcc
```

## Build

This repo now uses descriptive binary names in the `build/` directory:

- `build/pi_gauss_legendre`
- `build/pi_chudnovsky`

Create the build directory first:

```bash
mkdir -p build
```

### Build both binaries with `build.sh`

```bash
./build.sh
```

The script supports common macOS and Linux setups:

- Uses `pkg-config` when available
- Falls back to Homebrew paths on macOS
- Falls back to standard system linker paths on Linux and other Unix-like environments

Optional overrides:

```bash
CXX=clang++ ./build.sh
CPPFLAGS="-I/custom/include" LDFLAGS="-L/custom/lib" ./build.sh
BUILD_DIR=out ./build.sh
```

### Build both binaries with `pkg-config`

```bash
c++ -O3 -std=c++17 \
  GaussPi_CPP.cpp \
  $(pkg-config --cflags --libs gmp mpfr) \
  -o build/pi_gauss_legendre

c++ -O3 -std=c++17 \
  pi_chudnovsky.cpp \
  $(pkg-config --cflags --libs gmpxx) \
  -o build/pi_chudnovsky
```

### macOS Homebrew build commands

If you prefer explicit Homebrew paths instead of `pkg-config`:

```bash
clang++ -O3 -std=c++17 \
  -I$(brew --prefix gmp)/include \
  -I$(brew --prefix mpfr)/include \
  -L$(brew --prefix gmp)/lib \
  -L$(brew --prefix mpfr)/lib \
  GaussPi_CPP.cpp -lmpfr -lgmp \
  -o build/pi_gauss_legendre

clang++ -O3 -std=c++17 \
  -I$(brew --prefix gmp)/include \
  -L$(brew --prefix gmp)/lib \
  pi_chudnovsky.cpp -lgmpxx -lgmp \
  -o build/pi_chudnovsky
```

## Usage

Both binaries accept digit counts in plain or grouped forms, including suffixes:

- `100`
- `1,000`
- `1_000`
- `1,`
- `1k`, `100k`
- `1M`
- `1B`

Suffixes are case-insensitive: `k`, `m`, and `b` are equivalent to `K`, `M`, and `B`.

### Gauss-Legendre

```bash
./build/pi_gauss_legendre 100
```

Example output:

```text
3.1415926535...
```

Help:

```bash
./build/pi_gauss_legendre --help
```

### Chudnovsky

```bash
./build/pi_chudnovsky 100
```

Example output:

```text
3.1415926535...
```

## Accuracy

Both implementations were checked against MPFR's `mpfr_const_pi` output and matched exactly for:

- `1`
- `2`
- `5`
- `10`
- `50`
- `100`
- `250`
- `500`
- `1000`
- `5000`
- `10000`

These counts refer to digits after the decimal point.

## Notes

- `GaussPi_CPP.cpp` requires both GMP and MPFR at link time
- `pi_chudnovsky.cpp` requires GMP / GMPXX at link time
- Build artifacts are ignored via `.gitignore` under `build/`

## License

This project is licensed under the GNU General Public License v3.0. See `LICENSE` for details.
