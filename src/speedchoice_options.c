#include "speedchoice_options.h"

#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_speedchoice_options.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "launch_application.h"
#include "msgdata.h"
#include "save_speedchoice.h"
#include "speedchoice_cv.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0200CF18.h"
#include "unk_0200FA24.h"
#include "unk_02082908.h"
#include "yes_no_prompt.h"

#define min(a, b) ((a) <= (b) ? (a) : (b))

FS_EXTERN_OVERLAY(OVY_125);

#define LINES_PER_PAGE 6
#define NUM_PAGES      ((NELEMS(sOptions) + LINES_PER_PAGE - 1) / LINES_PER_PAGE)

typedef enum SpeedchoiceOptionsInputResponse {
    SPEEDCHOICE_INPUT_CALL_APP,
    SPEEDCHOICE_INPUT_CURSOR_LEFT,
    SPEEDCHOICE_INPUT_CURSOR_RIGHT,
    SPEEDCHOICE_INPUT_CURSOR_UP,
    SPEEDCHOICE_INPUT_CURSOR_DOWN,
    SPEEDCHOICE_INPUT_CONFIRM,
    SPEEDCHOICE_INPUT_CURSOR_TO_CONFIRM,

    SPEEDCHOICE_INPUT_NULL = -1,
} SpeedchoiceOptionsInputResponse;

typedef enum SpeedchoiceOptionsMainState {
    SPC_OPT_MAIN_STATE_FADE_IN,
    SPC_OPT_MAIN_STATE_WAIT_FADE_IN,
    SPC_OPT_MAIN_STATE_INPUT_LOOP,
    SPC_OPT_MAIN_STATE_PAGE_CHANGE,
    SPC_OPT_MAIN_STATE_CURSOR_MOVE,
    SPC_OPT_MAIN_STATE_OPT_SELECT,
    SPC_OPT_MAIN_STATE_FADE_OUT_TO_NAMING,
    SPC_OPT_MAIN_STATE_WAIT_FADE_OUT_TO_NAMING,
    SPC_OPT_MAIN_STATE_RUN_NAMING,
    SPC_OPT_MAIN_STATE_FADE_IN_FROM_NAMING,
    SPC_OPT_MAIN_STATE_CONFIRM,
    SPC_OPT_MAIN_STATE_CONFIRM_WAIT_YESNO,
    SPC_OPT_MAIN_STATE_FADE_OUT_CONFIRM,
    SPC_OPT_MAIN_STATE_WAIT_FADE_OUT_CONFIRM,
} SpeedchoiceOptionsMainState;

typedef struct SpeedchoiceOptions_AppData {
    HeapID heapId;
    SpeedchoiceOptions_Args *args;
    BgConfig *bgConfig;
    Window windows[15];
    Window confirmScreenWindow_Top;
    Window confirmScreenWindow_Bottom;
    NARC *narc;
    u8 pageNo;
    u8 cursorPos;
    s8 selectChangeDir;
    u8 cursorSelections[SPEEDCHOICE_SETTINGS_MAX + 3];
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *strbuf_unformatted;
    String *strbuf_formatted;
    SpriteRenderer *spriteRenderer;
    SpriteGfxHandler *spriteGfxHandler;
    Sprite *sprites[9];
    s16 work[4];
    YesNoPrompt *yesno;
    NamingScreenArgs *namingScreenArgs;
    OVY_MANAGER *namingScreenApp;
    SpeedchoiceOptionsMainState stateOnReturn;
} SpeedchoiceOptions_AppData;

typedef struct SpeedchoiceOptionLine {
    u8 name_gmm;
    u8 attr_id;
    u8 num_options;
    u8 options_gmm[4];
    u8 desc_gmm;
} SpeedchoiceOptionLine;

typedef struct SpeedchoiceOptionPage {
    SpeedchoiceOptionLine lines[LINES_PER_PAGE];
} SpeedchoiceOptionPage;

