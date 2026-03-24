#pragma once
#include <PR/ultratypes.h>

#ifdef TARGET_N64
/* For N64 ROM builds: declare the arrays without the PC runtime loader.
   The data is baked into the ROM by the build system. */
#define ROM_ASSET_LOAD_VTX(_name, _pa, _ps, _sa, _ss) \
    Vtx _name[(_ss) / 16];
#define ROM_ASSET_LOAD_TEXTURE(_name, _fn, _pa, _ps, _sa, _ss) \
    ALIGNED8 Texture _name[_ss];
#define ROM_ASSET_LOAD_SAMPLE(_name, _ptr, _pa, _ps, _sa, _ss)
#define ROM_ASSET_LOAD_SEQUENCE(_name, _ptr, _pa, _ps, _sa, _ss)
#define ROM_ASSET_LOAD_COLLISION(_name, _pa, _ps, _sa, _ss) \
    Collision _name[(_ss) / 2];
#define ROM_ASSET_LOAD_ANIM(_name, _pa, _ps, _sa, _ss) \
    static u16 _name[(_ss) / 2];
#define ROM_ASSET_LOAD_ANIM_2D(_name, _pa, _ps, _sa, _ss, _d2) \
    static u16 _name[(_ss) / (2 * _d2)][_d2];
#define ROM_ASSET_LOAD_MARIO_ANIM(_name, _ptr, _pa, _ps, _sa, _ss) \
    static u16 _name[(_ss) / 2];
#define ROM_ASSET_LOAD_DIALOG(_ptr, _pa, _ps, _sa, _ss)
#define ROM_ASSET_LOAD_DEMO(_name, _ptr, _pa, _ps, _sa, _ss)
#else

enum RomAssetType {
    ROM_ASSET_VTX,
    ROM_ASSET_TEXTURE,
    ROM_ASSET_SAMPLE,
    ROM_ASSET_COLLISION,
    ROM_ASSET_ANIM,
    ROM_ASSET_DIALOG,
    ROM_ASSET_DEMO,
};

#define ROM_ASSET_LOAD_VTX(_name, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
Vtx _name[(_segmentedSize) / 16] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_name, ROM_ASSET_VTX, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_TEXTURE(_name, _filename, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
ALIGNED8 Texture _name[_segmentedSize] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_name, ROM_ASSET_TEXTURE, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_SAMPLE(_name, _ptr, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_ptr, ROM_ASSET_SAMPLE, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_SEQUENCE(_name, _ptr, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_ptr, ROM_ASSET_SAMPLE, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_COLLISION(_name, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
Collision _name[(_segmentedSize) / 2] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_name, ROM_ASSET_COLLISION, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_ANIM(_name, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
static u16 _name[(_segmentedSize) / 2] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_name, ROM_ASSET_ANIM, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_ANIM_2D(_name, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize, _secondDim) \
static u16 _name[(_segmentedSize) / (2 * _secondDim)][_secondDim] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_name, ROM_ASSET_ANIM, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_MARIO_ANIM(_name, _ptr, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
static u16 _name[(_segmentedSize) / 2] = { 0 }; \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_ptr, ROM_ASSET_ANIM, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_DIALOG(_ptr, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
__attribute__((constructor)) static void _ptr ## _rom_assets_queue () { \
    rom_assets_queue(_ptr, ROM_ASSET_DIALOG, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

#define ROM_ASSET_LOAD_DEMO(_name, _ptr, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize) \
__attribute__((constructor)) static void _name ## _rom_assets_queue () { \
    rom_assets_queue(_ptr, ROM_ASSET_DEMO, _physicalAddress, _physicalSize, _segmentedAddress, _segmentedSize); \
}

void rom_assets_load(void);
void rom_assets_queue(void* ptr, enum RomAssetType assetType, u32 physicalAddress, u32 physicalSize, u32 segmentedAddress, u32 segmentedSize);
u8* rom_assets_decompress(u32* data, u32* decompressedSize);

#endif /* !TARGET_N64 */
