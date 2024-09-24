#include "speedchoice_options.h"

#include "global.h"

#include "msgdata/msg/msg_speedchoice_options.h"

#include "bg_window.h"
#include "heap.h"
#include "msgdata.h"
#include "unk_0200CF18.h"

FS_EXTERN_OVERLAY(OVY_125);

#define HEAP_ID_SPEEDCHOICE_OPTIONS (HEAP_ID_3)

typedef struct SpeedchoiceOptions_AppData {
    HeapID heapId;
    SaveSpeedchoice *speedchoice;
    BgConfig *bgConfig;
    Window windows[6];
    u8 pageNo;
    u8 lineNo;
    u8 cursorSelections[SPEEDCHOICE_SETTINGS_MAX + 3];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *strbuf_unformatted;
    String *strbuf_formatted;
    SpriteRenderer *spriteRenderer;
    SpriteGfxHandler *spriteGfxHandler;
    Sprite *sprites[9];
} SpeedchoiceOptions_AppData;

typedef struct SpeedchoiceOptionLine {
    u8 name_gmm;
    u8 attr_id;
    u8 num_options;
    u8 options_gmm[4];
    u8 desc_gmm;
} SpeedchoiceOptionLine;

typedef struct SpeedchoiceOptionPage {
    SpeedchoiceOptionLine lines[6];
} SpeedchoiceOptionPage;

