#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${BUILD_DIR:-$ROOT_DIR/build}"
CXX="${CXX:-c++}"

COMMON_FLAGS=("-O3" "-std=c++17")
GAUSS_CFLAGS=()
GAUSS_LIBS=()
CHUD_CFLAGS=()
CHUD_LIBS=()
ENV_CPPFLAGS=()
ENV_LDFLAGS=()
MODE="system-default"

have_cmd() {
    command -v "$1" >/dev/null 2>&1
}

if [[ -n "${CPPFLAGS:-}" ]]; then
    read -r -a ENV_CPPFLAGS <<< "${CPPFLAGS}"
fi

if [[ -n "${LDFLAGS:-}" ]]; then
    read -r -a ENV_LDFLAGS <<< "${LDFLAGS}"
fi

if ! have_cmd "$CXX"; then
    echo "error: compiler '$CXX' not found" >&2
    exit 1
fi

if have_cmd pkg-config && pkg-config --exists gmp mpfr; then
    read -r -a GAUSS_CFLAGS <<< "$(pkg-config --cflags gmp mpfr)"
    read -r -a GAUSS_LIBS <<< "$(pkg-config --libs gmp mpfr)"

    if pkg-config --exists gmp; then
        read -r -a CHUD_CFLAGS <<< "$(pkg-config --cflags gmp)"
        read -r -a CHUD_LIBS <<< "$(pkg-config --libs gmp)"
        CHUD_LIBS=("-lgmpxx" "${CHUD_LIBS[@]}")
    else
        CHUD_LIBS=("-lgmpxx" "-lgmp")
    fi

    MODE="pkg-config"
elif [[ "$(uname -s)" == "Darwin" ]] && have_cmd brew; then
    GMP_PREFIX="$(brew --prefix gmp)"
    MPFR_PREFIX="$(brew --prefix mpfr)"

    GAUSS_CFLAGS=("-I${GMP_PREFIX}/include" "-I${MPFR_PREFIX}/include")
    GAUSS_LIBS=("-L${GMP_PREFIX}/lib" "-L${MPFR_PREFIX}/lib" "-lmpfr" "-lgmp")

    CHUD_CFLAGS=("-I${GMP_PREFIX}/include")
    CHUD_LIBS=("-L${GMP_PREFIX}/lib" "-lgmpxx" "-lgmp")

    MODE="homebrew"
else
    GAUSS_LIBS=("-lmpfr" "-lgmp")
    CHUD_LIBS=("-lgmpxx" "-lgmp")
fi

mkdir -p "$BUILD_DIR"

echo "Building with mode: $MODE"
echo "Compiler: $CXX"
echo "Output directory: $BUILD_DIR"

"$CXX" \
    "${COMMON_FLAGS[@]}" \
    "${ENV_CPPFLAGS[@]}" \
    "${GAUSS_CFLAGS[@]}" \
    "$ROOT_DIR/GaussPi_CPP.cpp" \
    "${ENV_LDFLAGS[@]}" \
    "${GAUSS_LIBS[@]}" \
    -o "$BUILD_DIR/pi_gauss_legendre"

"$CXX" \
    "${COMMON_FLAGS[@]}" \
    "${ENV_CPPFLAGS[@]}" \
    "${CHUD_CFLAGS[@]}" \
    "$ROOT_DIR/pi_chudnovsky.cpp" \
    "${ENV_LDFLAGS[@]}" \
    "${CHUD_LIBS[@]}" \
    -o "$BUILD_DIR/pi_chudnovsky"

echo "Built: $BUILD_DIR/pi_gauss_legendre"
echo "Built: $BUILD_DIR/pi_chudnovsky"
