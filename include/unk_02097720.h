#ifndef HEARTGOLD_UNK_02097720_H
#define HEARTGOLD_UNK_02097720_H

#include "task.h"
#include "overlay_02.h"

typedef struct UnkStruct_02097720 {
    u32 unknown_0;
    u16* unknown_4;
    UnkStruct_Ov02_0224E4EC* unknown_8; // most likely a struct pointer
} UnkStruct_02097720;

void sub_02097720(TaskManager* taskManager, u16* unknownPtr);
BOOL sub_02097754(TaskManager *taskManager);

#endif  // HEARTGOLD_UNK_02097720_HH