static void VBlankCB_SpeedchoiceOptionsApp(void *arg);
static void SpeedchoiceOptions_LoadSelections(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_SaveSelections(SpeedchoiceOptions_AppData *data);
static SpeedchoiceOptionsInputResponse SpeedchoiceOptions_HandleInput(SpeedchoiceOptions_AppData *data);
static BOOL SpeedchoiceOptions_TurnPage(SpeedchoiceOptions_AppData *data, int direction);
static BOOL SpeedchoiceOptions_ChangeOption(SpeedchoiceOptions_AppData *data, int direction);
static BOOL SpeedchoiceOptions_MoveCursor(SpeedchoiceOptions_AppData *data, int direction);
static BOOL SpeedchoiceOptions_MoveCursorEx(SpeedchoiceOptions_AppData *data, int newCursorPos);
static void SpeedchoiceOptions_MoveCursorSprite(SpeedchoiceOptions_AppData *data);
static BOOL SpeedchoiceOptions_CallApp(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_DrawPage(SpeedchoiceOptions_AppData *data, int pageNo, GFBgLayer layer);
static void SpeedchoiceOptions_SetupBgConfig(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_FreeBgConfig(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_SetupGraphicsData(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_PrintOptionEx(SpeedchoiceOptions_AppData *data, int optionNo, Window *window, int x, int y);
static void SpeedchoiceOptions_PrintOption(SpeedchoiceOptions_AppData *data, int optionNo);
static void SpeedchoiceOptions_PrintOptionName(SpeedchoiceOptions_AppData *data, int optionNo);
static void SpeedchoiceOptions_PrintOptionDesc(SpeedchoiceOptions_AppData *data, int optionNo);
static void SpeedchoiceOptions_PrintConfirm(SpeedchoiceOptions_AppData *data);
static String *SpeedchoiceOptions_GetCVasString(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_PrintVersionAndCVString(SpeedchoiceOptions_AppData *data);
static void SpeedchoiceOptions_DrawConfirmScreen(SpeedchoiceOptions_AppData *data);
static YesNoResponse SpeedchoiceOptions_HandleYesNo(SpeedchoiceOptions_AppData *data);

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
                                    [SPEEDCHOICE_RUNNING_SHOES]    = SPEEDCHOICE_RUNNING_SHOES_VANILLA,
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
                                    [SPEEDCHOICE_DEX_BEEPS]        = SPEEDCHOICE_DEX_BEEPS_ON,
                                    [SPEEDCHOICE_HMS]              = SPEEDCHOICE_HMS_NORMAL,
                                    [SPEEDCHOICE_EXP]              = SPEEDCHOICE_EXP_BW,
                                    [SPEEDCHOICE_EARLY_KANTO]      = SPEEDCHOICE_EARLY_KANTO_ON,
                                    [SPEEDCHOICE_TRAINER_VISION]   = SPEEDCHOICE_TRAINER_VISION_NORMAL,
                                    [SPEEDCHOICE_GOAL]             = SPEEDCHOICE_GOAL_MANUAL,
                                    [SPEEDCHOICE_RUNNING_SHOES]    = SPEEDCHOICE_RUNNING_SHOES_FROM_START,
                                    },
};

static const u8 sSpeedchoiceOptionsOrder[] = {
    [SPEEDCHOICE_PRESET]           = 0,
    [SPEEDCHOICE_PLAYER_NAME]      = 1,
    [SPEEDCHOICE_PLAYER_MODEL]     = 2,
    [SPEEDCHOICE_HOLD_TO_MASH]     = 3,
    [SPEEDCHOICE_SPINNERS]         = 4,
    [SPEEDCHOICE_ROCKETLESS]       = 5,
    [SPEEDCHOICE_BETTER_MARTS]     = 6,
    [SPEEDCHOICE_GOOD_EARLY_WILDS] = 7,
    [SPEEDCHOICE_KIMONO_SEGMENTS]  = 8,
    [SPEEDCHOICE_FRIENDLESS]       = 9,
    [SPEEDCHOICE_EVIL_HAU]         = 10,
    [SPEEDCHOICE_HEALTH_BARS]      = 11,
    [SPEEDCHOICE_GOAL]             = 12,
    [SPEEDCHOICE_EARLY_KANTO]      = 13,
    [SPEEDCHOICE_EXP]              = 14,
    [SPEEDCHOICE_BIKE_MUSIC]       = 15,
    [SPEEDCHOICE_SURF_MUSIC]       = 16,
    [SPEEDCHOICE_DEX_BEEPS]        = 17,
    [SPEEDCHOICE_HMS]              = 18,
    [SPEEDCHOICE_TRAINER_VISION]   = 19,
    [SPEEDCHOICE_RUNNING_SHOES]    = 20,
};

static const SpeedchoiceOptionLine sOptions[] = {
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
     .num_options = 1,
     .options_gmm = {
            msg_speedchoice_options_name_placeholder,
        },
     .desc_gmm = msg_speedchoice_options_name_desc,
     },
    {
     .name_gmm    = msg_speedchoice_options_model,
     .attr_id     = SPEEDCHOICE_PLAYER_MODEL,
     .num_options = 2,
     .options_gmm = {
            msg_speedchoice_options_model_ethan,
            msg_speedchoice_options_model_lyra,
        },
     .desc_gmm = msg_speedchoice_options_model_desc,
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
    {
     .name_gmm    = msg_speedchoice_options_running_shoes,
     .attr_id     = SPEEDCHOICE_RUNNING_SHOES,
     .num_options = 2,
     .options_gmm = {
            msg_speedchoice_options_running_shoes_vanilla,
            msg_speedchoice_options_running_shoes_from_start,
        },
     .desc_gmm = msg_speedchoice_options_running_shoes_desc,
     },
};

static const WindowTemplate sWindowTemplates[] = {
    {
     GF_BG_LYR_MAIN_1,
     1,
     1,
     10,
     2,
     15,
     1,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     3,
     10,
     2,
     15,
     21,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     5,
     10,
     2,
     15,
     41,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     7,
     10,
     2,
     15,
     61,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     9,
     10,
     2,
     15,
     81,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     11,
     10,
     2,
     15,
     101,
     },
    {
     GF_BG_LYR_MAIN_1,
     1,
     13,
     10,
     2,
     15,
     121,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     1,
     16,
     2,
     15,
     141,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     3,
     16,
     2,
     15,
     173,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     5,
     16,
     2,
     15,
     205,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     7,
     16,
     2,
     15,
     237,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     9,
     16,
     2,
     15,
     269,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     11,
     16,
     2,
     15,
     301,
     },
    {
     GF_BG_LYR_MAIN_1,
     13,
     13,
     16,
     2,
     15,
     333,
     },
    {
     GF_BG_LYR_MAIN_1,
     8,
     15,
     16,
     2,
     15,
     365,
     },
    {
     GF_BG_LYR_SUB_1,
     0,
     18,
     32,
     6,
     15,
     1,
     },
};

static const WindowTemplate sWindowTemplate_ConfirmScreen_Top = {
    GF_BG_LYR_SUB_0,
    0,
    0,
    32,
    24,
    15,
    1,
};

static const WindowTemplate sWindowTemplate_ConfirmScreen_Bottom = {
    GF_BG_LYR_MAIN_0,
    0,
    12,
    32,
    6,
    15,
    1,
};

static const TouchscreenHitbox sButtonHitboxes[] = {
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = {},
     },
    {
     .rect = { TOUCHSCREEN_RECTLIST_END },
     },
};

BOOL SpeedchoiceOptions_Init(OVY_MANAGER *mgr, int *pState) {
    SpeedchoiceOptions_Args *args = OverlayManager_GetArgs(mgr);
    CreateHeap(HEAP_ID_3, HEAP_ID_OPTIONS_APP, 0x30000);

    SpeedchoiceOptions_AppData *data = OverlayManager_CreateAndGetData(mgr, sizeof(SpeedchoiceOptions_AppData), HEAP_ID_OPTIONS_APP);
    memset(data, 0, sizeof(SpeedchoiceOptions_AppData));
    data->heapId           = HEAP_ID_OPTIONS_APP;
    data->args             = args;
    data->namingScreenApp  = NULL;
    data->namingScreenArgs = NamingScreen_CreateArgs(HEAP_ID_OAKS_SPEECH, NAME_SCREEN_PLAYER, 0, PLAYER_NAME_LENGTH, args->options, NULL);
    SpeedchoiceOptions_LoadSelections(data);
    SpeedchoiceOptions_SetupBgConfig(data);
    data->msgFormat = MessageFormat_New(data->heapId);
    data->msgData   = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_speedchoice_options_bin, data->heapId);
    return TRUE;
}

#define myState (*pState)
BOOL SpeedchoiceOptions_Main(OVY_MANAGER *mgr, int *pState) {
    SpeedchoiceOptions_AppData *data = OverlayManager_GetData(mgr);

    switch (myState) {
    case SPC_OPT_MAIN_STATE_FADE_IN:
        SpeedchoiceOptions_DrawPage(data, data->pageNo, GF_BG_LYR_MAIN_1);
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, data->heapId);
        myState             = SPC_OPT_MAIN_STATE_WAIT_FADE_IN;
        data->stateOnReturn = SPC_OPT_MAIN_STATE_INPUT_LOOP;
        break;
    case SPC_OPT_MAIN_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            myState = data->stateOnReturn;
        }
        break;
    case SPC_OPT_MAIN_STATE_INPUT_LOOP:
        switch (SpeedchoiceOptions_HandleInput(data)) {
        case SPEEDCHOICE_INPUT_CALL_APP:
            myState             = SPC_OPT_MAIN_STATE_FADE_OUT_TO_NAMING;
            data->stateOnReturn = SPC_OPT_MAIN_STATE_INPUT_LOOP;
            break;
        case SPEEDCHOICE_INPUT_CURSOR_LEFT:
            myState               = data->cursorPos == LINES_PER_PAGE ? SPC_OPT_MAIN_STATE_PAGE_CHANGE : SPC_OPT_MAIN_STATE_OPT_SELECT;
            data->selectChangeDir = -1;
            break;
        case SPEEDCHOICE_INPUT_CURSOR_RIGHT:
            myState               = data->cursorPos == LINES_PER_PAGE ? SPC_OPT_MAIN_STATE_PAGE_CHANGE : SPC_OPT_MAIN_STATE_OPT_SELECT;
            data->selectChangeDir = 1;
            break;
        case SPEEDCHOICE_INPUT_CURSOR_UP:
            myState               = SPC_OPT_MAIN_STATE_CURSOR_MOVE;
            data->selectChangeDir = -1;
            break;
        case SPEEDCHOICE_INPUT_CURSOR_DOWN:
            myState               = SPC_OPT_MAIN_STATE_CURSOR_MOVE;
            data->selectChangeDir = 1;
            break;
        case SPEEDCHOICE_INPUT_CONFIRM:
            if (String_GetLength(data->namingScreenArgs->nameInputString) == 0) {
                myState             = SPC_OPT_MAIN_STATE_FADE_OUT_TO_NAMING;
                data->stateOnReturn = SPC_OPT_MAIN_STATE_CONFIRM_WAIT_YESNO;
            } else {
                myState = SPC_OPT_MAIN_STATE_CONFIRM;
            }
            break;
        case SPEEDCHOICE_INPUT_CURSOR_TO_CONFIRM:
            SpeedchoiceOptions_MoveCursorEx(data, LINES_PER_PAGE + 1);
            break;
        }
        break;
    case SPC_OPT_MAIN_STATE_PAGE_CHANGE:
        SpeedchoiceOptions_TurnPage(data, data->selectChangeDir);
        myState = SPC_OPT_MAIN_STATE_INPUT_LOOP;
        break;
    case SPC_OPT_MAIN_STATE_CURSOR_MOVE:
        SpeedchoiceOptions_MoveCursor(data, data->selectChangeDir);
        myState = SPC_OPT_MAIN_STATE_INPUT_LOOP;
        break;
    case SPC_OPT_MAIN_STATE_OPT_SELECT:
        SpeedchoiceOptions_ChangeOption(data, data->selectChangeDir);
        myState = SPC_OPT_MAIN_STATE_INPUT_LOOP;
        break;
    case SPC_OPT_MAIN_STATE_FADE_OUT_TO_NAMING:
        String_SetEmpty(data->namingScreenArgs->nameInputString);
        data->namingScreenArgs->playerGender = data->cursorSelections[SPEEDCHOICE_PLAYER_MODEL];
        data->namingScreenApp                = OverlayManager_New(&sOverlayTemplate_NamingScreen, data->namingScreenArgs, data->heapId);
        myState                              = SPC_OPT_MAIN_STATE_FADE_IN_FROM_NAMING;
        break;
    case SPC_OPT_MAIN_STATE_FADE_IN_FROM_NAMING:
        if (data->stateOnReturn == SPC_OPT_MAIN_STATE_CONFIRM_WAIT_YESNO) {
            SpeedchoiceOptions_DrawConfirmScreen(data);
        } else {
            SpeedchoiceOptions_DrawPage(data, data->pageNo, GF_BG_LYR_MAIN_1);
        }
        BeginNormalPaletteFade(0, 1, 1, RGB_BLACK, 6, 1, data->heapId);
        myState = SPC_OPT_MAIN_STATE_WAIT_FADE_IN;
        break;
    case SPC_OPT_MAIN_STATE_CONFIRM:
        SpeedchoiceOptions_DrawConfirmScreen(data);
        myState = SPC_OPT_MAIN_STATE_CONFIRM_WAIT_YESNO;
        break;
    case SPC_OPT_MAIN_STATE_CONFIRM_WAIT_YESNO:
        switch (SpeedchoiceOptions_HandleYesNo(data)) {
        case YESNORESPONSE_YES:
            myState = SPC_OPT_MAIN_STATE_FADE_OUT_CONFIRM;
            break;
        case YESNORESPONSE_NO:
            SpeedchoiceOptions_DrawPage(data, data->pageNo, GF_BG_LYR_MAIN_1);
            myState = SPC_OPT_MAIN_STATE_INPUT_LOOP;
            break;
        }
        break;
    case SPC_OPT_MAIN_STATE_FADE_OUT_CONFIRM:
        BeginNormalPaletteFade(4, 0, 0, RGB_BLACK, 6, 1, data->heapId);
        myState = SPC_OPT_MAIN_STATE_WAIT_FADE_OUT_CONFIRM;
        break;
    case SPC_OPT_MAIN_STATE_WAIT_FADE_OUT_CONFIRM:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }

    return FALSE;
}
#undef myState

