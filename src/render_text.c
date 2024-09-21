#include "render_text.h"

#include "global.h"

#include "constants/sndseq.h"

#include "font.h"
#include "render_window.h"
#include "string_control_code.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

static u16 unk00;
static TextFlags sTextFlags;
static TouchscreenHitbox sTouchScreenHitbox;

static const u8 _020F56BC[] = { 0, 1, 2, 1 };

static BOOL sub_020021A0(TextPrinterSubStruct *subStruct);
static BOOL sub_02002220(void);
static void TextPrinterInitDownArrowCounters(TextPrinter *printer);
static void TextPrinterDrawDownArrow(TextPrinter *printer);
static void TextPrinterClearDownArrow(TextPrinter *printer);
static BOOL TextPrinterContinue(TextPrinter *printer);
static BOOL TextPrinterWaitAutoMode(TextPrinter *printer);
static BOOL TextPrinterWaitWithDownArrow(TextPrinter *printer);
static u8 TextPrinterWait(TextPrinter *printer);

static BOOL sub_020021A0(TextPrinterSubStruct *subStruct) {
    if ((gSystem.heldKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) && subStruct->hasPrintBeenSpedUp) {
        sTextFlags.unk1_0 = FALSE;
        return TRUE;
    }

    if (sTextFlags.unk0_4) {
        if (!gSystem.touchHeld) {
            return FALSE;
        }

        if (sTextFlags.unk1_1) {
            if (TouchscreenHitbox_TouchHeldIsIn(&sTouchScreenHitbox)) {
                sTextFlags.unk1_0 = TRUE;
                return TRUE;
            }
            return FALSE;
        } else {
            sTextFlags.unk1_0 = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL sub_02002220(void) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        sTextFlags.unk1_0 = FALSE;
        return TRUE;
    }

    if (sTextFlags.unk0_4) {
        if (!gSystem.touchNew) {
            return FALSE;
        }

        if (sTextFlags.unk1_1) {
            if (TouchscreenHitbox_TouchHeldIsIn(&sTouchScreenHitbox)) {
                sTextFlags.unk1_0 = TRUE;
                return TRUE;
            }
            return FALSE;
        } else {
            sTextFlags.unk1_0 = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

RenderResult RenderText(TextPrinter *printer) {
    TextPrinterSubStruct *subStruct = (TextPrinterSubStruct *)(&printer->subStructFields);
    u16 currentChar;

    switch (printer->state) {
    case 0:
        if (sub_020021A0(subStruct)) {
            printer->delayCounter = 0;
            if (printer->textSpeedBottom != 0) {
                sTextFlags.unk0_6 = 1;
            }
        }

        if (printer->delayCounter && printer->textSpeedBottom) {
            printer->delayCounter--;

            if (sTextFlags.canABSpeedUpPrint != 0) {
                if (sub_02002220()) {
                    subStruct->hasPrintBeenSpedUp = 1;
                    printer->delayCounter         = 0;
                }
            }

            return RENDER_UPDATE;
        }

        printer->delayCounter = printer->textSpeedBottom;
        currentChar           = *printer->template.currentChar.raw;
        printer->template.currentChar.raw++;

        GF_ASSERT(currentChar != 0xF100);

        switch (currentChar) {
        case EOS:
            return RENDER_FINISH;
        case CHAR_LF:
            printer->template.currentX = printer->template.x;
            s32 fontAttribute          = GetFontAttribute(printer->template.fontId, 1);

            printer->template.currentY += printer->template.lineSpacing + fontAttribute;

            return RENDER_REPEAT;

        case 0xF0FD:
            printer->template.currentChar.raw++;
            return RENDER_REPEAT;
        case EXT_CTRL_CODE_BEGIN:
            printer->template.currentChar.raw--;
            switch ((u16)MsgArray_GetControlCode(printer->template.currentChar.raw)) {
            case 0xFF00:
                u16 field = (u16)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                if (field == 0xff) {
                    u8 r2                   = printer->template.unk1B;
                    printer->template.unk1B = (u8)((printer->template.fgColor - 1) / 2 + 100);

                    if (!(r2 >= 100 && r2 < 107)) {
                        break;
                    }

                    field = (u16)(r2 - 100);
                } else {
                    if (field >= 0x64) {
                        printer->template.unk1B = (u8)field;
                        break;
                    }
                }

                printer->template.fgColor     = (u8)(field * 2 + 1);
                printer->template.shadowColor = (u8)(field * 2 + 2);

                GenerateFontHalfRowLookupTable(printer->template.fgColor,
                    printer->template.bgColor,
                    printer->template.shadowColor);

                break;

            case 0x200:
                field = (u16)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);

                RenderScreenFocusIndicatorTile(printer,
                    printer->template.currentX,
                    printer->template.currentY,
                    field);
                if (printer->textSpeedTop != 0) {
                    CopyWindowToVram(printer->template.window);
                }

                break;
            case 0x207:
                printer->state = 7;
                TextPrinterInitDownArrowCounters(printer);
                printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);
                if (*printer->template.currentChar.raw == 0xE000) {
                    ++printer->template.currentChar.raw;
                }
                return RENDER_UPDATE;
            case 0x208:
                printer->state = 8;
                TextPrinterInitDownArrowCounters(printer);
                printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);
                if (*printer->template.currentChar.raw == 0xE000) {
                    ++printer->template.currentChar.raw;
                }
                return RENDER_UPDATE;
            case 0x201:
                printer->delayCounter = (u8)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);
                printer->state                    = 6;

                return RENDER_UPDATE;
            case 0x202:
                printer->unk2E = (u16)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);

                return RENDER_UPDATE;
            case 0x203:
                printer->template.currentX = MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                break;
            case 0x204:
                printer->template.currentY = MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                break;
            case 0x205: {
                int x     = GetWindowWidth(printer->template.window) * 8;
                int width = FontID_FlatArray_GetWidthFirstLine(printer->template.fontId, printer->template.currentChar.raw, printer->template.letterSpacing);
                if (width < x) {
                    printer->template.currentX = printer->template.x + (x - width) / 2;
                } else {
                    printer->template.currentX = printer->template.x;
                }
            } break;
            case 0x206: {
                int x     = GetWindowWidth(printer->template.window) * 8;
                int width = FontID_FlatArray_GetWidthFirstLine(printer->template.fontId, printer->template.currentChar.raw, printer->template.letterSpacing);
                if (width < x) {
                    printer->template.currentX = x - width;
                } else {
                    printer->template.currentX = printer->template.x;
                }
            } break;

            case 0xFF01:
                field = (u16)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);

                switch (field) {
                case 100:
                    printer->template.unk18 = 0;
                    printer->template.unk1A = 0;
                    break;
                case 200:
                    printer->template.unk18 = 0xFFFC;
                    printer->template.unk1A = 0;
                    break;
                }

                break;

            case 0xFE06:
                field = (u16)MsgArray_ControlCodeGetField(
                    printer->template.currentChar.raw, 0);
                if (field != 0xFE00) {
                    if (field != 0xFE01) {
                        break;
                    }

                    printer->state = 2;
                    TextPrinterInitDownArrowCounters(printer);
                    printer->template.currentChar.raw = MsgArray_SkipControlCode(
                        printer->template.currentChar.raw);

                    return RENDER_UPDATE;
                }

                printer->state = 3;
                TextPrinterInitDownArrowCounters(printer);
                printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);

                return RENDER_UPDATE;
            }

            printer->template.currentChar.raw = MsgArray_SkipControlCode(printer->template.currentChar.raw);
            return RENDER_REPEAT;

        case 0x25BC:
            printer->state = 2;
            TextPrinterInitDownArrowCounters(printer);
            return RENDER_UPDATE;

        case 0x25BD:
            printer->state = 3;
            TextPrinterInitDownArrowCounters(printer);
            return RENDER_UPDATE;
        }

        GlyphInfo *r5 = FontID_TryLoadGlyph(subStruct->glyphId, currentChar);
        CopyGlyphToWindow(printer->template.window,
            r5->data,
            r5->width,
            r5->height,
            printer->template.currentX,
            printer->template.currentY,
            printer->template.unk18);

        printer->template.currentX += r5->width + printer->template.letterSpacing;

        return RENDER_PRINT;
    case 1:
        if (TextPrinterWait(printer) != 0) {
            TextPrinterClearDownArrow(printer);

            printer->state = 0;
        }

        return RENDER_UPDATE;
    case 2:
        if (TextPrinterWaitWithDownArrow(printer) != 0) {
            TextPrinterClearDownArrow(printer);
            FillWindowPixelBuffer(
                printer->template.window, printer->template.bgColor);
            printer->template.currentX = printer->template.x;
            printer->template.currentY = printer->template.y;
            printer->state             = 0;
        }

        return RENDER_UPDATE;
    case 3:
        if (TextPrinterWaitWithDownArrow(printer) != 0) {
            TextPrinterClearDownArrow(printer);
            printer->scrollDistance    = (u8)(GetFontAttribute(printer->template.fontId, 1) + printer->template.lineSpacing);
            printer->template.currentX = printer->template.x;
            printer->state             = 4;
        }

        return RENDER_UPDATE;
    case 4:
        if (printer->scrollDistance != 0) {
            if ((int)printer->scrollDistance < 4) {
                ScrollWindow(printer->template.window,
                    0,
                    printer->scrollDistance,
                    (u8)(printer->template.bgColor | (printer->template.bgColor << 4)));
                printer->scrollDistance = 0;
            } else {
                ScrollWindow(printer->template.window,
                    0,
                    4,
                    (u8)(printer->template.bgColor | (printer->template.bgColor << 4)));

                printer->scrollDistance -= 4;
            }

            CopyWindowToVram(printer->template.window);
        } else {
            printer->state = 0;
        }

        return RENDER_UPDATE;
    case 5:
        printer->state = 0;
        return RENDER_UPDATE;
    case 6:
        if (printer->delayCounter != 0) {
            printer->delayCounter--;
        } else {
            printer->state = 0;
        }

        return RENDER_UPDATE;

    case 7:
        if (TextPrinterWaitWithDownArrow(printer)) {
            TextPrinterClearDownArrow(printer);
            FillWindowPixelBuffer(printer->template.window, printer->template.bgColor);
            printer->template.currentX = printer->template.x;
            printer->template.currentY = printer->template.y;
            printer->state             = 0;
        }
        return RENDER_UPDATE;
    case 8:
        if (TextPrinterWaitWithDownArrow(printer)) {
            TextPrinterClearDownArrow(printer);
            printer->scrollDistance    = printer->template.lineSpacing + GetFontAttribute(printer->template.fontId, 1);
            printer->template.currentX = printer->template.x;
            printer->state             = 4;
        }
        return RENDER_UPDATE;
    }

    return RENDER_FINISH;
}

