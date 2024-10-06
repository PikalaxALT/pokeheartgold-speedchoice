#include "speedchoice_crc.h"

#include <nitro/section.h>

#pragma section VERSION begin

ALIGN(4)
const u8 gRandoCV[4];

#pragma section VERSION end

void Speedchoice_EnsureCV(void) {
    SDK_REFER_SYMBOL(gRandoCV);
}