BOOL SpeedchoiceOptions_Exit(OVY_MANAGER *mgr, int *pState) {
    SpeedchoiceOptions_AppData *data = OverlayManager_GetData(mgr);
    SpeedchoiceOptions_SaveSelections(data);
    SpeedchoiceOptions_FreeBgConfig(data);
    DestroyMsgData(data->msgData);
    MessageFormat_Delete(data->msgFormat);
    NamingScreen_DeleteArgs(data->namingScreenArgs);
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
        data->cursorSelections[i] = Speedchoice_GetAttr(data->args->speedchoice, i);
    }

    const u16 *playerName = PlayerProfile_GetNamePtr(data->args->playerProfile);
    CopyU16ArrayToStringN(data->namingScreenArgs->nameInputString, playerName, PLAYER_NAME_LENGTH + 1);
    data->cursorSelections[SPEEDCHOICE_PLAYER_NAME]  = 0;
    data->cursorSelections[SPEEDCHOICE_PLAYER_MODEL] = PlayerProfile_GetTrainerGender(data->args->playerProfile);
    data->cursorSelections[SPEEDCHOICE_PRESET]       = SPEEDCHOICE_PRESET_VANILLA;
}

static void SpeedchoiceOptions_SaveSelections(SpeedchoiceOptions_AppData *data) {
    for (int i = 0; i < SPEEDCHOICE_SETTINGS_MAX; ++i) {
        Speedchoice_SetAttr(data->args->speedchoice, i, data->cursorSelections[i]);
    }
}

