#ifndef GUARD_POKEHEARTGOLD_SPEEDCHOICE_OPTIONS_H
#define GUARD_POKEHEARTGOLD_SPEEDCHOICE_OPTIONS_H

#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "save_speedchoice.h"

typedef struct SpeedchoiceOptions_Args {
    SaveSpeedchoice *speedchoice;
    Options *options;
    PlayerProfile *playerProfile;
} SpeedchoiceOptions_Args;

extern const OVY_MGR_TEMPLATE gOverlayTemplate_SpeedchoiceOptions;

BOOL SpeedchoiceOptions_Init(OVY_MANAGER *mgr, int *pState);
BOOL SpeedchoiceOptions_Main(OVY_MANAGER *mgr, int *pState);
BOOL SpeedchoiceOptions_Exit(OVY_MANAGER *mgr, int *pState);

#endif // GUARD_POKEHEARTGOLD_SPEEDCHOICE_OPTIONS_H