void sub_020027F0(int flag) {
    unk00 = flag;
}

static void TextPrinterInitDownArrowCounters(TextPrinter *printer) {
    TextPrinterSubStruct *subStruct = (TextPrinterSubStruct *)(&printer->subStructFields);

    if (sTextFlags.autoScroll) {
        subStruct->autoScrollDelay = 0;
    } else {
        subStruct->downArrowYPosIdx = 0;
        subStruct->downArrowDelay   = 0;
    }
    sub_0200EB68(printer->template.window, unk00);
}

static void TextPrinterDrawDownArrow(TextPrinter *printer) {
    TextPrinterSubStruct *subStruct = (TextPrinterSubStruct *)(&printer->subStructFields);

    if (sTextFlags.autoScroll || sTextFlags.useAlternateDownArrow) {
        return;
    }

    if (subStruct->downArrowDelay != 0) {
        subStruct->downArrowDelay--;
        return;
    }

    u8 bg_id  = GetWindowBgId(printer->template.window);
    u8 x      = GetWindowX(printer->template.window);
    u8 y      = GetWindowY(printer->template.window);
    u8 width  = GetWindowWidth(printer->template.window);
    u8 height = GetWindowHeight(printer->template.window) - 2;
    u16 r6    = unk00;

    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 18 + _020F56BC[subStruct->downArrowYPosIdx] * 4),
        (u8)(x + width + 1),
        (u8)(y + height),
        1,
        1,
        0x10);

    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 19 + _020F56BC[subStruct->downArrowYPosIdx] * 4),
        (u8)(x + width + 2),
        (u8)(y + height),
        1,
        1,
        0x10);
    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 20 + _020F56BC[subStruct->downArrowYPosIdx] * 4),
        (u8)(x + width + 1),
        (u8)(y + height + 1),
        1,
        1,
        0x10);
    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 21 + _020F56BC[subStruct->downArrowYPosIdx] * 4),
        (u8)(x + width + 2),
        (u8)(y + height + 1),
        1,
        1,
        0x10);

    BgCommitTilemapBufferToVram(printer->template.window->bgConfig, bg_id);
    subStruct->downArrowDelay = 8;
    subStruct->downArrowYPosIdx++;
}

