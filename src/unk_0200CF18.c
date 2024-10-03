#include "unk_0200CF18.h"

#include "global.h"

#include "assert.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200ACF0.h"
#include "unk_0200B150.h"
#include "unk_02020654.h"

static void SpriteGfxHandler_DeleteSpriteList(SpriteGfxHandler *gfxHandler);
static void SpriteGfxHandler_DeleteResourceHeaderList(SpriteGfxHandler *gfxHandler);
static void SpriteGfxHandler_DestroyResObjsAndMans(SpriteGfxHandler *gfxHandler);
static void DeinitSpriteRenderer(SpriteRenderer *renderer);
static void MyRemoveSpriteGfxHandler(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler);
static BOOL sub_0200D124(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const u16 *a2, int a3, int a4);
static Sprite *MyCreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, int headerIndex, s16 x, s16 y, s16 z, u16 animSeqNo, int rotation, int palIndex, NNS_G2D_VRAM_TYPE whichScreen, int a10, int a11, int a12, int a13);
static UnkImageStruct *MyLoadResourcesAndCreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const UnkTemplate_0200D748 *unkTemplate, fx32 yOffset);
static BOOL MyLoadCellOrAnim_NarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, GfGfxResType a6, int resId);
static BOOL MyLoadCellOrAnim_OpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, GfGfxResType a6, int resId);
static BOOL MyInsertResObjIntoList(GF_2DGfxResObjList *list, GF_2DGfxResObj *obj);
static BOOL MyUnloadCellOrAnimById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 cellOrAnimId);
static BOOL MyUnloadCharById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 charId);
static BOOL MyUnloadPlttById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 plttId);

SpriteRenderer *SpriteRenderer_Create(HeapID heapId) {
    SpriteRenderer *ret = AllocFromHeap(heapId, sizeof(SpriteRenderer));
    if (ret == NULL) {
        return NULL;
    }
    ret->heapId         = heapId;
    ret->numGfxHandlers = 0;
    ret->hasOamManager  = TRUE;
    return ret;
}

SpriteGfxHandler *SpriteRenderer_CreateGfxHandler(SpriteRenderer *renderer) {
    GF_ASSERT(renderer != NULL);
    SpriteGfxHandler *ret = AllocFromHeap(renderer->heapId, sizeof(SpriteGfxHandler));
    if (ret == NULL) {
        return NULL;
    }
    ++renderer->numGfxHandlers;
    for (int i = 0; i < GF_GFX_RES_TYPE_MAX; ++i) {
        ret->_2dGfxResMan[i] = NULL;
    }
    return ret;
}

GF_G2dRenderer *SpriteRenderer_GetG2dRendererPtr(SpriteRenderer *renderer) {
    return &renderer->renderer;
}

