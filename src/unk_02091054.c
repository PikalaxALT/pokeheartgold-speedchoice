#include "unk_02091054.h"

#include "global.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3_0 : 2;
    u8 unk3_2 : 6;
} Unk02091054;

static const Unk02091054 _02105F04[81] = {
    {0x27,  0x00, 0x35, 0x01, 0x00},
    { 0x28, 0x01, 0x35, 0x01, 0x00},
    { 0x29, 0x02, 0x36, 0x01, 0x01},
    { 0x2a, 0x03, 0x37, 0x01, 0x02},
    { 0x2b, 0x04, 0x38, 0x01, 0x03},
    { 0x2c, 0x05, 0x39, 0x02, 0x04},
    { 0x2d, 0x06, 0x3a, 0x02, 0x05},
    { 0x2f, 0x07, 0x3b, 0x01, 0x06},
    { 0x2e, 0x08, 0x3c, 0x01, 0x07},
    { 0x31, 0x09, 0x3d, 0x01, 0x08},
    { 0x30, 0x0a, 0x3e, 0x01, 0x09},
    { 0x33, 0x0b, 0x3f, 0x02, 0x0a},
    { 0x32, 0x0c, 0x40, 0x02, 0x0b},
    { 0x34, 0x0d, 0x41, 0x02, 0x0c},
    { 0x35, 0x0e, 0x42, 0x02, 0x0d},
    { 0x36, 0x0f, 0x43, 0x02, 0x0e},
    { 0x37, 0x10, 0x44, 0x02, 0x0f},
    { 0x38, 0x11, 0x49, 0x02, 0x10},
    { 0x39, 0x12, 0x4a, 0x02, 0x11},
    { 0x3a, 0x13, 0x4b, 0x02, 0x12},
    { 0x3b, 0x14, 0x4c, 0x02, 0x13},
    { 0x3c, 0x15, 0x45, 0x02, 0x14},
    { 0x3d, 0x16, 0x46, 0x02, 0x15},
    { 0x3e, 0x17, 0x47, 0x02, 0x16},
    { 0x3f, 0x18, 0x48, 0x02, 0x17},
    { 0x40, 0x19, 0x4d, 0x01, 0x18},
    { 0x41, 0x1a, 0x4e, 0x01, 0x19},
    { 0x42, 0x1b, 0x4f, 0x01, 0x1a},
    { 0x43, 0x1c, 0x50, 0x01, 0x1b},
    { 0x44, 0x1d, 0x51, 0x01, 0x1c},
    { 0x45, 0x1e, 0x52, 0x01, 0x1d},
    { 0x46, 0x1f, 0x53, 0x01, 0x1e},
    { 0x47, 0x20, 0x54, 0x01, 0x1f},
    { 0x48, 0x21, 0x55, 0x01, 0x20},
    { 0x49, 0x22, 0x56, 0x01, 0x21},
    { 0x4a, 0x23, 0x57, 0x02, 0x22},
    { 0x4b, 0x24, 0x58, 0x02, 0x23},
    { 0x4c, 0x25, 0x59, 0x01, 0x24},
    { 0x4d, 0x26, 0x5a, 0x01, 0x25},
    { 0x4e, 0x27, 0x5b, 0x01, 0x26},
    { 0x4f, 0x28, 0x5c, 0x01, 0x27},
    { 0x50, 0x29, 0x5d, 0x01, 0x28},
    { 0x51, 0x2a, 0x5e, 0x01, 0x29},
    { 0x52, 0x2b, 0x5f, 0x01, 0x2a},
    { 0x53, 0x2c, 0x60, 0x01, 0x2b},
    { 0x54, 0x2d, 0x61, 0x01, 0x2c},
    { 0x55, 0x2e, 0x62, 0x01, 0x2d},
    { 0x56, 0x2f, 0x63, 0x01, 0x2e},
    { 0x57, 0x30, 0x64, 0x01, 0x2f},
    { 0x58, 0x31, 0x65, 0x01, 0x30},
    { 0x59, 0x32, 0x66, 0x00, 0x00},
    { 0x5a, 0x33, 0x67, 0x00, 0x00},
    { 0x5b, 0x34, 0x68, 0x00, 0x00},
    { 0x5c, 0x35, 0x69, 0x00, 0x00},
    { 0x5d, 0x36, 0x6a, 0x00, 0x00},
    { 0x5e, 0x37, 0x6b, 0x00, 0x00},
    { 0x5f, 0x38, 0x6c, 0x00, 0x00},
    { 0x60, 0x39, 0x6d, 0x00, 0x00},
    { 0x61, 0x3a, 0x6e, 0x00, 0x00},
    { 0x62, 0x3b, 0x6f, 0x00, 0x00},
    { 0x63, 0x3c, 0x70, 0x00, 0x00},
    { 0x64, 0x3d, 0x71, 0x00, 0x00},
    { 0x65, 0x3e, 0x72, 0x00, 0x00},
    { 0x66, 0x3f, 0x73, 0x00, 0x00},
    { 0x67, 0x40, 0x74, 0x00, 0x00},
    { 0x68, 0x41, 0x75, 0x00, 0x00},
    { 0x69, 0x42, 0x76, 0x00, 0x00},
    { 0x6a, 0x43, 0x77, 0x00, 0x00},
    { 0x6b, 0x44, 0x78, 0x00, 0x00},
    { 0x6c, 0x45, 0x79, 0x00, 0x00},
    { 0x6d, 0x46, 0x7a, 0x00, 0x00},
    { 0x6e, 0x47, 0x7b, 0x00, 0x00},
    { 0x6f, 0x48, 0x7c, 0x00, 0x00},
    { 0x70, 0x49, 0x7d, 0x00, 0x00},
    { 0x71, 0x4a, 0x7e, 0x00, 0x00},
    { 0x72, 0x4b, 0x7f, 0x00, 0x00},
    { 0x73, 0x4c, 0x80, 0x00, 0x00},
    { 0x74, 0x4d, 0x81, 0x00, 0x00},
    { 0x75, 0x4e, 0x82, 0x00, 0x31},
    { 0x76, 0x4f, 0x83, 0x00, 0x32},
    { 0x77, 0x50, 0x84, 0x00, 0x33},
};

// Has something to do with seals...
u8 sub_02091054(u32 idx) {
    GF_ASSERT(idx < NELEMS(_02105F04));
    return _02105F04[idx].unk0;
}

u8 sub_0209106C(u8 idx) {
    GF_ASSERT(idx < NELEMS(_02105F04));
    return _02105F04[idx].unk1;
}

u8 sub_02091084(u32 idx) {
    GF_ASSERT(idx < NELEMS(_02105F04));
    return _02105F04[idx].unk2;
}

BOOL sub_0209109C(u32 idx) {
    GF_ASSERT(idx < NELEMS(_02105F04));
    if (idx < 0x32 || idx > 0x4d) {
        return FALSE;
    }

    return TRUE;
}

u32 sub_020910B8(u32 idx) {
    GF_ASSERT(idx < NELEMS(_02105F04));
    return _02105F04[idx].unk3_2;
}