static void VBlankCB_SpeedchoiceOptionsApp(void *arg);
static void SpeedchoiceOptions_LoadSelections(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_SaveSelections(SpeedchoiceOptions_AppData *data);

const OVY_MGR_TEMPLATE gOverlayTemplate_SpeedchoiceOptions = {
    SpeedchoiceOptions_Init,
    SpeedchoiceOptions_Main,
    SpeedchoiceOptions_Exit,
    FS_OVERLAY_ID(OVY_125),
};

#define SPEEDCHOICE_PRESET       (SPEEDCHOICE_SETTINGS_MAX + 0)
#define SPEEDCHOICE_PLAYER_NAME  (SPEEDCHOICE_SETTINGS_MAX + 1)
#define SPEEDCHOICE_PLAYER_MODEL (SPEEDCHOICE_SETTINGS_MAX + 2)

#define SPEEDCHOICE_PRESET_VANILLA 0
#define SPEEDCHOICE_PRESET_BINGO   1

static const u8 sPresets[][SPEEDCHOICE_SETTINGS_MAX] = {
    [SPEEDCHOICE_PRESET_VANILLA] = {
                                    [SPEEDCHOICE_HOLD_TO_MASH]     = SPEEDCHOICE_HOLD_TO_MASH_NO,
                                    [SPEEDCHOICE_SPINNERS]         = SPEEDCHOICE_SPINNERS_KEEP,
                                    [SPEEDCHOICE_ROCKETLESS]       = SPEEDCHOICE_ROCKETLESS_KEEP,
                                    [SPEEDCHOICE_HEALTH_BARS]      = SPEEDCHOICE_HEALTH_BARS_VANILLA,
                                    [SPEEDCHOICE_BETTER_MARTS]     = SPEEDCHOICE_BETTER_MARTS_OFF,
                                    [SPEEDCHOICE_GOOD_EARLY_WILDS] = SPEEDCHOICE_GOOD_EARLY_WILDS_OFF,
                                    [SPEEDCHOICE_BIKE_MUSIC]       = SPEEDCHOICE_BIKE_MUSIC_OFF,
                                    [SPEEDCHOICE_SURF_MUSIC]       = SPEEDCHOICE_SURF_MUSIC_OFF,
                                    [SPEEDCHOICE_EVIL_HAU]         = SPEEDCHOICE_EVIL_HAU_OFF,
                                    [SPEEDCHOICE_FRIENDLESS]       = SPEEDCHOICE_FRIENDLESS_OFF,
                                    [SPEEDCHOICE_KIMONO_SEGMENTS]  = SPEEDCHOICE_KIMONO_SEGMENTS_KEEP,
                                    [SPEEDCHOICE_DEX_BEEPS]        = SPEEDCHOICE_DEX_BEEPS_OFF,
                                    [SPEEDCHOICE_HMS]              = SPEEDCHOICE_HMS_NORMAL,
                                    [SPEEDCHOICE_EXP]              = SPEEDCHOICE_EXP_NORMAL,
                                    [SPEEDCHOICE_EARLY_KANTO]      = SPEEDCHOICE_EARLY_KANTO_OFF,
                                    [SPEEDCHOICE_TRAINER_VISION]   = SPEEDCHOICE_TRAINER_VISION_NORMAL,
                                    [SPEEDCHOICE_GOAL]             = SPEEDCHOICE_GOAL_MANUAL,
                                    },
    [SPEEDCHOICE_PRESET_BINGO] = {

                                    [SPEEDCHOICE_HOLD_TO_MASH]     = SPEEDCHOICE_HOLD_TO_MASH_YES,
                                    [SPEEDCHOICE_SPINNERS]         = SPEEDCHOICE_SPINNERS_PURGE,
                                    [SPEEDCHOICE_ROCKETLESS]       = SPEEDCHOICE_ROCKETLESS_PURGE,
                                    [SPEEDCHOICE_HEALTH_BARS]      = SPEEDCHOICE_HEALTH_BARS_INSTANT,
                                    [SPEEDCHOICE_BETTER_MARTS]     = SPEEDCHOICE_BETTER_MARTS_ON,
                                    [SPEEDCHOICE_GOOD_EARLY_WILDS] = SPEEDCHOICE_GOOD_EARLY_WILDS_OFF,
                                    [SPEEDCHOICE_BIKE_MUSIC]       = SPEEDCHOICE_BIKE_MUSIC_ON,
                                    [SPEEDCHOICE_SURF_MUSIC]       = SPEEDCHOICE_SURF_MUSIC_ON,
                                    [SPEEDCHOICE_EVIL_HAU]         = SPEEDCHOICE_EVIL_HAU_OFF,
                                    [SPEEDCHOICE_FRIENDLESS]       = SPEEDCHOICE_FRIENDLESS_ON,
                                    [SPEEDCHOICE_KIMONO_SEGMENTS]  = SPEEDCHOICE_KIMONO_SEGMENTS_PURGE,
                                    [SPEEDCHOICE_DEX_BEEPS]        = SPEEDCHOICE_DEX_BEEPS_OFF,
                                    [SPEEDCHOICE_HMS]              = SPEEDCHOICE_HMS_NORMAL,
                                    [SPEEDCHOICE_EXP]              = SPEEDCHOICE_EXP_BW,
                                    [SPEEDCHOICE_EARLY_KANTO]      = SPEEDCHOICE_EARLY_KANTO_ON,
                                    [SPEEDCHOICE_TRAINER_VISION]   = SPEEDCHOICE_TRAINER_VISION_NORMAL,
                                    [SPEEDCHOICE_GOAL]             = SPEEDCHOICE_GOAL_MANUAL,
                                    },
};

static const SpeedchoiceOptionPage sPages[] = {
    {
     .lines = {
            {
                .name_gmm    = msg_speedchoice_options_preset,
                .attr_id     = SPEEDCHOICE_PRESET,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_preset_vanilla,
                    msg_speedchoice_options_preset_bingo,
                },
                .desc_gmm = msg_speedchoice_options_preset_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_name,
                .attr_id     = SPEEDCHOICE_PLAYER_NAME,
                .num_options = 0,
                .options_gmm = {},
                .desc_gmm    = msg_speedchoice_options_name_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_model,
                .attr_id     = SPEEDCHOICE_PLAYER_MODEL,
                .num_options = 0,
                .options_gmm = {},
                .desc_gmm    = msg_speedchoice_options_model_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_hold_to_mash,
                .attr_id     = SPEEDCHOICE_HOLD_TO_MASH,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_hold_to_mash_no,
                    msg_speedchoice_options_hold_to_mash_yes,
                },
                .desc_gmm = msg_speedchoice_options_hold_to_mash_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_spinners,
                .attr_id     = SPEEDCHOICE_SPINNERS,
                .num_options = 4,
                .options_gmm = {
                    msg_speedchoice_options_spinners_purge,
                    msg_speedchoice_options_spinners_keep,
                    msg_speedchoice_options_spinners_hell,
                    msg_speedchoice_options_spinners_why,
                },
                .desc_gmm = msg_speedchoice_options_spinners_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_rocketless,
                .attr_id     = SPEEDCHOICE_ROCKETLESS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_rocketless_keep,
                    msg_speedchoice_options_rocketless_purge,
                },
                .desc_gmm = msg_speedchoice_options_rocketless_desc,
            },
        },
     },
    {
     .lines = {
            {
                .name_gmm    = msg_speedchoice_options_better_marts,
                .attr_id     = SPEEDCHOICE_BETTER_MARTS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_better_marts_off,
                    msg_speedchoice_options_better_marts_on,
                },
                .desc_gmm = msg_speedchoice_options_better_marts_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_good_early_wilds,
                .attr_id     = SPEEDCHOICE_GOOD_EARLY_WILDS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_good_early_wilds_off,
                    msg_speedchoice_options_good_early_wilds_on,
                },
                .desc_gmm = msg_speedchoice_options_good_early_wilds_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_kimono_segments,
                .attr_id     = SPEEDCHOICE_KIMONO_SEGMENTS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_kimono_segments_keep,
                    msg_speedchoice_options_kimono_segments_purge,
                },
                .desc_gmm = msg_speedchoice_options_kimono_segments_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_friendless,
                .attr_id     = SPEEDCHOICE_FRIENDLESS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_friendless_keep,
                    msg_speedchoice_options_friendless_purge,
                },
                .desc_gmm = msg_speedchoice_options_friendless_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_evil_hau,
                .attr_id     = SPEEDCHOICE_EVIL_HAU,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_evil_hau_no,
                    msg_speedchoice_options_evil_hau_yes,
                },
                .desc_gmm = msg_speedchoice_options_evil_hau_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_health_bars,
                .attr_id     = SPEEDCHOICE_HEALTH_BARS,
                .num_options = 3,
                .options_gmm = {
                    msg_speedchoice_options_health_bars_vanilla,
                    msg_speedchoice_options_health_bars_fast,
                    msg_speedchoice_options_health_bars_instant,
                },
                .desc_gmm = msg_speedchoice_options_health_bars_desc,
            },
        },
     },
    {
     .lines = {
            {
                .name_gmm    = msg_speedchoice_options_goal,
                .attr_id     = SPEEDCHOICE_GOAL,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_goal_lance,
                    msg_speedchoice_options_goal_red,
                    msg_speedchoice_options_goal_manual,
                },
                .desc_gmm = msg_speedchoice_options_goal_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_early_kanto,
                .attr_id     = SPEEDCHOICE_EARLY_KANTO,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_early_kanto_off,
                    msg_speedchoice_options_early_kanto_on,
                },
                .desc_gmm = msg_speedchoice_options_early_kanto_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_exp,
                .attr_id     = SPEEDCHOICE_EXP,
                .options_gmm = {
                    msg_speedchoice_options_exp_vanilla,
                    msg_speedchoice_options_exp_bw,
                    msg_speedchoice_options_exp_none,
                },
                .desc_gmm = msg_speedchoice_options_exp_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_bike_music,
                .attr_id     = SPEEDCHOICE_BIKE_MUSIC,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_bike_music_off,
                    msg_speedchoice_options_bike_music_on,
                },
                .desc_gmm = msg_speedchoice_options_bike_music_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_surf_music,
                .attr_id     = SPEEDCHOICE_SURF_MUSIC,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_surf_music_off,
                    msg_speedchoice_options_surf_music_on,
                },
                .desc_gmm = msg_speedchoice_options_surf_music_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_dex_beeps,
                .attr_id     = SPEEDCHOICE_DEX_BEEPS,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_dex_beeps_off,
                    msg_speedchoice_options_dex_beeps_on,
                },
                .desc_gmm = msg_speedchoice_options_dex_beeps_desc,
            },
        },
     },
    {
     .lines = {
            {
                .name_gmm    = msg_speedchoice_options_hms,
                .attr_id     = SPEEDCHOICE_HMS,
                .num_options = 3,
                .options_gmm = {
                    msg_speedchoice_options_hms_vanilla,
                    msg_speedchoice_options_hms_nerf,
                    msg_speedchoice_options_hms_random,
                },
                .desc_gmm = msg_speedchoice_options_hms_desc,
            },
            {
                .name_gmm    = msg_speedchoice_options_trainer_vision,
                .attr_id     = SPEEDCHOICE_TRAINER_VISION,
                .num_options = 2,
                .options_gmm = {
                    msg_speedchoice_options_trainer_vision_normal,
                    msg_speedchoice_options_trainer_vision_max,
                },
                .desc_gmm = msg_speedchoice_options_trainer_vision_desc,
            },
        },
     },
};