BOOL SpriteRenderer_CreateOamCharPlttManagers(SpriteRenderer *renderer, const OamManagerParam *oamManagerParam, const OamCharTransferParam *oamTransferParam, int numPltts) {
    GF_ASSERT(renderer != NULL);
    if (renderer == NULL) {
        return FALSE;
    }
    ObjCharTransferTemplate transferTemplate;
    transferTemplate.maxTasks = oamTransferParam->maxTasks;
    transferTemplate.sizeMain = oamTransferParam->sizeMain;
    transferTemplate.sizeSub  = oamTransferParam->sizeSub;
    transferTemplate.heapId   = renderer->heapId;
    ObjCharTransfer_InitEx(&transferTemplate, oamTransferParam->charModeMain, oamTransferParam->charModeSub);
    ObjPlttTransfer_Init(numPltts, renderer->heapId);
    NNS_G2dInitOamManagerModule();
    if (renderer->hasOamManager == TRUE) {
        OamManager_Create(oamManagerParam->fromOBJmain, oamManagerParam->numOBJmain, oamManagerParam->fromAffineMain, oamManagerParam->numAffineMain, oamManagerParam->fromOBJsub, oamManagerParam->numOBJsub, oamManagerParam->fromAffineSub, oamManagerParam->numAffineSub, renderer->heapId);
    }
    renderer->cellTransferState = sub_02020654(0x20, renderer->heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    return TRUE;
}

BOOL SpriteRenderer_CreateSpriteList(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, int numSprites) {
    if (renderer == NULL || gfxHandler == NULL) {
        return FALSE;
    }
    gfxHandler->spriteList = G2dRenderer_Init(numSprites, &renderer->renderer, renderer->heapId);
    return TRUE;
}

void thunk_Sprite_Delete(Sprite *sprite) {
    Sprite_Delete(sprite);
}

void SpriteGfxHandler_RenderAndAnimateSprites(SpriteGfxHandler *gfxHandler) {
    GF_ASSERT(gfxHandler != NULL);
    SpriteList_RenderAndAnimateSprites(gfxHandler->spriteList);
}

void thunk_OamManager_ApplyAndResetBuffers(void) {
    OamManager_ApplyAndResetBuffers();
}

void SpriteRenderer_thunk_UpdateCellTransferStateManager(void) {
    thunk_UpdateCellTransferStateManager();
}

static void SpriteGfxHandler_DeleteSpriteList(SpriteGfxHandler *gfxHandler) {
    SpriteList_Delete(gfxHandler->spriteList);
}

static void SpriteGfxHandler_DeleteResourceHeaderList(SpriteGfxHandler *gfxHandler) {
    if (gfxHandler->spriteHeaderList != NULL) {
        SpriteResourceHeaderList_Destroy(gfxHandler->spriteHeaderList);
    }
}

static void SpriteGfxHandler_DestroyResObjsAndMans(SpriteGfxHandler *gfxHandler) {
    for (int i = 0; i < gfxHandler->numGfxResObjectTypes; ++i) {
        GF2DGfxResHeader_Reset(GF2DGfxResHeader_GetByIndex(gfxHandler->_2dGfxResHeader, i));
    }
    FreeToHeap(gfxHandler->_2dGfxResHeader);
    sub_0200AED4(gfxHandler->_2dGfxResObjList[0]);
    sub_0200B0CC(gfxHandler->_2dGfxResObjList[1]);
    for (int i = 0; i < gfxHandler->numGfxResObjectTypes; ++i) {
        Delete2DGfxResObjList(gfxHandler->_2dGfxResObjList[i]);
        Destroy2DGfxResObjMan(gfxHandler->_2dGfxResMan[i]);
    }
}

static void DeinitSpriteRenderer(SpriteRenderer *renderer) {
    sub_0202067C(renderer->cellTransferState);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    if (renderer->hasOamManager == TRUE) {
        OamManager_Free();
    }
}

static void MyRemoveSpriteGfxHandler(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler) {
    --renderer->numGfxHandlers;
    FreeToHeap(gfxHandler);
}

void SpriteRenderer_RemoveGfxHandler(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler) {
    SpriteGfxHandler_DeleteSpriteList(gfxHandler);
    SpriteGfxHandler_DeleteResourceHeaderList(gfxHandler);
    SpriteGfxHandler_DestroyResObjsAndMans(gfxHandler);
    MyRemoveSpriteGfxHandler(renderer, gfxHandler);
}

void SpriteRenderer_Delete(SpriteRenderer *renderer) {
    GF_ASSERT(renderer->numGfxHandlers == 0);
    DeinitSpriteRenderer(renderer);
    FreeToHeap(renderer);
}

static BOOL sub_0200D124(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const u16 *fileIdList, int loadCharMode, int loadPlttMode) {
    int i;
    int numGfxResTypes;
    int size;
    GF_2DGfxResHeader *header;
    void *data;
    NARC *narc;

    numGfxResTypes = GF_GFX_RES_TYPE_MAX;

    if (renderer == NULL || gfxHandler == NULL) {
        return FALSE;
    }
    if (fileIdList[GF_GFX_RES_TYPE_MCEL] == 0xFFFF) {
        numGfxResTypes = GF_GFX_RES_TYPE_MAX - 2;
    }
    gfxHandler->numGfxResObjectTypes = numGfxResTypes;
    size                             = GF2DGfxResHeader_sizeof();
    gfxHandler->_2dGfxResHeader      = AllocFromHeap(renderer->heapId, size * numGfxResTypes);
    narc                             = NARC_New(NARC_data_resdat, renderer->heapId);

    for (i = 0; i < numGfxResTypes; ++i) {
        header = GF2DGfxResHeader_GetByIndex(gfxHandler->_2dGfxResHeader, i);
        data   = GfGfxLoader_LoadFromOpenNarc(narc, fileIdList[i], FALSE, renderer->heapId, TRUE);
        GF2DGfxResHeader_Init((GF_2DGfxResHeaderNarcList *)data, header, renderer->heapId);
        FreeToHeap(data);
    }
    for (i = 0; i < numGfxResTypes; ++i) {
        header                      = GF2DGfxResHeader_GetByIndex(gfxHandler->_2dGfxResHeader, i);
        size                        = GF2dGfxResHeader_GetNumObjects(header);
        gfxHandler->_2dGfxResMan[i] = Create2DGfxResObjMan(size, (GfGfxResType)i, renderer->heapId);
    }
    for (i = 0; i < numGfxResTypes; ++i) {
        header                          = GF2DGfxResHeader_GetByIndex(gfxHandler->_2dGfxResHeader, i);
        size                            = GF2dGfxResHeader_GetNumObjects(header);
        gfxHandler->_2dGfxResObjList[i] = Create2DGfxResObjList(size, renderer->heapId);
        gfxHandler->numGfxResObjects[i] = LoadAll2DGfxResObjsFromHeader(gfxHandler->_2dGfxResMan[i], header, gfxHandler->_2dGfxResObjList[i], renderer->heapId);
    }
    switch (loadCharMode) {
    case 0:
        sub_0200ADE4(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR]);
        break;
    case 1:
        sub_0200AE58(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR]);
        break;
    case 2:
    default:
        sub_0200AD30(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR]);
        break;
    }
    switch (loadPlttMode) {
    case 0:
        sub_0200B050(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT]);
        break;
    case 1:
    default:
        sub_0200AFD8(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT]);
        break;
    }
    data                         = GfGfxLoader_LoadFromOpenNarc(narc, fileIdList[6], FALSE, renderer->heapId, TRUE);
    gfxHandler->spriteHeaderList = SpriteResourceHeaderList_Create(
        (struct ResdatNarcEntry *)data,
        renderer->heapId,
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CELL],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_ANIM],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MCEL],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MANM]);
    FreeToHeap(data);
    NARC_Delete(narc);
    return TRUE;
}

BOOL sub_0200D294(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const u16 *fileIdList) {
    return sub_0200D124(renderer, gfxHandler, fileIdList, 2, 1);
}

BOOL sub_0200D2A4(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const u16 *fileIdList, int loadCharMode, int loadPlttMode) {
    return sub_0200D124(renderer, gfxHandler, fileIdList, loadCharMode, loadPlttMode);
}

Sprite *SpriteRenderer_CreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const UnkStruct_0200D2B4 *template) {
    return MyCreateSprite(renderer, gfxHandler, template->resourceSet, template->x, template->y, template->x /* typo? */, template->animSeqNo, template->rotation, template->palIndex, template->whichScreen, template->unk_18, template->unk_1C, template->unk_20, template->unk_24);
}

