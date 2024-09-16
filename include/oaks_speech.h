#ifndef GUARD_POKEHEARTGOLD_OAKS_SPEECH_H
#define GUARD_POKEHEARTGOLD_OAKS_SPEECH_H

#include "bg_window.h"
#include "options.h"
#include "overlay_manager.h"
#include "save.h"
#include "unk_0200CF18.h"
#include "unk_02082908.h"

BOOL OakSpeech_Init(OVY_MANAGER *ovyMan, int *pState);
BOOL OakSpeech_Main(OVY_MANAGER *ovyMan, int *pState);
BOOL OakSpeech_Exit(OVY_MANAGER *ovyMan, int *pState);

#endif // GUARD_POKEHEARTGOLD_OAKS_SPEECH_H
