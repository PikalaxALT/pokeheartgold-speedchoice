#include "constants/scrcmd.h"
#include "fielddata/script/scr_seq/event_R34R0101.h"
	.rodata
	.option alignment off

	.byte 3
	.short _EV_scr_seq_R34R0101_000 + 1, 0
	.byte 1
	.word scr_seq_R34R0101_map_scripts_2-.-4
	.byte 0

scr_seq_R34R0101_map_scripts_2:
	.short VAR_SCENE_DAYCARE_INTRO, 1, _EV_scr_seq_R34R0101_001 + 1
	.short 0

	.balign 4, 0