static Sprite *MyCreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, int headerIndex, s16 x, s16 y, s16 z, u16 animSeqNo, int priority, int palIndex, NNS_G2D_VRAM_TYPE whichScreen, int a10, int a11, int a12, int a13) {
    Sprite *ret = NULL;
    SpriteTemplate template;

    template.spriteList = gfxHandler->spriteList;
    template.header     = &gfxHandler->spriteHeaderList->headers[headerIndex];

    template.position.x = FX32_CONST(x);
    template.position.y = FX32_CONST(y);
    template.position.z = FX32_CONST(z);

    if (whichScreen == NNS_G2D_VRAM_TYPE_2DSUB) {
        template.position.y += FX32_CONST(GX_LCD_SIZE_Y);
    }
    template.scale.x     = FX32_ONE;
    template.scale.y     = FX32_ONE;
    template.scale.z     = FX32_ONE;
    template.rotation    = 0;
    template.priority    = priority;
    template.whichScreen = whichScreen;
    template.heapId      = renderer->heapId;
    ret                  = Sprite_CreateAffine(&template);
    if (ret != NULL) {
        Sprite_SetAnimCtrlSeq(ret, animSeqNo);
        switch (a10) {
        case 0:
            Sprite_SetPalIndex(ret, palIndex);
            break;
        case 1:
            break;
        case 2:
            Sprite_SetPalIndexRespectVramOffset(ret, palIndex);
            break;
        }
    }
    return ret;
}

BOOL SpriteRenderer_Init2DGfxResManagersFromCountsArray(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, SpriteResourceCountsListUnion *countsArray) {
    int i, j;
    int numGfxResTypes = GF_GFX_RES_TYPE_MAX;
    int num;

    if (renderer == NULL || gfxHandler == NULL) {
        return FALSE;
    }
    if (countsArray->numMcel == 0 || countsArray->numManm == 0) {
        numGfxResTypes = GF_GFX_RES_TYPE_MAX - 2;
    }
    gfxHandler->numGfxResObjectTypes = numGfxResTypes;

    for (i = 0; i < numGfxResTypes; ++i) {
        gfxHandler->_2dGfxResMan[i] = Create2DGfxResObjMan(countsArray->asArray[i], (GfGfxResType)i, renderer->heapId);
    }

    for (i = 0; i < numGfxResTypes; ++i) {
        num = countsArray->asArray[i];
        if (num == 0) {
            continue;
        }
        gfxHandler->_2dGfxResObjList[i] = Create2DGfxResObjList(num, renderer->heapId);
        gfxHandler->numGfxResObjects[i] = 0;
        for (j = 0; j < gfxHandler->_2dGfxResObjList[i]->max; ++j) {
            gfxHandler->_2dGfxResObjList[i]->obj[j] = NULL;
        }
    }
    return TRUE;
}

BOOL SpriteRenderer_LoadCharResObjFromNarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *obj = AddCharResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], narcId, fileId, compressed, resId, vram, renderer->heapId);
    if (obj != NULL) {
        sub_0200ADA4(obj);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], obj);
        return TRUE;
    }
    GF_ASSERT(0);
    return obj != NULL;
}

BOOL SpriteRenderer_LoadCharResObjFromOpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *obj = AddCharResObjFromOpenNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], narc, fileId, compressed, resId, vram, renderer->heapId);
    if (obj != NULL) {
        sub_0200ADA4(obj);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], obj);
        return TRUE;
    }
    GF_ASSERT(0);
    return obj != NULL;
}

s8 SpriteRenderer_LoadPlttResObjFromNarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int pltt_num, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], resId)) {
        return -1;
    }
    GF_2DGfxResObj *obj = AddPlttResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], narcId, fileId, compressed, resId, vram, pltt_num, renderer->heapId);
    if (obj != NULL) {
        GF_ASSERT(sub_0200B00C(obj) == TRUE);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT], obj);
        return GF_PlttResObj_GetVramSlotNum(obj, vram);
    }
    GF_ASSERT(0);
    return -1;
}

s8 SpriteRenderer_LoadPlttResObjFromOpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int pltt_num, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], resId)) {
        return -1;
    }
    GF_2DGfxResObj *obj = AddPlttResObjFromOpenNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], narc, fileId, compressed, resId, vram, pltt_num, renderer->heapId);
    if (obj != NULL) {
        GF_ASSERT(sub_0200B00C(obj) == TRUE);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT], obj);
        return GF_PlttResObj_GetVramSlotNum(obj, vram);
    }
    GF_ASSERT(0);
    return -1;
}

u8 sub_0200D644(PaletteData *plttData, PaletteBufferId bufferId, SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int pltt_num, NNS_G2D_VRAM_TYPE vram, int resId) {
    int ret = SpriteRenderer_LoadPlttResObjFromNarcId(renderer, gfxHandler, narcId, fileId, compressed, pltt_num, vram, resId);
    if (ret != -1) {
        PaletteData_LoadPaletteSlotFromHardware(plttData, bufferId, ret * 16, pltt_num * 32);
    }
    return ret;
}

u8 sub_0200D68C(PaletteData *plttData, PaletteBufferId bufferId, SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int pltt_num, NNS_G2D_VRAM_TYPE vram, int resId) {
    int ret = SpriteRenderer_LoadPlttResObjFromOpenNarc(renderer, gfxHandler, narc, fileId, compressed, pltt_num, vram, resId);
    if (ret != -1) {
        PaletteData_LoadPaletteSlotFromHardware(plttData, bufferId, ret * 16, pltt_num * 32);
    }
    return ret;
}