BOOL SpeedchoiceOptions_Init(OVY_MANAGER *mgr, int *pState) {
    SpeedchoiceOptions_Args *args = OverlayManager_GetArgs(mgr);
    CreateHeap(HEAP_ID_3, HEAP_ID_OPTIONS_APP, 0x30000);

    SpeedchoiceOptions_AppData *data = OverlayManager_CreateAndGetData(mgr, sizeof(SpeedchoiceOptions_AppData), HEAP_ID_OPTIONS_APP);
    memset(data, 0, sizeof(SpeedchoiceOptions_AppData));
    data->heapId      = HEAP_ID_OPTIONS_APP;
    data->speedchoice = args->speedchoice;
    SpeedchoiceOptions_LoadSelections(data);
    return TRUE;
}

BOOL SpeedchoiceOptions_Main(OVY_MANAGER *mgr, int *pState) {
    return TRUE;
}

BOOL SpeedchoiceOptions_Exit(OVY_MANAGER *mgr, int *pState) {
    SpeedchoiceOptions_AppData *data = OverlayManager_GetData(mgr);
    SpeedchoiceOptions_SaveSelections(data);
    OverlayManager_FreeData(mgr);
    DestroyHeap(HEAP_ID_OPTIONS_APP);
    return TRUE;
}

static void VBlankCB_SpeedchoiceOptionsApp(void *arg) {
    SpeedchoiceOptions_AppData *data = arg;

    thunk_OamManager_ApplyAndResetBuffers();
    NNS_GfdDoVramTransfer();
    DoScheduledBgGpuUpdates(data->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_VBLANK);
}

static void SpeedchoiceOptions_LoadSelections(SpeedchoiceOptions_AppData *data) {
    for (int i = 0; i < SPEEDCHOICE_SETTINGS_MAX; ++i) {
        data->cursorSelections[i] = Speedchoice_GetAttr(data->speedchoice, i);
    }
}

static void SpeedchoiceOptions_SaveSelections(SpeedchoiceOptions_AppData *data) {
    for (int i = 0; i < SPEEDCHOICE_SETTINGS_MAX; ++i) {
        Speedchoice_SetAttr(data->speedchoice, i, data->cursorSelections[i]);
    }
}
