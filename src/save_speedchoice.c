#include "save_speedchoice.h"

#include "global.h"

struct SaveSpeedchoice {
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
};

u32 Save_Speedchoice_sizeof(void) {
    return sizeof(SaveSpeedchoice);
}

void Save_Speedchoice_Init(SaveSpeedchoice *ssc) {
    MI_CpuClearFast(ssc, sizeof(SaveSpeedchoice));
    // temp for testing
    Speedchoice_SetAttr(ssc, SPEEDCHOICE_HOLD_TO_MASH, SPEEDCHOICE_HOLD_TO_MASH_YES);
    Speedchoice_SetAttr(ssc, SPEEDCHOICE_FRIENDLESS, SPEEDCHOICE_FRIENDLESS_ON);
}

SaveSpeedchoice *Save_Speedchoice_Get(SaveData *saveData) {
    GF_ASSERT(SaveSubstruct_AssertCRC(SAVE_SPEEDCHOICE));
    return SaveArray_Get(saveData, SAVE_SPEEDCHOICE);
}

int Speedchoice_GetAttr(SaveSpeedchoice *ssc, int stg) {
    switch (stg) {
    case SPEEDCHOICE_HOLD_TO_MASH:
        return ssc->holdToMash;
    case SPEEDCHOICE_SPINNERS:
        return ssc->spinners;
    case SPEEDCHOICE_ROCKETLESS:
        return ssc->rocketSections;
    case SPEEDCHOICE_HEALTH_BARS:
        return ssc->hpBars;
    case SPEEDCHOICE_BETTER_MARTS:
        return ssc->betterMarts;
    case SPEEDCHOICE_GOOD_EARLY_WILDS:
        return ssc->earlyWilds;
    case SPEEDCHOICE_BIKE_MUSIC:
        return ssc->bikeMusic;
    case SPEEDCHOICE_SURF_MUSIC:
        return ssc->surfMusic;
    case SPEEDCHOICE_EVIL_HAU:
        return ssc->evilHau;
    case SPEEDCHOICE_FRIENDLESS:
        return ssc->friendless;
    case SPEEDCHOICE_KIMONO_SEGMENTS:
        return ssc->kimonoSegments;
    case SPEEDCHOICE_DEX_BEEPS:
        return ssc->dexBeeps;
    case SPEEDCHOICE_HMS:
        return ssc->trainerVision;
    case SPEEDCHOICE_EXP:
        return ssc->hms;
    case SPEEDCHOICE_EARLY_KANTO:
        return ssc->exp;
    case SPEEDCHOICE_TRAINER_VISION:
        return ssc->earlyKanto;
    case SPEEDCHOICE_GOAL:
        return ssc->goal;
    case SPEEDCHOICE_RUNNING_SHOES:
        return ssc->runningShoes;
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
}

void Speedchoice_SetAttr(SaveSpeedchoice *ssc, int stg, int val) {
    switch (stg) {
    case SPEEDCHOICE_HOLD_TO_MASH:
        ssc->holdToMash = val;
        break;
    case SPEEDCHOICE_SPINNERS:
        ssc->spinners = val;
        break;
    case SPEEDCHOICE_ROCKETLESS:
        ssc->rocketSections = val;
        break;
    case SPEEDCHOICE_HEALTH_BARS:
        ssc->hpBars = val;
        break;
    case SPEEDCHOICE_BETTER_MARTS:
        ssc->betterMarts = val;
        break;
    case SPEEDCHOICE_GOOD_EARLY_WILDS:
        ssc->earlyWilds = val;
        break;
    case SPEEDCHOICE_BIKE_MUSIC:
        ssc->bikeMusic = val;
        break;
    case SPEEDCHOICE_SURF_MUSIC:
        ssc->surfMusic = val;
        break;
    case SPEEDCHOICE_EVIL_HAU:
        ssc->evilHau = val;
        break;
    case SPEEDCHOICE_FRIENDLESS:
        ssc->friendless = val;
        break;
    case SPEEDCHOICE_KIMONO_SEGMENTS:
        ssc->kimonoSegments = val;
        break;
    case SPEEDCHOICE_DEX_BEEPS:
        ssc->dexBeeps = val;
        break;
    case SPEEDCHOICE_HMS:
        ssc->trainerVision = val;
        break;
    case SPEEDCHOICE_EXP:
        ssc->hms = val;
        break;
    case SPEEDCHOICE_EARLY_KANTO:
        ssc->exp = val;
        break;
    case SPEEDCHOICE_TRAINER_VISION:
        ssc->earlyKanto = val;
        break;
    case SPEEDCHOICE_GOAL:
        ssc->goal = val;
        break;
    case SPEEDCHOICE_RUNNING_SHOES:
        ssc->runningShoes = val;
        break;
    default:
        GF_ASSERT(FALSE);
        return;
    }
    SaveSubstruct_UpdateCRC(SAVE_SPEEDCHOICE);
}