BOOL SpriteRenderer_LoadCellResObjFromNarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int resId) {
    return MyLoadCellOrAnim_NarcId(renderer, gfxHandler, narcId, fileId, compressed, GF_GFX_RES_TYPE_CELL, resId);
}

BOOL SpriteRenderer_LoadCellResObjFromOpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int resId) {
    return MyLoadCellOrAnim_OpenNarc(renderer, gfxHandler, narc, fileId, compressed, GF_GFX_RES_TYPE_CELL, resId);
}

BOOL SpriteRenderer_LoadAnimResObjFromNarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int resId) {
    return MyLoadCellOrAnim_NarcId(renderer, gfxHandler, narcId, fileId, compressed, GF_GFX_RES_TYPE_ANIM, resId);
}

BOOL SpriteRenderer_LoadAnimResObjFromOpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int resId) {
    return MyLoadCellOrAnim_OpenNarc(renderer, gfxHandler, narc, fileId, compressed, GF_GFX_RES_TYPE_ANIM, resId);
}

UnkImageStruct *SpriteRenderer_LoadResourcesAndCreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const UnkTemplate_0200D748 *template) {
    return MyLoadResourcesAndCreateSprite(renderer, gfxHandler, template, FX32_CONST(GX_LCD_SIZE_Y));
}

UnkImageStruct *SpriteRenderer_LoadResourcesAndCreateSprite_CustomBottomScreenOffset(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const UnkTemplate_0200D748 *template, fx32 yOffset) {
    return MyLoadResourcesAndCreateSprite(renderer, gfxHandler, template, yOffset);
}

static UnkImageStruct *MyLoadResourcesAndCreateSprite(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, const UnkTemplate_0200D748 *unkTemplate, fx32 yOffset) {
    int i;
    int paletteOffset;
    UnkImageStruct *ret = AllocFromHeap(renderer->heapId, sizeof(UnkImageStruct));
    SpriteTemplate spriteTemplate;
    int resIdList[GF_GFX_RES_TYPE_MAX];

    if (ret == NULL) {
        return NULL;
    }
    ret->spriteResourceHeaderList = AllocFromHeap(renderer->heapId, sizeof(SpriteResourceHeaderList));
    if (ret->spriteResourceHeaderList == NULL) {
        return NULL;
    }
    ret->spriteResourceHeaderList->headers = AllocFromHeap(renderer->heapId, sizeof(SpriteResourcesHeader));
    ret->spriteResourcesHeader             = ret->spriteResourceHeaderList->headers;
    if (ret->spriteResourceHeaderList->headers == NULL) {
        if (ret->spriteResourceHeaderList != NULL) { // always true
            FreeToHeap(ret->spriteResourceHeaderList);
        }
        return NULL; // leaks 16 bytes
    }
    for (i = 0; i < GF_GFX_RES_TYPE_MAX; ++i) {
        resIdList[i] = unkTemplate->resIdList[i];
    }
    if (gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MCEL] == NULL || gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MANM] == NULL) {
        resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        resIdList[GF_GFX_RES_TYPE_MANM] = -1;
    } else {
        if (resIdList[GF_GFX_RES_TYPE_MCEL] != -1 && !GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MCEL], resIdList[GF_GFX_RES_TYPE_MCEL])) {
            resIdList[GF_GFX_RES_TYPE_MCEL] = -1;
        }
        if (resIdList[GF_GFX_RES_TYPE_MANM] != -1 && !GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MANM], resIdList[GF_GFX_RES_TYPE_MANM])) {
            resIdList[GF_GFX_RES_TYPE_MANM] = -1;
        }
    }
    CreateSpriteResourcesHeader(
        ret->spriteResourcesHeader,
        resIdList[GF_GFX_RES_TYPE_CHAR],
        resIdList[GF_GFX_RES_TYPE_PLTT],
        resIdList[GF_GFX_RES_TYPE_CELL],
        resIdList[GF_GFX_RES_TYPE_ANIM],
        resIdList[GF_GFX_RES_TYPE_MCEL],
        resIdList[GF_GFX_RES_TYPE_MANM],
        unkTemplate->vramTransfer,
        unkTemplate->bgPriority,
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CELL],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_ANIM],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MCEL],
        gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_MANM]);

    spriteTemplate.spriteList = gfxHandler->spriteList;
    spriteTemplate.header     = ret->spriteResourcesHeader;
    spriteTemplate.position.x = FX32_CONST(unkTemplate->x);
    spriteTemplate.position.y = FX32_CONST(unkTemplate->y);
    spriteTemplate.position.z = FX32_CONST(unkTemplate->z);
    if (unkTemplate->vram == NNS_G2D_VRAM_TYPE_2DSUB) {
        spriteTemplate.position.y += yOffset;
    }
    spriteTemplate.scale.x     = FX32_ONE;
    spriteTemplate.scale.y     = FX32_ONE;
    spriteTemplate.scale.z     = FX32_ONE;
    spriteTemplate.rotation    = 0;
    spriteTemplate.priority    = unkTemplate->spritePriority;
    spriteTemplate.whichScreen = unkTemplate->vram;
    spriteTemplate.heapId      = renderer->heapId;
    ret->sprite                = Sprite_CreateAffine(&spriteTemplate);
    ret->vramTransfer          = unkTemplate->vramTransfer;
    if (ret->sprite != NULL) {
        Sprite_SetAnimCtrlSeq(ret->sprite, unkTemplate->animation);
        if (unkTemplate->pal != 0xFFFF) {
            paletteOffset = Sprite_GetPalIndex(ret->sprite);
            Sprite_SetPalIndex(ret->sprite, paletteOffset + unkTemplate->pal);
        }
    } else {
        GF_ASSERT(0);
    }
    return ret;
}

