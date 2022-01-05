#include "constants/scrcmd.h"
#include "fielddata/script/scr_seq/event_R26R0101.h"
#include "msgdata/msg/msg_0366_R26R0101.h"
	.include "asm/macros/script.inc"

	.rodata

	scrdef scr_seq_R26R0101_000
	scrdef scr_seq_R26R0101_001
	scrdef_end

scr_seq_R26R0101_000:
	play_se SEQ_SE_DP_SELECT
	lockall
	faceplayer
	npc_msg msg_0366_R26R0101_00000
	closemsg
	fade_screen 6, 1, 0, 0x0000
	wait_fade
	play_fanfare SEQ_ME_ASA
	wait_fanfare
	heal_party
	scrcmd_436
	scrcmd_150
	fade_screen 6, 1, 1, 0x0000
	wait_fade
	npc_msg msg_0366_R26R0101_00001
	waitbutton
	closemsg
	releaseall
	end

scr_seq_R26R0101_001:
	play_se SEQ_SE_DP_SELECT
	lockall
	faceplayer
	npc_msg msg_0366_R26R0101_00002
	waitbutton
	closemsg
	releaseall
	end
	.balign 4, 0
