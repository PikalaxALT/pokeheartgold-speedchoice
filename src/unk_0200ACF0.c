#include "unk_0200ACF0.h"

#include "global.h"

#include "obj_char_transfer.h"
#include "unk_02022588.h"

BOOL sub_0200ACF0(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    ObjCharTransferTaskTemplate template;
    template.charData = GF2DGfxResObj_GetCharDataPtr(obj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(obj);
    template.id       = GF2DGfxResObj_GetResID(obj);
    template.atEnd    = FALSE;
    return ObjCharTransfer_CreateTaskAndDoTransferFromTemplate(&template);
}

void sub_0200AD30(GF_2DGfxResObjList *charResObjList) {
    GF_ASSERT(charResObjList != NULL);
    for (int i = 0; i < charResObjList->max; ++i) {
        if (charResObjList->obj[i] != NULL) {
            GF_ASSERT(sub_0200ACF0(charResObjList->obj[i]));
        }
    }
}

BOOL sub_0200AD64(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    ObjCharTransferTaskTemplate template;
    template.charData = GF2DGfxResObj_GetCharDataPtr(obj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(obj);
    template.id       = GF2DGfxResObj_GetResID(obj);
    template.atEnd    = FALSE;
    return ObjCharTransfer_CreateTaskAndDoTransferFromTemplate_UpdateMappingTypeFromHW(&template);
}

BOOL sub_0200ADA4(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    ObjCharTransferTaskTemplate template;
    template.charData = GF2DGfxResObj_GetCharDataPtr(obj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(obj);
    template.id       = GF2DGfxResObj_GetResID(obj);
    template.atEnd    = TRUE;
    return ObjCharTransfer_CreateTaskAndDoTransferFromTemplate(&template);
}

void sub_0200ADE4(GF_2DGfxResObjList *charResObjList) {
    GF_ASSERT(charResObjList != NULL);
    for (int i = 0; i < charResObjList->max; ++i) {
        if (charResObjList->obj[i] != NULL) {
            GF_ASSERT(sub_0200ADA4(charResObjList->obj[i]));
        }
    }
}

BOOL sub_0200AE18(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    ObjCharTransferTaskTemplate template;
    template.charData = GF2DGfxResObj_GetCharDataPtr(obj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(obj);
    template.id       = GF2DGfxResObj_GetResID(obj);
    template.atEnd    = TRUE;
    return ObjCharTransfer_CreateTaskAndDoTransferFromTemplate_UpdateMappingTypeFromHW(&template);
}

void sub_0200AE58(GF_2DGfxResObjList *charResObjList) {
    GF_ASSERT(charResObjList != NULL);
    for (int i = 0; i < charResObjList->max; ++i) {
        if (charResObjList->obj[i] != NULL) {
            GF_ASSERT(sub_0200AE18(charResObjList->obj[i]));
        }
    }
}

void sub_0200AE8C(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);

    int id                        = GF2DGfxResObj_GetResID(obj);
    NNSG2dCharacterData *charData = GF2DGfxResObj_GetCharDataPtr(obj);
    ObjCharTransfer_ReplaceGraphicsFromChardataByResID(id, charData);
}

void sub_0200AEB0(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    ObjCharTransfer_ResetTransferTasksByResID(GF2DGfxResObj_GetResID(obj));
}

void sub_0200AED4(GF_2DGfxResObjList *charResObjList) {
    GF_ASSERT(charResObjList != NULL);
    for (int i = 0; i < charResObjList->max; ++i) {
        if (charResObjList->obj[i] != NULL) {
            sub_0200AEB0(charResObjList->obj[i]);
        }
    }
}

NNSG2dImageProxy *sub_0200AF00(GF_2DGfxResObj *obj) {
    GF_ASSERT(obj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(obj) == GF_GFX_RES_TYPE_CHAR);

    return ObjCharTransfer_GetProxyPtrByResID(GF2DGfxResObj_GetResID(obj));
}

NNSG2dImageProxy *sub_0200AF24(GF_2DGfxResObj *charResObj, GF_2DGfxResObj *cellResObj) {
    NNSG2dImageProxy *ret;

    GF_ASSERT(charResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(charResObj) == GF_GFX_RES_TYPE_CHAR);
    GF_ASSERT(cellResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(cellResObj) == GF_GFX_RES_TYPE_CELL);

    int resID                    = GF2DGfxResObj_GetResID(charResObj);
    NNSG2dCellDataBank *cellData = GF2DGfxResObj_GetCellDataPtr(cellResObj);
    ret                          = ObjCharTransfer_ResizeTaskByResID_GetProxyPtr(resID, cellData->pVramTransferData->szByteMax);
    if (ret == NULL) {
        ret = ObjCharTransfer_CopyTransferTaskByProxyPtr(ObjCharTransfer_GetProxyPtrByResID(resID));
    }
    return ret;
}

void sub_0200AF80(const NNSG2dImageProxy *imgProxy) {
    GF_ASSERT(imgProxy != NULL);
    ObjCharTransfer_DeleteTaskCopyByProxyPtr(imgProxy);
}

BOOL sub_0200AF94(GF_2DGfxResObj *plttResObj) {
    GF_ASSERT(plttResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(plttResObj) == GF_GFX_RES_TYPE_PLTT);

    UnkStruct_02022660 template;
    template.plttData = GF2DGfxResObj_GetPlttDataPtr(plttResObj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(plttResObj);
    template.id       = GF2DGfxResObj_GetResID(plttResObj);
    template.plttNum  = GF2DGfxResObj_GetPlttNum(plttResObj);
    return sub_02022660(&template);
}

void sub_0200AFD8(GF_2DGfxResObjList *plttResObjList) {
    GF_ASSERT(plttResObjList != NULL);
    for (int i = 0; i < plttResObjList->max; ++i) {
        if (plttResObjList->obj[i] != NULL) {
            GF_ASSERT(sub_0200AF94(plttResObjList->obj[i]));
        }
    }
}

BOOL sub_0200B00C(GF_2DGfxResObj *plttResObj) {
    GF_ASSERT(plttResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(plttResObj) == GF_GFX_RES_TYPE_PLTT);

    UnkStruct_02022660 template;
    template.plttData = GF2DGfxResObj_GetPlttDataPtr(plttResObj);
    template.vram     = GF2DGfxResObj_GetLoadAddress(plttResObj);
    template.id       = GF2DGfxResObj_GetResID(plttResObj);
    template.plttNum  = GF2DGfxResObj_GetPlttNum(plttResObj);
    return sub_020226A4(&template);
}

void sub_0200B050(GF_2DGfxResObjList *plttResObjList) {
    GF_ASSERT(plttResObjList != NULL);
    for (int i = 0; i < plttResObjList->max; ++i) {
        if (plttResObjList->obj[i] != NULL) {
            GF_ASSERT(sub_0200B00C(plttResObjList->obj[i]));
        }
    }
}

void sub_0200B084(GF_2DGfxResObj *plttResObj) {
    GF_ASSERT(plttResObj != NULL);
    int resID                   = GF2DGfxResObj_GetResID(plttResObj);
    NNSG2dPaletteData *plttData = GF2DGfxResObj_GetPlttDataPtr(plttResObj);
    sub_020226E4(resID, plttData);
}

void sub_0200B0A8(GF_2DGfxResObj *plttResObj) {
    GF_ASSERT(plttResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(plttResObj) == GF_GFX_RES_TYPE_PLTT);

    sub_02022744(GF2DGfxResObj_GetResID(plttResObj));
}

void sub_0200B0CC(GF_2DGfxResObjList *plttResObjList) {
    GF_ASSERT(plttResObjList != NULL);
    for (int i = 0; i < plttResObjList->max; ++i) {
        if (plttResObjList->obj[i] != NULL) {
            sub_0200B0A8(plttResObjList->obj[i]);
        }
    }
}

NNSG2dImagePaletteProxy *sub_0200B0F8(GF_2DGfxResObj *plttResObj, NNSG2dImageProxy *imageProxy) {
    GF_ASSERT(plttResObj != NULL);
    GF_ASSERT(GF2DGfxResObj_GetResType(plttResObj) == GF_GFX_RES_TYPE_PLTT);

    int resID = GF2DGfxResObj_GetResID(plttResObj);
    if (imageProxy != NULL) {
        return sub_020227D4(resID, imageProxy);
    } else {
        return sub_020227AC(resID);
    }
}

int sub_0200B12C(GF_2DGfxResObj *plttResObj, NNS_G2D_VRAM_TYPE vram) {
    NNSG2dImagePaletteProxy *plttProxy = sub_0200B0F8(plttResObj, NULL);
    int location                       = NNS_G2dGetImagePaletteLocation(plttProxy, vram);
    if (location != -1) {
        location /= 32;
    }
    return location;
}