NNSG2dImagePaletteProxy *GfGfxHandler_GetPlttProxy(SpriteGfxHandler *gfxHandler, int id) {
    return GF_PlttResObj_GetPlttProxy(Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], id), NULL);
}

int GfGfxHandler_GetPlttNumById(SpriteGfxHandler *gfxHandler, int id, NNS_G2D_VRAM_TYPE vram) {
    return GF_PlttResObj_GetVramSlotNum(Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], id), vram);
}

BOOL SpriteGfxHandler_UnloadCharObjById(SpriteGfxHandler *gfxHandler, u32 character) {
    return MyUnloadCharById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], character);
}

BOOL SpriteGfxHandler_UnloadPlttObjById(SpriteGfxHandler *gfxHandler, u32 pal) {
    return MyUnloadPlttById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT], pal);
}

BOOL SpriteGfxHandler_UnloadCellObjById(SpriteGfxHandler *gfxHandler, u32 cell) {
    return MyUnloadCellOrAnimById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CELL], gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CELL], cell);
}

BOOL SpriteGfxHandler_UnloadAnimObjById(SpriteGfxHandler *gfxHandler, u32 animation) {
    return MyUnloadCellOrAnimById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_ANIM], gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_ANIM], animation);
}

void SpriteRenderer_UnloadResourcesAndRemoveGfxHandler(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler) {
    int i;

    SpriteGfxHandler_DeleteSpriteList(gfxHandler);
    sub_0200AED4(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR]);
    sub_0200B0CC(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_PLTT]);

    for (i = 0; i < gfxHandler->numGfxResObjectTypes; ++i) {
        Delete2DGfxResObjList(gfxHandler->_2dGfxResObjList[i]);
        Destroy2DGfxResObjMan(gfxHandler->_2dGfxResMan[i]);
    }
    MyRemoveSpriteGfxHandler(renderer, gfxHandler);
}

void UnkImageStruct_Delete(UnkImageStruct *unk) {
    if (unk->vramTransfer) {
        sub_0200AF80(unk->spriteResourcesHeader->imageProxy);
    }
    Sprite_Delete(unk->sprite);
    SpriteResourceHeaderList_Destroy(unk->spriteResourceHeaderList);
    FreeToHeap(unk);
}

static BOOL MyLoadCellOrAnim_NarcId(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, GfGfxResType a6, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[a6], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *data = AddCellOrAnimResObjFromNarc(gfxHandler->_2dGfxResMan[a6], narcId, fileId, compressed, resId, a6, renderer->heapId);
    if (data != NULL) {
        BOOL result = MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[a6], data);
        GF_ASSERT(result == TRUE);
        return result;
    }
    GF_ASSERT(0);
    return data != NULL;
}

static BOOL MyLoadCellOrAnim_OpenNarc(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, GfGfxResType a6, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[a6], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *data = AddCellOrAnimResObjFromOpenNarc(gfxHandler->_2dGfxResMan[a6], narc, fileId, compressed, resId, a6, renderer->heapId);
    if (data != NULL) {
        BOOL result = MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[a6], data);
        GF_ASSERT(result == TRUE);
        return result;
    }
    GF_ASSERT(0);
    return data != NULL;
}

