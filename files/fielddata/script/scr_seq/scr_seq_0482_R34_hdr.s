#include "constants/scrcmd.h"
#include "fielddata/script/scr_seq/event_R34.h"
	.rodata
	.option alignment off

	.byte 2
	.short _EV_scr_seq_R34_000 + 1, 0
	.byte 1
	.word scr_seq_R34_map_scripts_2-.-4
	.byte 0

scr_seq_R34_map_scripts_2:
	.short VAR_SCENE_DAYCARE_INTRO, 2, _EV_scr_seq_R34_001 + 1
	.short 0

	.balign 4, 0
