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
};

u32 Save_Speedchoice_sizeof(void) {
    return sizeof(SaveSpeedchoice);
}

void Save_Speedchoice_Init(SaveSpeedchoice *ssc) {
    MI_CpuClearFast(ssc, sizeof(SaveSpeedchoice));
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
    default:
        GF_ASSERT(FALSE);
        return;
    }
    SaveSubstruct_UpdateCRC(SAVE_SPEEDCHOICE);
}