static void TextPrinterClearDownArrow(TextPrinter *printer) {
    if (sTextFlags.useAlternateDownArrow) {
        return;
    }

    u8 bg_id  = GetWindowBgId(printer->template.window);
    u8 x      = GetWindowX(printer->template.window);
    u8 y      = GetWindowY(printer->template.window);
    u8 width  = GetWindowWidth(printer->template.window);
    u8 height = GetWindowHeight(printer->template.window) - 2;
    u16 r6    = unk00;

    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 10),
        (u8)(x + width + 1),
        (u8)(y + height),
        1,
        2,
        0x10);
    FillBgTilemapRect(printer->template.window->bgConfig,
        bg_id,
        (u16)(r6 + 11),
        (u8)(x + width + 2),
        (u8)(y + height),
        1,
        2,
        0x10);
    BgCommitTilemapBufferToVram(printer->template.window->bgConfig, bg_id);
}

static BOOL TextPrinterContinue(TextPrinter *printer) {
#pragma unused(printer)
    if (sub_02002220()) {
        PlaySE(SEQ_SE_DP_SELECT);

        sTextFlags.unk0_7 = 1;

        return TRUE;
    }

    return FALSE;
}

static BOOL TextPrinterWaitAutoMode(TextPrinter *printer) {
    TextPrinterSubStruct *subStruct = (TextPrinterSubStruct *)(&printer->subStructFields);

    if (subStruct->autoScrollDelay == 100) {
        return TRUE;
    }

    subStruct->autoScrollDelay++;
    if (sTextFlags.unk0_5) {
        return TextPrinterContinue(printer);
    }

    return FALSE;
}