static SpeedchoiceOptionsInputResponse SpeedchoiceOptions_HandleInput(SpeedchoiceOptions_AppData *data) {
    const SpeedchoiceOptionLine *page = &sOptions[data->pageNo];
    int optionNum                     = LINES_PER_PAGE * data->pageNo + data->cursorPos;
    if (gSystem.touchNew) {
        int hitbox = TouchscreenHitbox_FindHitboxAtTouchNew(sButtonHitboxes);
        switch (hitbox) {
        case 0:
        case 2:
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
            data->cursorPos = hitbox / 2;
            return SPEEDCHOICE_INPUT_CURSOR_LEFT;
        case 1:
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
            data->cursorPos = hitbox / 2;
            return SPEEDCHOICE_INPUT_CURSOR_RIGHT;
        case 14:
            return SPEEDCHOICE_INPUT_CONFIRM;
        case 15:
            if (data->pageNo == 0) {
                data->cursorPos = 1;
                return SPEEDCHOICE_INPUT_CALL_APP;
            }
            break;
        }
        return SPEEDCHOICE_INPUT_NULL;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        if (data->pageNo == 0 && data->cursorPos == 1) {
            return SPEEDCHOICE_INPUT_CALL_APP;
        }
    } else if (gSystem.newKeys & PAD_BUTTON_START) {
        return SPEEDCHOICE_INPUT_CURSOR_TO_CONFIRM;
    } else if (gSystem.newKeys & PAD_KEY_DOWN) {
        return SPEEDCHOICE_INPUT_CURSOR_DOWN;
    } else if (gSystem.newKeys & PAD_KEY_UP) {
        return SPEEDCHOICE_INPUT_CURSOR_UP;
    } else if (gSystem.newKeys & PAD_KEY_LEFT) {
        return SPEEDCHOICE_INPUT_CURSOR_LEFT;
    } else if (gSystem.newKeys & PAD_KEY_RIGHT) {
        return SPEEDCHOICE_INPUT_CURSOR_RIGHT;
    }
    return SPEEDCHOICE_INPUT_NULL;
}

