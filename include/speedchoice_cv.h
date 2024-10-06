#ifndef POKEHEARTGOLD_SPEEDCHOICE_CRC_H
#define POKEHEARTGOLD_SPEEDCHOICE_CRC_H

#include "global.h"

#include "save_speedchoice.h"

extern u8 gRandoCV[sizeof(SaveSpeedchoice)];
extern char gSpeedchoiceVersion[];

void Speedchoice_EnsureCV(void);

#endif // POKEHEARTGOLD_SPEEDCHOICE_CRC_H
