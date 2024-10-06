#include "speedchoice_cv.h"

#include <nitro/section.h>

#pragma section VERSION begin

ALIGN(4)
u8 gRandoCV[sizeof(SaveSpeedchoice)] = { 0 };
ALIGN(4)
char gSpeedchoiceVersion[] = "Pokemon HeartGold Speedchoice v0.0.1";

#pragma section VERSION end

void Speedchoice_EnsureCV(void) {
    SDK_REFER_SYMBOL(gRandoCV);
    SDK_REFER_SYMBOL(gSpeedchoiceVersion);
}