static BOOL MyInsertResObjIntoList(GF_2DGfxResObjList *list, GF_2DGfxResObj *obj) {
    for (int i = 0; i < list->max; ++i) {
        if (list->obj[i] == NULL) {
            list->obj[i] = obj;
            ++list->num;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL MyUnloadCellOrAnimById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 cellOrAnimId) {
    for (int i = 0; i < list->max; ++i) {
        if (list->obj[i] != NULL) {
            u32 test_id = GF2DGfxResObj_GetResID(list->obj[i]);
            if (test_id == cellOrAnimId) {
                DestroySingle2DGfxResObj(manager, list->obj[i]);
                list->obj[i] = NULL;
                --list->num;
                return TRUE;
            }
        }
    }
    return FALSE;
}

static BOOL MyUnloadCharById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 charId) {
    for (int i = 0; i < list->max; ++i) {
        if (list->obj[i] != NULL) {
            u32 test_id = GF2DGfxResObj_GetResID(list->obj[i]);
            if (test_id == charId) {
                ObjCharTransfer_ResetTransferTasksByResID(charId);
                DestroySingle2DGfxResObj(manager, list->obj[i]);
                list->obj[i] = NULL;
                --list->num;
                return TRUE;
            }
        }
    }
    return FALSE;
}

static BOOL MyUnloadPlttById(GF_2DGfxResMan *manager, GF_2DGfxResObjList *list, u32 plttId) {
    for (int i = 0; i < list->max; ++i) {
        if (list->obj[i] != NULL) {
            u32 test_id = GF2DGfxResObj_GetResID(list->obj[i]);
            if (test_id == plttId) {
                ObjPlttTransfer_FreeTaskByID(plttId);
                DestroySingle2DGfxResObj(manager, list->obj[i]);
                list->obj[i] = NULL;
                --list->num;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void TickSpriteAnimation1Frame(Sprite *sprite) {
    Sprite_TickAnimCtrlFrame(sprite, FX32_ONE);
}

void UnkImageStruct_TickSpriteAnimation1Frame(UnkImageStruct *unk) {
    TickSpriteAnimation1Frame(unk->sprite);
}

void UnkImageStruct_TickSpriteAnimation2Frames(UnkImageStruct *unk) {
    Sprite_TickAnimCtrlFrame(unk->sprite, 2 * FX32_ONE);
}

void UnkImageStruct_TickSpriteAnimationNFrames(UnkImageStruct *unk, fx32 frames) {
    Sprite_TickAnimCtrlFrame(unk->sprite, frames);
}

u32 UnkImageStruct_GetSpriteAnimSeqNo(UnkImageStruct *unk) {
    return Sprite_GetNumAnimSeqs(unk->sprite);
}

void UnkImageStruct_SetSpriteAnimSeqNo(UnkImageStruct *unk, int seqno) {
    Sprite_SetAnimCtrlSeq(unk->sprite, seqno);
}

void UnkImageStruct_TryChangeSpriteAnimSeqNo(UnkImageStruct *unk, int a1) {
    Sprite_TryChangeAnimSeq(unk->sprite, a1);
}

u16 UnkImageStruct_GetSpriteCurrentAnimSeqNo(UnkImageStruct *unk) {
    return Sprite_GetAnimationNumber(unk->sprite);
}

void thunk_Set2dSpriteAnimActiveFlag(Sprite *sprite, int a1) {
    Sprite_SetAnimActiveFlag(sprite, a1);
}

void UnkImageStruct_SetSpriteAnimActiveFlag(UnkImageStruct *unk, int a1) {
    thunk_Set2dSpriteAnimActiveFlag(unk->sprite, a1);
}

void sub_0200DC84(Sprite *sprite, fx32 frame) {
    Sprite_SetAnimSpeed(sprite, frame);
}

void sub_0200DC8C(UnkImageStruct *unk, fx32 frame) {
    sub_0200DC84(unk->sprite, frame);
}

BOOL sub_0200DC98(Sprite *sprite) {
    return Sprite_IsCellAnimationRunning(sprite);
}

BOOL sub_0200DCA0(UnkImageStruct *unk) {
    return sub_0200DC98(unk->sprite);
}

void UnkImageStruct_ResetSpriteAnimCtrlState(UnkImageStruct *unk) {
    Sprite_ResetAnimCtrlState(unk->sprite);
}

void thunk_Sprite_SetAnimCtrlCurrentFrame(Sprite *sprite, u16 frameIndex) {
    Sprite_SetAnimCtrlCurrentFrame(sprite, frameIndex);
}

void UnkImageStruct_SetSpriteAnimCtrlCurrentFrame(UnkImageStruct *unk, u16 frameIndex) {
    thunk_Sprite_SetAnimCtrlCurrentFrame(unk->sprite, frameIndex);
}

u16 thunk_Sprite_GetAnimCtrlCurrentFrame(Sprite *sprite) {
    return Sprite_GetAnimCtrlCurrentFrame(sprite);
}

u16 UnkImageStruct_GetSpriteAnimCtrlCurrentFrame(UnkImageStruct *unk) {
    return thunk_Sprite_GetAnimCtrlCurrentFrame(unk->sprite);
}

void thunk_Set2dSpriteVisibleFlag(Sprite *sprite, int flag) {
    Sprite_SetVisibleFlag(sprite, flag);
}

void UnkImageStruct_SetSpriteVisibleFlag(UnkImageStruct *unk, int flag) {
    thunk_Set2dSpriteVisibleFlag(unk->sprite, flag);
}

BOOL thunk_Get2dSpriteVisibleFlag(Sprite *sprite) {
    return Sprite_GetVisibleFlag(sprite);
}

BOOL UnkImageStruct_GetSpriteVisibleFlag(UnkImageStruct *a0) {
    return thunk_Get2dSpriteVisibleFlag(a0->sprite);
}

void thunk_Sprite_SetPalIndex(Sprite *sprite, int a1) {
    Sprite_SetPalIndex(sprite, a1);
}

void UnkImageStruct_SetSpritePalIndex(UnkImageStruct *unk, int a1) {
    thunk_Sprite_SetPalIndex(unk->sprite, a1);
}

void thunk_Sprite_SetPalOffset(Sprite *sprite, u8 a1) {
    Sprite_SetPalOffset(sprite, a1);
}

void UnkImageStruct_SetSpritePalOffset(UnkImageStruct *unk, u8 a1) {
    thunk_Sprite_SetPalOffset(unk->sprite, a1);
}

u8 UnkImageStruct_GetSpritePalOffset(UnkImageStruct *unk) {
    return Sprite_GetPalOffset(unk->sprite);
}

void thunk_Sprite_SetPriority(Sprite *sprite, int a1) {
    Sprite_SetPriority(sprite, a1);
}

int UnkImageStruct_GetSpritePriority(UnkImageStruct *unk) {
    return Sprite_GetPriority(unk->sprite);
}

void UnkImageStruct_SetSpritePriority(UnkImageStruct *unk, int a1) {
    thunk_Sprite_SetPriority(unk->sprite, a1);
}

void thunk_Sprite_SetDrawPriority(Sprite *sprite, u16 a1) {
    Sprite_SetDrawPriority(sprite, a1);
}

void UnkImageStruct_SetSpriteDrawPriority(UnkImageStruct *unk, u16 a1) {
    thunk_Sprite_SetDrawPriority(unk->sprite, a1);
}

u16 thunk_Sprite_GetDrawPriority(Sprite *sprite) {
    return Sprite_GetDrawPriority(sprite);
}

u16 UnkImageStruct_GetSpriteDrawPriority(UnkImageStruct *unk) {
    return thunk_Sprite_GetDrawPriority(unk->sprite);
}

void Sprite_SetPositionXY(Sprite *sprite, s16 x, s16 y) {
    VecFx32 vec;

    vec.x = x * FX32_ONE;
    vec.y = y * FX32_ONE;
    if (Sprite_GetVramType(sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        vec.y += GX_LCD_SIZE_Y * FX32_ONE;
    }
    vec.z = 0;
    Sprite_SetMatrix(sprite, &vec);
}

void UnkImageStruct_SetSpritePositionXY(UnkImageStruct *unk, s16 x, s16 y) {
    Sprite_SetPositionXY(unk->sprite, x, y);
}

void Sprite_SetPositionXY_CustomScreenYOffset(Sprite *sprite, s16 x, s16 y, fx32 yOffset) {
    VecFx32 vec;

    vec.x = x * FX32_ONE;
    vec.y = y * FX32_ONE;
    if (Sprite_GetVramType(sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        vec.y += yOffset;
    }
    vec.z = 0;
    Sprite_SetMatrix(sprite, &vec);
}

void UnkImageStruct_SetSpritePositionXY_CustomScreenYOffset(UnkImageStruct *unk, s16 x, s16 y, fx32 yOffset) {
    Sprite_SetPositionXY_CustomScreenYOffset(unk->sprite, x, y, yOffset);
}

void Sprite_GetPositionXY(Sprite *sprite, s16 *x, s16 *y) {
    const VecFx32 *pos = Sprite_GetMatrixPtr(sprite);
    *x                 = pos->x / FX32_ONE;
    if (Sprite_GetVramType(sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        *y = (pos->y - GX_LCD_SIZE_Y * FX32_ONE) / FX32_ONE;
    } else {
        *y = pos->y / FX32_ONE;
    }
}

void UnkImageStruct_GetSpritePositionXY(UnkImageStruct *unk, s16 *x, s16 *y) {
    Sprite_GetPositionXY(unk->sprite, x, y);
}

void Sprite_GetPositionXY_CustomScreenYOffset(Sprite *sprite, s16 *x, s16 *y, fx32 yOffset) {
    const VecFx32 *pos = Sprite_GetMatrixPtr(sprite);
    *x                 = pos->x / FX32_ONE;
    if (Sprite_GetVramType(sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        *y = (pos->y - yOffset) / FX32_ONE;
    } else {
        *y = pos->y / FX32_ONE;
    }
}

void UnkImageStruct_GetSpritePositionXY_CustomScreenYOffset(UnkImageStruct *unk, s16 *x, s16 *y, fx32 yOffset) {
    Sprite_GetPositionXY_CustomScreenYOffset(unk->sprite, x, y, yOffset);
}

void Sprite_AddPositionXY(Sprite *sprite, s16 dx, s16 dy) {
    VecFx32 vec;
    const VecFx32 *pos = Sprite_GetMatrixPtr(sprite);
    vec.x              = pos->x + dx * FX32_ONE;
    vec.y              = pos->y + dy * FX32_ONE;
    vec.z              = pos->z;
    Sprite_SetMatrix(sprite, &vec);
}

void UnkImageStruct_AddSpritePositionXY(UnkImageStruct *unk, s16 dx, s16 dy) {
    Sprite_AddPositionXY(unk->sprite, dx, dy);
}

void UnkImageStruct_AddSpritePrecisePositionXY(UnkImageStruct *unk, fx32 dx, fx32 dy) {
    VecFx32 vec;
    const VecFx32 *pos = Sprite_GetMatrixPtr(unk->sprite);
    vec.x              = pos->x + dx;
    vec.y              = pos->y + dy;
    vec.z              = pos->z;
    Sprite_SetMatrix(unk->sprite, &vec);
}

void UnkImageStruct_SetSpritePrecisePositionXY_NoBottomScreenCorrection(UnkImageStruct *unk, fx32 x, fx32 y) {
    VecFx32 vec;
    const VecFx32 *pos = Sprite_GetMatrixPtr(unk->sprite);
    vec.x              = x;
    vec.y              = y;
    vec.z              = pos->z;
    Sprite_SetMatrix(unk->sprite, &vec);
}

void UnkImageStruct_GetSpritePositionXY_NoBottomScreenCorrection(UnkImageStruct *unk, fx32 *x, fx32 *y) {
    const VecFx32 *pos = Sprite_GetMatrixPtr(unk->sprite);
    *x                 = pos->x;
    *y                 = pos->y;
}

void UnkImageStruct_SetSpritePrecisePositionXY(UnkImageStruct *unk, fx32 x, fx32 y, fx32 yOffset) {
    if (Sprite_GetVramType(unk->sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        UnkImageStruct_SetSpritePrecisePositionXY_NoBottomScreenCorrection(unk, x, y + yOffset);
    } else {
        UnkImageStruct_SetSpritePrecisePositionXY_NoBottomScreenCorrection(unk, x, y);
    }
}

void UnkImageStruct_GetSpritePrecisePositionXY(UnkImageStruct *unk, fx32 *x, fx32 *y, fx32 yOffset) {
    UnkImageStruct_GetSpritePositionXY_NoBottomScreenCorrection(unk, x, y);
    if (Sprite_GetVramType(unk->sprite) == NNS_G2D_VRAM_TYPE_2DSUB) {
        *y -= yOffset;
    }
}

void sub_0200DF90(Sprite *sprite, u8 a1) {
    Sprite_SetAffineOverwriteType(sprite, a1);
}

void sub_0200DF98(UnkImageStruct *unk, u8 a1) {
    sub_0200DF90(unk->sprite, a1);
}

void sub_0200DFA4(Sprite *sprite, f32 x, f32 y) {
    VecFx32 *scale = Sprite_GetScalePtr(sprite);
    scale->x       = FX_F32_TO_FX32(x);
    scale->y       = FX_F32_TO_FX32(y);
    Sprite_SetScale(sprite, scale);
}

void sub_0200E024(UnkImageStruct *unk, f32 x, f32 y) {
    sub_0200DFA4(unk->sprite, x, y);
}

void sub_0200E030(Sprite *sprite, f32 *x, f32 *y) {
    VecFx32 *scale = Sprite_GetScalePtr(sprite);
    *x             = FX_FX32_TO_F32(scale->x);
    *y             = FX_FX32_TO_F32(scale->y);
}

void sub_0200E060(UnkImageStruct *unk, f32 *x, f32 *y) {
    sub_0200E030(unk->sprite, x, y);
}

void sub_0200E06C(Sprite *sprite, u16 a1) {
    Sprite_SetRotation(sprite, a1);
}

void sub_0200E074(UnkImageStruct *unk, u16 a1) {
    sub_0200E06C(unk->sprite, a1);
}

void sub_0200E080(Sprite *sprite, u16 a1) {
    u16 rotation = Sprite_GetRotation(sprite);
    rotation += a1;
    Sprite_SetRotation(sprite, rotation);
}

void sub_0200E098(UnkImageStruct *unk, u16 a1) {
    sub_0200E080(unk->sprite, a1);
}

u16 sub_0200E0A4(Sprite *sprite) {
    return Sprite_GetRotation(sprite);
}

u16 sub_0200E0AC(UnkImageStruct *unk) {
    return sub_0200E0A4(unk->sprite);
}

void sub_0200E0B8(Sprite *sprite, u8 a1) {
    Sprite_SetFlip_AffineOff(sprite, a1);
}

void sub_0200E0C0(UnkImageStruct *unk, u8 a1) {
    sub_0200E0B8(unk->sprite, a1);
}

void sub_0200E0CC(UnkImageStruct *unk, s16 x, s16 y) {
    VecFx32 matrix;
    matrix.x = x * FX32_ONE;
    matrix.y = y * FX32_ONE;
    matrix.z = 0;
    Sprite_SetAffineMatrix(unk->sprite, &matrix);
}

void sub_0200E0E8(UnkImageStruct *unk, BOOL mosaic) {
    Sprite_SetMosaic(unk->sprite, mosaic);
}

void sub_0200E0F4(Sprite *sprite, GXOamMode mode) {
    Sprite_SetOamMode(sprite, mode);
}

void sub_0200E0FC(UnkImageStruct *unk, GXOamMode mode) {
    sub_0200E0F4(unk->sprite, mode);
}

GXOamMode sub_0200E108(Sprite *sprite) {
    return Sprite_GetOamMode(sprite);
}

GXOamMode sub_0200E110(UnkImageStruct *unk) {
    return sub_0200E108(unk->sprite);
}

u32 sub_0200E11C(UnkImageStruct *unk) {
    return Sprite_GetCurrentAnimFrameExAttr(unk->sprite);
}

BOOL sub_0200E128(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *obj = AddCharResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], narcId, fileId, compressed, resId, vram, renderer->heapId);
    if (obj != NULL) {
        sub_0200AD64(obj);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], obj);
        return TRUE;
    }
    GF_ASSERT(0);
    return obj != NULL;
}

BOOL sub_0200E188(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *obj = AddCharResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], narcId, fileId, compressed, resId, vram, renderer->heapId);
    if (obj != NULL) {
        sub_0200AE18(obj);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], obj);
        return TRUE;
    }
    GF_ASSERT(0);
    return obj != NULL;
}

BOOL sub_0200E1E8(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, NNS_G2D_VRAM_TYPE vram, int resId) {
    if (!GF2DGfxResObjExistsById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId)) {
        return FALSE;
    }
    GF_2DGfxResObj *obj = AddCharResObjFromOpenNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], narc, fileId, compressed, resId, vram, renderer->heapId);
    if (obj != NULL) {
        sub_0200AE18(obj);
        MyInsertResObjIntoList(gfxHandler->_2dGfxResObjList[GF_GFX_RES_TYPE_CHAR], obj);
        return TRUE;
    }
    GF_ASSERT(0);
    return obj != NULL;
}