static BOOL SpeedchoiceOptions_TurnPage(SpeedchoiceOptions_AppData *data, int direction) {
    switch (data->work[0]) {
    case 0:
        PlaySE(SEQ_SE_PL_SYU03);
        data->pageNo = min((u32)(data->pageNo + direction), NUM_PAGES - 1);
        SpeedchoiceOptions_DrawPage(data, data->pageNo, GF_BG_LYR_MAIN_2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        data->work[1] = direction == 1 ? 0 : 32;
        BgSetPosTextAndCommit(data->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, data->work[1] * 8);
        ++data->work[0];
        break;
    case 1:
        data->work[1] += direction;
        BgSetPosTextAndCommit(data->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, data->work[1] * 8);
        if (data->work[1] == (direction == 1 ? 32 : 0)) {
            ++data->work[0];
        }
        break;
    case 2:
        PlaySE(SEQ_SE_PL_KIRAKIRA);
        SpeedchoiceOptions_DrawPage(data, data->pageNo, GF_BG_LYR_MAIN_1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        return TRUE;
    }

    return FALSE;
}

static BOOL SpeedchoiceOptions_ChangeOption(SpeedchoiceOptions_AppData *data, int direction) {
    if (data->cursorPos >= LINES_PER_PAGE) {
        return FALSE;
    }
    int optionNo  = data->pageNo * LINES_PER_PAGE + data->cursorPos;
    int oldChoice = data->cursorSelections[sOptions[optionNo].attr_id];
    int newChoice = oldChoice + direction;
    if (newChoice < 0) {
        newChoice = sOptions[optionNo].num_options - 1;
    } else if (newChoice >= sOptions[optionNo].num_options) {
        newChoice = 0;
    }
    if (newChoice != oldChoice) {
        PlaySE(SEQ_SE_DP_SELECT);
        data->cursorSelections[sOptions[optionNo].attr_id] = newChoice;
        SpeedchoiceOptions_PrintOption(data, optionNo);
        return TRUE;
    }

    return FALSE;
}

static BOOL SpeedchoiceOptions_MoveCursor(SpeedchoiceOptions_AppData *data, int direction) {
    int newCursorPos = data->cursorPos + direction;
    if (newCursorPos < 0) {
        newCursorPos = LINES_PER_PAGE + 1;
    } else if (newCursorPos >= LINES_PER_PAGE + 2) {
        newCursorPos = 0;
    }
    return SpeedchoiceOptions_MoveCursorEx(data, newCursorPos);
}

static BOOL SpeedchoiceOptions_MoveCursorEx(SpeedchoiceOptions_AppData *data, int newCursorPos) {
    data->cursorPos = newCursorPos;
    PlaySE(SEQ_SE_DP_SELECT);
    SpeedchoiceOptions_MoveCursorSprite(data);
    return TRUE;
}

static BOOL SpeedchoiceOptions_CallApp(SpeedchoiceOptions_AppData *data) {
    return sOptions[data->pageNo * LINES_PER_PAGE + data->cursorPos].attr_id == SPEEDCHOICE_PLAYER_NAME;
}

static void SpeedchoiceOptions_DrawPage(SpeedchoiceOptions_AppData *data, int pageNo, GFBgLayer layer) {
    for (int i = 0; i < 15; ++i) {
        data->windows[i].bgId = layer;
    }
    for (int i = pageNo * LINES_PER_PAGE; i < (pageNo + 1) * LINES_PER_PAGE && i < NELEMS(sOptions); ++i) {
        SpeedchoiceOptions_PrintOptionName(data, i);
        SpeedchoiceOptions_PrintOption(data, i);
    }
    SpeedchoiceOptions_PrintConfirm(data);
}

static const u32 sOptionsAppBgLayers[5] = {
    GF_BG_LYR_MAIN_0,
    GF_BG_LYR_MAIN_1,
    GF_BG_LYR_MAIN_2,
    GF_BG_LYR_SUB_0,
    GF_BG_LYR_SUB_1,
};

static void SpeedchoiceOptions_SetupBgConfig(SpeedchoiceOptions_AppData *data) {
    data->bgConfig = BgConfig_Alloc(data->heapId);

    static const GraphicsModes graphicsModes = {
        GX_DISPMODE_GRAPHICS,
        GX_BGMODE_0,
        GX_BGMODE_0,
        GX_BG0_AS_2D,
    };
    SetBothScreensModesAndDisable(&graphicsModes);

    BgTemplate templates[5] = {
        {
         .x          = 0,
         .y          = 0,
         .bufferSize = 0x800,
         .baseTile   = 0,
         .size       = GF_BG_SCR_SIZE_256x256,
         .colorMode  = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf800,
         .charBase   = GX_BG_CHARBASE_0x00000,
         .bgExtPltt  = GX_BG_EXTPLTT_01,
         .priority   = 0,
         .areaOver   = GX_BG_AREAOVER_XLU,
         .mosaic     = FALSE,
         },
        {
         .x          = 0,
         .y          = 0,
         .bufferSize = 0x800,
         .baseTile   = 0,
         .size       = GF_BG_SCR_SIZE_256x256,
         .colorMode  = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf000,
         .charBase   = GX_BG_CHARBASE_0x04000,
         .bgExtPltt  = GX_BG_EXTPLTT_01,
         .priority   = 1,
         .areaOver   = GX_BG_AREAOVER_XLU,
         .mosaic     = FALSE,
         },
        {
         .x          = 0,
         .y          = 0,
         .bufferSize = 0x800,
         .baseTile   = 0,
         .size       = GF_BG_SCR_SIZE_256x256,
         .colorMode  = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xe800,
         .charBase   = GX_BG_CHARBASE_0x00000,
         .bgExtPltt  = GX_BG_EXTPLTT_01,
         .priority   = 2,
         .areaOver   = GX_BG_AREAOVER_XLU,
         .mosaic     = FALSE,
         },
        {
         .x          = 0,
         .y          = 0,
         .bufferSize = 0x800,
         .baseTile   = 0,
         .size       = GF_BG_SCR_SIZE_256x256,
         .colorMode  = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf800,
         .charBase   = GX_BG_CHARBASE_0x00000,
         .bgExtPltt  = GX_BG_EXTPLTT_01,
         .priority   = 1,
         .areaOver   = GX_BG_AREAOVER_XLU,
         .mosaic     = FALSE,
         },
        {
         .x          = 0,
         .y          = 0,
         .bufferSize = 0x800,
         .baseTile   = 0,
         .size       = GF_BG_SCR_SIZE_256x256,
         .colorMode  = GX_BG_COLORMODE_16,
         .screenBase = GX_BG_SCRBASE_0xf000,
         .charBase   = GX_BG_CHARBASE_0x08000,
         .bgExtPltt  = GX_BG_EXTPLTT_01,
         .priority   = 0,
         .areaOver   = GX_BG_AREAOVER_XLU,
         .mosaic     = FALSE,
         },
    };

    for (int i = 0; i < 5; i++) {
        InitBgFromTemplate(data->bgConfig, sOptionsAppBgLayers[i], &templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(data->bgConfig, sOptionsAppBgLayers[i]);
    }

    BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, data->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, data->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, data->heapId);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, data->heapId);
}

static void SpeedchoiceOptions_FreeBgConfig(SpeedchoiceOptions_AppData *data) {
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_MAIN_1);
    FreeBgTilemapBuffer(data->bgConfig, GF_BG_LYR_MAIN_0);
    FreeToHeap(data->bgConfig);
}

