#ifndef POKEHEARTGOLD_OVY_109_H
#define POKEHEARTGOLD_OVY_109_H

#include "overlay_manager.h"
#include "save.h"

typedef struct PhotoAlbumArgs {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3[0x2];
    u8 unk5;
    u8 unk6[0x2];
    u32 *unk8;
    SaveData *saveData;
} PhotoAlbumArgs;

BOOL PhotoAlbum_Init(OVY_MANAGER *man, int *state);
BOOL PhotoAlbum_Main(OVY_MANAGER *man, int *state);
BOOL PhotoAlbum_Exit(OVY_MANAGER *man, int *state);

#endif //POKEHEARTGOLD_OVY_109_H
