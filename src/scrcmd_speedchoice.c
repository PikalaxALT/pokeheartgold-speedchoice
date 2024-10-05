#include "global.h"

#include "field_system.h"
#include "save_speedchoice.h"
#include "scrcmd.h"

BOOL ScrCmd_GetSpeedchoiceAttr(ScriptContext *ctx) {
    int attr             = ScriptReadByte(ctx);
    u16 *dest            = ScriptGetVarPointer(ctx);
    SaveSpeedchoice *ssc = Save_Speedchoice_Get(FieldSystem_GetSaveData(ctx->fieldSystem));
    *dest                = Speedchoice_GetAttr(ssc, attr);
    return FALSE;
}

BOOL ScrCmd_CompareSpeedchoiceAttr(ScriptContext *ctx) {
    int attr              = ScriptReadByte(ctx);
    int val               = ScriptReadByte(ctx);
    SaveSpeedchoice *ssc  = Save_Speedchoice_Get(FieldSystem_GetSaveData(ctx->fieldSystem));
    ctx->comparisonResult = ScrCmd_Util_Compare(Speedchoice_GetAttr(ssc, attr), val);
    return FALSE;
}
