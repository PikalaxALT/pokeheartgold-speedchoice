#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/sprites.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/std_script.h"
	.include "asm/macros.inc"
	.include "unk_0206A360.inc"
	.include "global.inc"

	.public NNS_G3dGlb

	.text

	thumb_func_start sub_0206A360
sub_0206A360: ; 0x0206A360
	push {r3, r4}
	ldr r4, _0206A384 ; =_0210FAE8
	mov r2, #0xff
	mov r3, #0
_0206A368:
	ldr r1, [r4]
	cmp r0, r1
	bne _0206A374
	lsl r0, r3, #0x18
	lsr r2, r0, #0x18
	b _0206A37C
_0206A374:
	add r3, r3, #1
	add r4, #0x3c
	cmp r3, #0x19
	blt _0206A368
_0206A37C:
	add r0, r2, #0
	pop {r3, r4}
	bx lr
	nop
_0206A384: .word _0210FAE8
	thumb_func_end sub_0206A360

	thumb_func_start sub_0206A388
sub_0206A388: ; 0x0206A388
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	str r0, [sp]
	add r6, r1, #0
	add r5, r2, #0
	add r7, r3, #0
	bl TaskManager_GetFieldSystem
	str r0, [sp, #4]
	mov r0, #4
	mov r1, #0x38
	bl AllocFromHeapAtEnd
	add r4, r0, #0
	mov r0, #0
	str r0, [r4]
	str r0, [r4, #4]
	ldr r0, [sp, #4]
	bl FieldSystem_GetBgConfigPtr
	str r0, [r4, #0xc]
	mov r0, #0
	strh r0, [r4, #0x18]
	strh r0, [r4, #0x1a]
	strh r6, [r4, #0x16]
	cmp r5, #4
	bhi _0206A3EA
	add r1, r5, r5
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0206A3CA: ; jump table
	.short _0206A3D4 - _0206A3CA - 2 ; case 0
	.short _0206A3D8 - _0206A3CA - 2 ; case 1
	.short _0206A3DE - _0206A3CA - 2 ; case 2
	.short _0206A3E4 - _0206A3CA - 2 ; case 3
	.short _0206A3E4 - _0206A3CA - 2 ; case 4
_0206A3D4:
	str r0, [r4, #0x2c]
	b _0206A3F2
_0206A3D8:
	mov r0, #1
	str r0, [r4, #0x2c]
	b _0206A3F2
_0206A3DE:
	mov r0, #2
	str r0, [r4, #0x2c]
	b _0206A3F2
_0206A3E4:
	mov r0, #3
	str r0, [r4, #0x2c]
	b _0206A3F2
_0206A3EA:
	bl GF_AssertFail
	mov r0, #0
	str r0, [r4, #0x2c]
_0206A3F2:
	mov r0, #4
	str r0, [r4, #8]
	str r7, [r4, #0x30]
	mov r0, #0
	strh r0, [r4, #0x34]
	strh r0, [r4, #0x36]
	ldr r0, [sp]
	ldr r1, _0206A40C ; =sub_0206A410
	add r2, r4, #0
	bl TaskManager_Call
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0206A40C: .word sub_0206A410
	thumb_func_end sub_0206A388

	thumb_func_start sub_0206A410
sub_0206A410: ; 0x0206A410
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r0, #0
	bl TaskManager_GetFieldSystem
	add r5, r0, #0
	add r0, r4, #0
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	ldr r0, [r4]
	cmp r0, #3
	bls _0206A42C
	b _0206A5BC
_0206A42C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0206A438: ; jump table
	.short _0206A440 - _0206A438 - 2 ; case 0
	.short _0206A4D0 - _0206A438 - 2 ; case 1
	.short _0206A4E0 - _0206A438 - 2 ; case 2
	.short _0206A50C - _0206A438 - 2 ; case 3
_0206A440:
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl GetBgColorMode
	strh r0, [r4, #0x14]
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl GetBgPriority
	str r0, [r4, #0x1c]
	ldr r0, [r4, #0xc]
	mov r1, #0
	bl GetBgPriority
	str r0, [r4, #0x20]
	ldr r0, [r4, #0xc]
	mov r1, #1
	bl GetBgPriority
	str r0, [r4, #0x24]
	ldr r0, [r4, #0xc]
	mov r1, #3
	bl GetBgPriority
	str r0, [r4, #0x28]
	ldr r0, [r4, #0xc]
	mov r1, #2
	mov r2, #0
	mov r3, #1
	bl SetBgControlParam
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #1
	bl SetBgPriority
	mov r0, #0
	mov r1, #2
	bl SetBgPriority
	add r0, r4, #0
	bl sub_0206A5D0
	add r0, r4, #0
	bl sub_0206A660
	mov r0, #0
	str r0, [sp]
	ldr r0, _0206A5C4 ; =0x04000050
	mov r1, #4
	mov r2, #0x21
	mov r3, #0x10
	bl G2x_SetBlendAlpha_
	mov r0, #0x10
	mov r1, #1
	str r0, [sp]
	str r1, [sp, #4]
	ldr r0, [r4, #8]
	add r2, r1, #0
	str r0, [sp, #8]
	mov r0, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _0206A5BC
_0206A4D0:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0206A5BC
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _0206A5BC
_0206A4E0:
	ldr r0, _0206A5C8 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _0206A4F6
	mov r0, #0
	strh r0, [r4, #0x18]
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _0206A5BC
_0206A4F6:
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
	cmp r0, #0x3c
	ble _0206A5BC
	mov r0, #0
	strh r0, [r4, #0x18]
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _0206A5BC
_0206A50C:
	ldrh r0, [r4, #0x18]
	cmp r0, #0x10
	bhs _0206A536
	ldrh r0, [r4, #0x1a]
	add r0, r0, #1
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0x1a]
	cmp r0, #2
	blo _0206A528
	mov r0, #0
	strh r0, [r4, #0x1a]
	ldrh r0, [r4, #0x18]
	add r0, r0, #1
	strh r0, [r4, #0x18]
_0206A528:
	ldrh r2, [r4, #0x18]
	mov r0, #0x10
	sub r1, r0, r2
	lsl r0, r2, #8
	orr r1, r0
	ldr r0, _0206A5CC ; =0x04000052
	strh r1, [r0]
_0206A536:
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0206A694
	cmp r0, #0
	beq _0206A5BC
	ldrh r0, [r4, #0x18]
	cmp r0, #0x10
	blo _0206A5BC
	mov r0, #2
	mov r1, #0
	bl ToggleBgLayer
	ldrh r3, [r4, #0x14]
	ldr r0, [r4, #0xc]
	mov r1, #2
	lsl r3, r3, #0x18
	mov r2, #0
	lsr r3, r3, #0x18
	bl SetBgControlParam
	ldr r1, [r4, #0x1c]
	mov r0, #2
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl SetBgPriority
	ldr r1, [r4, #0x20]
	mov r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl SetBgPriority
	ldr r1, [r4, #0x24]
	mov r0, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl SetBgPriority
	ldr r1, [r4, #0x28]
	mov r0, #3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl SetBgPriority
	ldr r0, _0206A5C4 ; =0x04000050
	mov r2, #0
	strh r2, [r0]
	ldr r3, [r4, #8]
	mov r0, #2
	mov r1, #0x20
	bl BG_ClearCharDataRange
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r4, #0x10]
	mov r1, #1
	bl WindowArray_Delete
	add r0, r4, #0
	bl FreeToHeap
	add sp, #0xc
	mov r0, #1
	pop {r4, r5, pc}
_0206A5BC:
	mov r0, #0
	add sp, #0xc
	pop {r4, r5, pc}
	nop
_0206A5C4: .word 0x04000050
_0206A5C8: .word gSystem
_0206A5CC: .word 0x04000052
	thumb_func_end sub_0206A410

	thumb_func_start sub_0206A5D0
sub_0206A5D0: ; 0x0206A5D0
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r0, #0
	mov r0, #0x16
	lsl r0, r0, #4
	str r0, [sp]
	ldr r0, [r4, #8]
	mov r1, #0x3c
	str r0, [sp, #4]
	ldrh r2, [r4, #0x16]
	ldr r5, _0206A654 ; =_0210FAE8 + 8
	ldr r3, [r4, #0x2c]
	mul r1, r2
	mov r2, #0xc
	mul r2, r3
	add r1, r5, r1
	ldr r1, [r2, r1]
	mov r2, #0
	mov r0, #0x96
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r4, #8]
	mov r1, #0x3c
	str r0, [sp, #0xc]
	ldrh r2, [r4, #0x16]
	ldr r5, _0206A658 ; =_0210FAE8 + 12
	ldr r3, [r4, #0x2c]
	mul r1, r2
	mov r2, #0xc
	mul r2, r3
	add r1, r5, r1
	ldr r1, [r2, r1]
	ldr r2, [r4, #0xc]
	mov r0, #0x96
	mov r3, #2
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [r4, #8]
	mov r1, #0x3c
	str r0, [sp, #0xc]
	ldrh r2, [r4, #0x16]
	ldr r5, _0206A65C ; =_0210FAE8 + 16
	ldr r3, [r4, #0x2c]
	mul r1, r2
	mov r2, #0xc
	mul r2, r3
	add r1, r5, r1
	ldr r1, [r2, r1]
	ldr r2, [r4, #0xc]
	mov r0, #0x96
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0206A654: .word _0210FAE8 + 8
_0206A658: .word _0210FAE8 + 12
_0206A65C: .word _0210FAE8 + 16
	thumb_func_end sub_0206A5D0

	thumb_func_start sub_0206A660
sub_0206A660: ; 0x0206A660
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	ldr r0, [r4, #8]
	mov r1, #1
	bl AllocWindows
	str r0, [r4, #0x10]
	mov r3, #0
	str r3, [sp]
	mov r0, #0x1c
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0xb
	lsl r0, r0, #6
	str r0, [sp, #0x10]
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #0x10]
	bl AddWindowParameterized
	add sp, #0x14
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end sub_0206A660

	thumb_func_start sub_0206A694
sub_0206A694: ; 0x0206A694
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r1, #0
	ldr r1, [r0, #0x30]
	cmp r1, #0
	bne _0206A6A6
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, r5, r6, pc}
_0206A6A6:
	add r4, r0, #0
	ldrh r0, [r0, #0x34]
	add r4, #0x34
	cmp r0, #5
	bhi _0206A790
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0206A6BC: ; jump table
	.short _0206A6C8 - _0206A6BC - 2 ; case 0
	.short _0206A6D8 - _0206A6BC - 2 ; case 1
	.short _0206A714 - _0206A6BC - 2 ; case 2
	.short _0206A72A - _0206A6BC - 2 ; case 3
	.short _0206A74A - _0206A6BC - 2 ; case 4
	.short _0206A78A - _0206A6BC - 2 ; case 5
_0206A6C8:
	ldr r0, [r5, #0x40]
	mov r1, #0
	bl PlayerAvatar_ToggleAutomaticHeightUpdating_NowApply
	ldrh r0, [r4]
	add r0, r0, #1
	strh r0, [r4]
	b _0206A790
_0206A6D8:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	ldrh r1, [r4, #2]
	add r6, r0, #0
	add r1, r1, #1
	strh r1, [r4, #2]
	add r1, sp, #0
	bl MapObject_GetPositionVec
	mov r0, #2
	ldr r1, [sp, #4]
	lsl r0, r0, #0xc
	sub r0, r1, r0
	str r0, [sp, #4]
	add r0, r6, #0
	add r1, sp, #0
	bl MapObject_SetPositionVec
	ldrh r0, [r4, #2]
	cmp r0, #0x10
	blo _0206A790
	ldr r0, [r5, #0x40]
	mov r1, #1
	bl PlayerAvatar_ToggleAutomaticHeightUpdating_NowApply
	ldrh r0, [r4]
	add r0, r0, #1
	strh r0, [r4]
	b _0206A790
_0206A714:
	ldr r0, [r5, #0x40]
	mov r1, #1
	bl Field_PlayerAvatar_OrrTransitionFlags
	ldr r0, [r5, #0x40]
	bl Field_PlayerAvatar_ApplyTransitionFlags
	ldrh r0, [r4]
	add r0, r0, #1
	strh r0, [r4]
	b _0206A790
_0206A72A:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	add r5, r0, #0
	bl MapObject_AreBitsSetForMovementScriptInit
	cmp r0, #0
	beq _0206A790
	add r0, r5, #0
	mov r1, #1
	bl MapObject_SetHeldMovement
	ldrh r0, [r4]
	add r0, r0, #1
	strh r0, [r4]
	b _0206A790
_0206A74A:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	bl MapObject_AreBitsSetForMovementScriptInit
	cmp r0, #0
	beq _0206A790
	add r0, r5, #0
	bl FollowingPokemon_IsActive
	cmp r0, #0
	beq _0206A782
	add r0, r5, #0
	mov r1, #1
	bl ov01_02205790
	add r0, r5, #0
	bl FollowingPokemon_GetMapObject
	mov r1, #0x30
	bl sub_0205FC94
	add r0, r5, #0
	bl FollowingPokemon_GetMapObject
	mov r1, #1
	bl sub_02069DC8
_0206A782:
	ldrh r0, [r4]
	add r0, r0, #1
	strh r0, [r4]
	b _0206A790
_0206A78A:
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, r5, r6, pc}
_0206A790:
	mov r0, #0
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end sub_0206A694

	.data

_0210FAE8:
	.byte 0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.byte 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.byte 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00
	.byte 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00
	.byte 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00
	.byte 0x19, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00

	.byte 0x43, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00
	.byte 0x1D, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
	.byte 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00
	.byte 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

	.byte 0xEB, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00
	.byte 0x1D, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
	.byte 0x1E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00
	.byte 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00

	.byte 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00
	.byte 0x26, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00
	.byte 0x27, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00
	.byte 0x2B, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

	.byte 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00
	.byte 0x2F, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00
	.byte 0x30, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00
	.byte 0x34, 0x00, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00

	.byte 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00
	.byte 0x38, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00
	.byte 0x3C, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00
	.byte 0x40, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00

	.byte 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x43, 0x00, 0x00, 0x00
	.byte 0x44, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00
	.byte 0x48, 0x00, 0x00, 0x00, 0x49, 0x00, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x4B, 0x00, 0x00, 0x00
	.byte 0x4C, 0x00, 0x00, 0x00, 0x4D, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00

	.byte 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00
	.byte 0x50, 0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x00
	.byte 0x54, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x56, 0x00, 0x00, 0x00, 0x57, 0x00, 0x00, 0x00
	.byte 0x58, 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00

	.byte 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00, 0x00, 0x5B, 0x00, 0x00, 0x00
	.byte 0x5C, 0x00, 0x00, 0x00, 0x5D, 0x00, 0x00, 0x00, 0x5E, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00
	.byte 0x60, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00
	.byte 0x64, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00

	.byte 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00
	.byte 0x68, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00
	.byte 0x69, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00
	.byte 0x6D, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00

	.byte 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00
	.byte 0x71, 0x00, 0x00, 0x00, 0x72, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00
	.byte 0x75, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00
	.byte 0x79, 0x00, 0x00, 0x00, 0x7A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00

	.byte 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00
	.byte 0x7D, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
	.byte 0x7E, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00
	.byte 0x82, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00

	.byte 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x85, 0x00, 0x00, 0x00
	.byte 0x86, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x89, 0x00, 0x00, 0x00
	.byte 0x87, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x89, 0x00, 0x00, 0x00, 0x8A, 0x00, 0x00, 0x00
	.byte 0x8B, 0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00

	.byte 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x00, 0x00, 0x00, 0x8E, 0x00, 0x00, 0x00
	.byte 0x8F, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00
	.byte 0x90, 0x00, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x93, 0x00, 0x00, 0x00
	.byte 0x94, 0x00, 0x00, 0x00, 0x95, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00

	.byte 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x97, 0x00, 0x00, 0x00
	.byte 0x98, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x00, 0x00, 0x9B, 0x00, 0x00, 0x00
	.byte 0x99, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x00, 0x00, 0x9B, 0x00, 0x00, 0x00, 0x9C, 0x00, 0x00, 0x00
	.byte 0x9D, 0x00, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00

	.byte 0xB3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x97, 0x00, 0x00, 0x00
	.byte 0x98, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x00, 0x00, 0x9B, 0x00, 0x00, 0x00
	.byte 0x99, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x00, 0x00, 0x9B, 0x00, 0x00, 0x00, 0x9C, 0x00, 0x00, 0x00
	.byte 0x9D, 0x00, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00

	.byte 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9F, 0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00
	.byte 0xA1, 0x00, 0x00, 0x00, 0xA2, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x00
	.byte 0xA5, 0x00, 0x00, 0x00, 0xA6, 0x00, 0x00, 0x00, 0xA7, 0x00, 0x00, 0x00, 0xA8, 0x00, 0x00, 0x00
	.byte 0xA9, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00

	.byte 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB, 0x00, 0x00, 0x00, 0xAC, 0x00, 0x00, 0x00
	.byte 0xAD, 0x00, 0x00, 0x00, 0xAE, 0x00, 0x00, 0x00, 0xAF, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00
	.byte 0xAE, 0x00, 0x00, 0x00, 0xAF, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 0xB1, 0x00, 0x00, 0x00
	.byte 0xB2, 0x00, 0x00, 0x00, 0xB3, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00

	.byte 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00, 0xB5, 0x00, 0x00, 0x00
	.byte 0xB6, 0x00, 0x00, 0x00, 0xB7, 0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x00, 0x00
	.byte 0xB7, 0x00, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x00, 0x00, 0xBA, 0x00, 0x00, 0x00
	.byte 0xBB, 0x00, 0x00, 0x00, 0xBC, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00

	.byte 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x00, 0x00, 0x00, 0xBE, 0x00, 0x00, 0x00
	.byte 0xBF, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xC1, 0x00, 0x00, 0x00, 0xC2, 0x00, 0x00, 0x00
	.byte 0xC3, 0x00, 0x00, 0x00, 0xC4, 0x00, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x00, 0xC6, 0x00, 0x00, 0x00
	.byte 0xC7, 0x00, 0x00, 0x00, 0xC8, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00

	.byte 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0x00, 0x00, 0x00, 0xCA, 0x00, 0x00, 0x00
	.byte 0xCB, 0x00, 0x00, 0x00, 0xCC, 0x00, 0x00, 0x00, 0xCD, 0x00, 0x00, 0x00, 0xCE, 0x00, 0x00, 0x00
	.byte 0xCC, 0x00, 0x00, 0x00, 0xCD, 0x00, 0x00, 0x00, 0xCE, 0x00, 0x00, 0x00, 0xCF, 0x00, 0x00, 0x00
	.byte 0xD0, 0x00, 0x00, 0x00, 0xD1, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00

	.byte 0x7A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD2, 0x00, 0x00, 0x00, 0xD3, 0x00, 0x00, 0x00
	.byte 0xD4, 0x00, 0x00, 0x00, 0xD5, 0x00, 0x00, 0x00, 0xD6, 0x00, 0x00, 0x00, 0xD7, 0x00, 0x00, 0x00
	.byte 0xD5, 0x00, 0x00, 0x00, 0xD6, 0x00, 0x00, 0x00, 0xD7, 0x00, 0x00, 0x00, 0xD8, 0x00, 0x00, 0x00
	.byte 0xD9, 0x00, 0x00, 0x00, 0xDA, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00

	.byte 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x00, 0x00, 0x00, 0xDC, 0x00, 0x00, 0x00
	.byte 0xDD, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xDF, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00
	.byte 0xDE, 0x00, 0x00, 0x00, 0xDF, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE1, 0x00, 0x00, 0x00
	.byte 0xE2, 0x00, 0x00, 0x00, 0xE3, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00