static BOOL TextPrinterWaitWithDownArrow(TextPrinter *printer) {
    if (sTextFlags.autoScroll) {
        return TextPrinterWaitAutoMode(printer);
    }
    TextPrinterDrawDownArrow(printer);

    return TextPrinterContinue(printer);
}

static u8 TextPrinterWait(TextPrinter *printer) {
    if (sTextFlags.autoScroll) {
        return (u8)TextPrinterWaitAutoMode(printer);
    }

    return (u8)TextPrinterContinue(printer);
}

void TextFlags_SetCanABSpeedUpPrint(BOOL param0) {
    sTextFlags.canABSpeedUpPrint = param0;
}

void sub_02002B50(int param0) {
    sTextFlags.autoScroll = param0 & 1;
    sTextFlags.unk0_5     = (param0 >> 1) & 1;
}

void sub_02002B8C(BOOL enable) {
    sTextFlags.unk0_4 = enable;
}

void sub_02002BA8(BOOL enable) {
    sTextFlags.useAlternateDownArrow = enable;
}

u8 sub_02002BC4(void) {
    return sTextFlags.unk0_6;
}

void sub_02002BD4(void) {
    sTextFlags.unk0_6 = 0;
}

u8 sub_02002BE4(void) {
    return sTextFlags.unk0_7;
}

void sub_02002BF4(void) {
    sTextFlags.unk0_7 = 0;
}

BOOL sub_02002C04(void) {
    if (sTextFlags.unk0_4) {
        return sTextFlags.unk1_0;
    } else {
        return FALSE;
    }
}

void sub_02002C20(const TouchscreenHitbox *hitbox) {
    sTextFlags.unk1_1  = TRUE;
    sTouchScreenHitbox = *hitbox;
}

void sub_02002C40(void) {
    sTextFlags.unk1_1              = FALSE;
    sTouchScreenHitbox.rect.top    = 0;
    sTouchScreenHitbox.rect.bottom = 192;
    sTouchScreenHitbox.rect.left   = 0;
    sTouchScreenHitbox.rect.right  = 255;
}

void sub_02002C60(BOOL a0) {
    if (a0 == FALSE) {
        TextFlags_SetCanABSpeedUpPrint(TRUE);
        sub_02002B50(3);
        sub_02002B8C(TRUE);
    } else {
        sub_02002B50(1);
        TextFlags_SetCanABSpeedUpPrint(FALSE);
        sub_02002B8C(FALSE);
    }
}

void sub_02002C90(void) {
    TextFlags_SetCanABSpeedUpPrint(FALSE);
    sub_02002B50(0);
    sub_02002B8C(FALSE);
}
