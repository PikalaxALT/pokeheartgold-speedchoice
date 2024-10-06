#ifndef GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H
#define GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H

#include "global.h"

#include "constants/speedchoice.h"

#include "save.h"

typedef struct SaveSpeedchoice {
    u32 holdToMash     : 1;
    u32 spinners       : 2;
    u32 rocketSections : 1;
    u32 hpBars         : 2;
    u32 betterMarts    : 1;
    u32 earlyWilds     : 1;
    u32 bikeMusic      : 1;
    u32 surfMusic      : 1;
    u32 evilHau        : 2;
    u32 friendless     : 1;
    u32 kimonoSegments : 1;
    u32 dexBeeps       : 1;
    u32 trainerVision  : 1;
    u32 hms            : 2;
    u32 exp            : 2;
    u32 earlyKanto     : 1;
    u32 goal           : 1;
    u32 runningShoes   : 1;
    u32                : 9;
} SaveSpeedchoice;

u32 Save_Speedchoice_sizeof(void);
void Save_Speedchoice_Init(SaveSpeedchoice *ssc);
SaveSpeedchoice *Save_Speedchoice_Get(SaveData *saveData);
int Speedchoice_GetAttr(SaveSpeedchoice *ssc, int stg);
void Speedchoice_SetAttr(SaveSpeedchoice *ssc, int stg, int val);

#endif // GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H
