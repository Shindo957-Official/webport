#!/usr/bin/env bash
# build_n64_rom.sh - Build an N64 ROM (.z64) from this project
#
# Prerequisites:
#   - mips64-elf binutils/gcc (cross-compiler targeting MIPS N64)
#     On Debian/Ubuntu: sudo apt install gcc-mips64-linux-gnuabi64 binutils-mips64-linux-gnuabi64
#     Or install mips64-elf-* from a devkitPro / n64chain toolchain
#   - python3 with pip packages: Pillow, pycparser, pypng
#   - A legally-owned SM64 US ROM named "baserom.us.z64" in this directory
#     (other regions: baserom.jp.z64, baserom.eu.z64, baserom.sh.z64)
#
# Usage:
#   ./build_n64_rom.sh [VERSION] [CROSS_PREFIX] [extra make args...]
#
#   VERSION       - ROM region: us (default), jp, eu, sh
#   CROSS_PREFIX  - MIPS toolchain prefix (default: mips64-elf-)
#   extra args    - passed directly to make (e.g. NON_MATCHING=1 DEBUG=1)
#
# Examples:
#   ./build_n64_rom.sh
#   ./build_n64_rom.sh us mips64-elf-
#   ./build_n64_rom.sh jp mips64-linux-gnuabi64-
#   ./build_n64_rom.sh us mips64-elf- NON_MATCHING=1

set -e

VERSION="${1:-us}"
CROSS="${2:-mips64-elf-}"
shift 2 2>/dev/null || true
EXTRA_ARGS="$*"

BASEROM="baserom.${VERSION}.z64"

echo "=== SM64 CoopDX - N64 ROM Build ==="
echo "  Version:  $VERSION"
echo "  Toolchain prefix: $CROSS"
echo ""

# --- Dependency checks ---

check_command() {
    if ! command -v "$1" &>/dev/null; then
        echo "ERROR: '$1' not found. $2"
        exit 1
    fi
}

check_command python3 "Install python3."
check_command "${CROSS}as" "Install the MIPS cross-assembler. Try: sudo apt install binutils-mips64-linux-gnuabi64 (and set CROSS to mips64-linux-gnuabi64-)"
check_command "${CROSS}gcc" "Install the MIPS cross-compiler. Try: sudo apt install gcc-mips64-linux-gnuabi64 (and set CROSS to mips64-linux-gnuabi64-)"
check_command "${CROSS}objcopy" "Install MIPS binutils."

python3 -c "import PIL" 2>/dev/null || {
    echo "ERROR: Python 'Pillow' package not found. Run: pip3 install Pillow"
    exit 1
}

# --- Check for base ROM ---

if [ ! -f "$BASEROM" ]; then
    echo "ERROR: Base ROM '$BASEROM' not found."
    echo "  Place a legally-owned SM64 ${VERSION^^} ROM named '$BASEROM' in this directory."
    echo "  Supported versions: us, jp, eu, sh"
    exit 1
fi

# --- Extract assets ---

echo "[1/3] Extracting assets from $BASEROM..."
python3 extract_assets.py "$VERSION"

# --- Build ROM ---

JOBS="${JOBS:-$(nproc 2>/dev/null || echo 4)}"
echo "[2/3] Compiling (using $JOBS parallel jobs)..."
make TARGET_N64=1 VERSION="$VERSION" CROSS="$CROSS" -j"$JOBS" $EXTRA_ARGS

# --- Done ---

ROM="build/${VERSION}_n64/sm64.${VERSION}.z64"
echo ""
echo "[3/3] Build complete!"
echo "  Output ROM: $ROM"
echo ""
echo "You can load it in an N64 emulator (e.g. Mupen64Plus, Project64, Ares)"
echo "or flash it to a cartridge using an appropriate flashcart."