void sub_0200E248(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int resId) {
    GF_2DGfxResObj *obj = Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId);
    ReplaceCharResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], obj, narcId, fileId, compressed, renderer->heapId);
    sub_0200AE8C(obj);
}

void sub_0200E27C(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NarcId narcId, int fileId, BOOL compressed, int resId) {
    GF_2DGfxResObj *obj = Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], resId);
    ReplacePlttResObjFromNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], obj, narcId, fileId, compressed, renderer->heapId);
    sub_0200B084(obj);
}

SpriteList *sub_0200E2B0(SpriteGfxHandler *gfxHandler) {
    return gfxHandler->spriteList;
}

void sub_0200E2B4(SpriteGfxHandler *gfxHandler, SpriteList *spriteList) {
    gfxHandler->spriteList = spriteList;
}

void sub_0200E2B8(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int resId) {
    GF_2DGfxResObj *obj = Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], resId);
    ReplaceCharResObjFromOpenNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_CHAR], obj, narc, fileId, compressed, renderer->heapId);
    sub_0200AE8C(obj);
}

void sub_0200E2EC(SpriteRenderer *renderer, SpriteGfxHandler *gfxHandler, NARC *narc, int fileId, BOOL compressed, int resId) {
    GF_2DGfxResObj *obj = Get2DGfxResObjById(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], resId);
    ReplacePlttResObjFromOpenNarc(gfxHandler->_2dGfxResMan[GF_GFX_RES_TYPE_PLTT], obj, narc, fileId, compressed, renderer->heapId);
    sub_0200B084(obj);
}
