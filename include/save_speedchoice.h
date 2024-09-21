#ifndef GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H
#define GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H

#include "global.h"

#include "constants/speedchoice.h"

#include "save.h"

typedef struct SaveSpeedchoice SaveSpeedchoice;

u32 Save_Speedchoice_sizeof(void);
void Save_Speedchoice_Init(SaveSpeedchoice *ssc);
SaveSpeedchoice *Save_Speedchoice_Get(SaveData *saveData);
int Speedchoice_GetAttr(SaveSpeedchoice *ssc, int stg);
void Speedchoice_SetAttr(SaveSpeedchoice *ssc, int stg, int val);

#endif // GUARD_HEARTGOLD_SPEEDCHOICE_SAVE_H
