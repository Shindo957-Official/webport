# Building an N64 ROM

This project is based on the SM64 decompilation and retains the ability to compile back into a Nintendo 64 ROM (`.z64`). The N64 build strips all PC-specific features (networking, Discord, Lua mods, CoopNet) and produces the core gameplay as it would run on real N64 hardware or an emulator.

## Prerequisites

### 1. A legally-owned SM64 ROM

You must own a copy of Super Mario 64. Place the ROM in the root of the repository, named according to its region:

| Region | Filename |
|--------|----------|
| USA (recommended) | `baserom.us.z64` |
| Japan | `baserom.jp.z64` |
| Europe | `baserom.eu.z64` |
| Shindou | `baserom.sh.z64` |

Verify the ROM's SHA-1 hash matches one of the files in the repo (`sm64.us.sha1`, etc.).

### 2. MIPS cross-compiler toolchain

You need a MIPS N64 cross-compiler. Common options:

**Debian/Ubuntu:**
```bash
sudo apt install gcc-mips64-linux-gnuabi64 binutils-mips64-linux-gnuabi64
# Toolchain prefix: mips64-linux-gnuabi64-
```

**Arch Linux (AUR):**
```bash
yay -S mips64-elf-binutils mips64-elf-gcc
# Toolchain prefix: mips64-elf-
```

**macOS (Homebrew):**
```bash
brew install --HEAD gcc-mips64-elf-linux-gnu
# or use a pre-built n64chain toolchain
```

**Pre-built n64chain (all platforms):**
Download from [glankk/n64chain](https://github.com/glankk/n64chain) and add to `PATH`.
Toolchain prefix: `mips64-elf-`

### 3. Python 3 + Pillow

```bash
pip3 install Pillow
```

### 4. `armips` (RSP assembler)

```bash
sudo apt install armips
# or download from https://github.com/Kingcom/armips
```

## Quick Build

Use the provided convenience script:

```bash
./build_n64_rom.sh [VERSION] [CROSS_PREFIX]
```

Examples:
```bash
# US version with mips64-elf toolchain (most common)
./build_n64_rom.sh us mips64-elf-

# US version with Debian/Ubuntu apt toolchain
./build_n64_rom.sh us mips64-linux-gnuabi64-

# Japan version
./build_n64_rom.sh jp mips64-elf-
```

The script will:
1. Check all dependencies
2. Extract assets from your base ROM
3. Compile and link the N64 ROM

Output: `build/us_n64/sm64.us.z64`

## Manual Build

If you prefer to run steps manually:

```bash
# 1. Extract game assets from the base ROM
python3 extract_assets.py us

# 2. Compile the N64 ROM
make TARGET_N64=1 VERSION=us CROSS=mips64-elf- -j$(nproc)
```

Output: `build/us_n64/sm64.us.z64`

## Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `VERSION` | `us` | ROM region (`us`, `jp`, `eu`, `sh`) |
| `CROSS` | *(empty)* | Cross-compiler prefix (e.g. `mips64-elf-`) |
| `NON_MATCHING` | `0` | Skip byte-exact matching; allows GCC instead of IDO |
| `DEBUG` | `0` | Include debug symbols |
| `GRUCODE` | auto | RSP microcode variant (`f3d_old`, `f3dex`, `f3dex2`, etc.) |

### Non-matching build (easier, GCC)

If you don't need a byte-for-byte identical ROM and just want a working build with modern GCC:

```bash
make TARGET_N64=1 VERSION=us CROSS=mips64-elf- NON_MATCHING=1 COMPILER=gcc -j$(nproc)
```

### Matching build (IDO compiler)

A matching build replicates the exact original compiler output and requires the IDO 5.3 compiler (included in `tools/ido5.3_recomp`):

```bash
make TARGET_N64=1 VERSION=us CROSS=mips64-elf- NON_MATCHING=0 -j$(nproc)
```

## Running the ROM

- **Emulator:** Load `build/us_n64/sm64.us.z64` in [Mupen64Plus](https://mupen64plus.org/), [Project64](https://www.pj64-emu.com/), or [Ares](https://ares-emu.net/).
- **Flashcart:** Flash the `.z64` to an EverDrive-64 or similar cartridge to run on real hardware.

## What's excluded in the N64 build

The following PC-specific features are automatically excluded when `TARGET_N64=1`:

- Online multiplayer / networking
- Discord Game SDK
- CoopNet server hosting
- Lua modding API
- SDL2/OpenGL rendering (uses original N64 RSP microcode instead)
- DynOS / dynamic options system
- Custom character support (Lua-driven)