static void SpeedchoiceOptions_SetupGraphicsData(SpeedchoiceOptions_AppData *data) {
    GfGfxLoader_GXLoadPal(NARC_a_0_7_2, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, data->heapId);
    GfGfxLoader_LoadCharData(NARC_a_0_7_2, 8, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_2, 19, data->bgConfig, GF_BG_LYR_SUB_0, 0, 0, FALSE, data->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_0_7_2, 2, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, data->heapId);
    GfGfxLoader_LoadCharData(NARC_a_0_7_2, 7, data->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_2, 17, data->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, FALSE, data->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_0_7_2, 18, data->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, FALSE, data->heapId);
}

static void SpeedchoiceOptions_PrintOptionEx(SpeedchoiceOptions_AppData *data, int optionNo, Window *window, int x, int y) {
    GF_ASSERT(optionNo < NELEMS(sOptions));
    int choice = data->cursorSelections[optionNo];
    GF_ASSERT(choice < sOptions[optionNo].num_options);

    if (sOptions[optionNo].attr_id == SPEEDCHOICE_PLAYER_NAME) {
        if (String_GetLength(data->namingScreenArgs->nameInputString) != 0) {
            String_Copy(data->strbuf_formatted, data->namingScreenArgs->nameInputString);
        } else {
            ReadMsgDataIntoString(data->msgData, msg_speedchoice_options_player_name_not_set, data->strbuf_formatted);
        }
    } else {
        ReadMsgDataIntoString(data->msgData, sOptions[optionNo].options_gmm[choice], data->strbuf_formatted);
    }
    AddTextPrinterParameterized(window, 0, data->strbuf_formatted, x, y, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(window);
}

static void SpeedchoiceOptions_PrintOption(SpeedchoiceOptions_AppData *data, int optionNo) {
    int rowNum = optionNo % LINES_PER_PAGE;
    u32 x      = (sWindowTemplates[rowNum + 7].width * 8 - FontID_String_GetWidth(0, data->strbuf_formatted, 0)) / 2;
    SpeedchoiceOptions_PrintOptionEx(data, optionNo, &data->windows[rowNum + 7], x, 0);
}

static void SpeedchoiceOptions_PrintOptionName(SpeedchoiceOptions_AppData *data, int optionNo) {
    GF_ASSERT(optionNo < NELEMS(sOptions));

    int rowNum = optionNo % LINES_PER_PAGE;
    ReadMsgDataIntoString(data->msgData, sOptions[optionNo].name_gmm, data->strbuf_formatted);
    u32 x = (sWindowTemplates[rowNum + 7].width * 8 - FontID_String_GetWidth(0, data->strbuf_formatted, 0)) / 2;
    AddTextPrinterParameterized(&data->windows[rowNum], 0, data->strbuf_formatted, x, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(&data->windows[rowNum]);
}

static void SpeedchoiceOptions_PrintOptionDesc(SpeedchoiceOptions_AppData *data, int optionNo) {
    GF_ASSERT(optionNo < NELEMS(sOptions));

    ReadMsgDataIntoString(data->msgData, sOptions[optionNo].name_gmm, data->strbuf_formatted);
    AddTextPrinterParameterized(&data->windows[15], 0, data->strbuf_formatted, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(&data->windows[15]);
}

static void SpeedchoiceOptions_PrintConfirm(SpeedchoiceOptions_AppData *data) {
    ReadMsgDataIntoString(data->msgData, msg_speedchoice_options_confirm, data->strbuf_formatted);
    AddTextPrinterParameterized(&data->windows[14], 0, data->strbuf_formatted, 0, 0, TEXT_SPEED_NOTRANSFER, NULL);
    ScheduleWindowCopyToVram(&data->windows[14]);
}

static void SpeedchoiceOptions_MoveCursorSprite(SpeedchoiceOptions_AppData *data) {
}

static String *SpeedchoiceOptions_GetCVasString(SpeedchoiceOptions_AppData *data) {
    String *ret = String_New(2 * sizeof(SaveSpeedchoice) + 1, data->heapId);
    GF_ASSERT(ret != NULL);

    u32 cv[sizeof(SaveSpeedchoice) / 4];
    u32 *ssc = (u32 *)data->args->speedchoice;
    u32 *pCV = (u32 *)gRandoCV;
    u16 buf[2 * sizeof(SaveSpeedchoice) + 1];
    int i;
    int nybble;

    for (i = 0; i < sizeof(SaveSpeedchoice) / 4; ++i) {
        cv[i] = pCV[i] ^ ssc[i];
    }

    for (i = 0; i < sizeof(SaveSpeedchoice) * 2; ++i) {
        nybble = (cv[i / 8] >> (i * 4)) & 15;
        buf[i] = CHAR_JP_0 + nybble;
    }
    buf[2 * sizeof(SaveSpeedchoice)] = EOS;
    CopyU16ArrayToString(ret, buf);
    return ret;
}

static const u8 sKeySettings[] = {
    SPEEDCHOICE_PLAYER_NAME,
    SPEEDCHOICE_ROCKETLESS,
    SPEEDCHOICE_SPINNERS,
    SPEEDCHOICE_TRAINER_VISION,
    SPEEDCHOICE_HMS,
    // SPEEDCHOICE_ENCOUNTER_SLOTS,
    SPEEDCHOICE_EXP,
    // SPEEDCHOICE_TIN_TOWER,
    SPEEDCHOICE_BETTER_MARTS,
    SPEEDCHOICE_GOOD_EARLY_WILDS,
    SPEEDCHOICE_EARLY_KANTO,
};

static void SpeedchoiceOptions_PrintVersionAndCVString(SpeedchoiceOptions_AppData *data) {
    String *cvString = SpeedchoiceOptions_GetCVasString(data);
    BufferString(data->msgFormat, 0, cvString, 0, 0, 0);
    String *speedchoiceVersion = ConvertAsciiToPmString(gSpeedchoiceVersion, data->heapId);
    BufferString(data->msgFormat, 1, speedchoiceVersion, 0, 0, 0);
#if defined(HEARTGOLD)
    ReadMsgDataIntoString(data->msgData, msg_speedchoice_options_CV_HG, data->strbuf_unformatted);
#elif defined(SOULSILVER)
    ReadMsgDataIntoString(data->msgData, msg_speedchoice_options_CV_SS, data->strbuf_unformatted);
#else
#error Invalid version
#endif
    StringExpandPlaceholders(data->msgFormat, data->strbuf_formatted, data->strbuf_unformatted);
    AddTextPrinterParameterized(&data->confirmScreenWindow_Top, 0, data->strbuf_formatted, 8, 8 + 12 * NELEMS(sKeySettings), TEXT_SPEED_NOTRANSFER, NULL);
    String_Delete(cvString);
    String_Delete(speedchoiceVersion);
}

static void SpeedchoiceOptions_PrintKeySettings(SpeedchoiceOptions_AppData *data) {
    String *string = String_New(64, HEAP_ID_OPTIONS_APP);

    ReadMsgDataIntoString(data->msgData, msg_speedchoice_options_confirm_screen_setting_ph, data->strbuf_unformatted);
    for (int i = 0; i < NELEMS(sKeySettings); ++i) {
        int setting                       = sKeySettings[i];
        const SpeedchoiceOptionLine *line = &sOptions[sSpeedchoiceOptionsOrder[setting]];
        ReadMsgDataIntoString(data->msgData, line->name_gmm, string);
        BufferString(data->msgFormat, 0, string, 0, 0, 0);
        if (setting == SPEEDCHOICE_PLAYER_NAME) {
            BufferString(data->msgFormat, 1, data->namingScreenArgs->nameInputString, 0, 0, 0);
        } else {
            ReadMsgDataIntoString(data->msgData, line->options_gmm[data->cursorSelections[setting]], string);
            BufferString(data->msgFormat, 1, string, 0, 0, 0);
        }
        StringExpandPlaceholders(data->msgFormat, data->strbuf_formatted, data->strbuf_unformatted);
        AddTextPrinterParameterized(&data->confirmScreenWindow_Top, 0, data->strbuf_formatted, 8, 8 + 12 * i, TEXT_SPEED_NOTRANSFER, NULL);
    }

    String_Delete(string);
}

static void SpeedchoiceOptions_DrawConfirmScreen(SpeedchoiceOptions_AppData *data) {
    AddWindow(data->bgConfig, &data->confirmScreenWindow_Top, &sWindowTemplate_ConfirmScreen_Top);
    AddWindow(data->bgConfig, &data->confirmScreenWindow_Bottom, &sWindowTemplate_ConfirmScreen_Bottom);
    FillWindowPixelBuffer(&data->confirmScreenWindow_Top, 15);
    FillWindowPixelBuffer(&data->confirmScreenWindow_Bottom, 15);
    SpeedchoiceOptions_PrintVersionAndCVString(data);
    SpeedchoiceOptions_PrintKeySettings(data);
}

static YesNoResponse SpeedchoiceOptions_HandleYesNo(SpeedchoiceOptions_AppData *data) {
    return YesNoPrompt_HandleInput(data->yesno);
}
