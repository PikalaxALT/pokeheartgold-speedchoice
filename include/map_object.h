#ifndef POKEHEARTGOLD_MAP_OBJECT_H
#define POKEHEARTGOLD_MAP_OBJECT_H

#include "field_types_def.h"

typedef struct SavedMapObject {
    u32 flags;
    u32 flags2;
    u8 objId;
    u8 movement;
    s8 xRange;
    s8 yRange;
    s8 initialFacing;
    s8 currentFacing;
    s8 nextFacing;
    u16 unk10;
    u16 spriteId;
    u16 type;
    u16 flagId;
    u16 script;
    s16 param[3];
    s16 initialX;
    s16 initialHeight;
    s16 initialY;
    s16 currentX;
    s16 currentHeight;
    s16 currentY;
    fx32 vecY;
    u8 unk30[16];
    u8 unk40[16];
} SavedMapObject;

struct MapObjectManager { // declared field_system.h
    u32 flags;
    u32 object_count;
    u32 unk8;
    u32 priority;
    u8 unk10[4];
    NARC *mmodel_narc;
    u8 unk18[4];
    u8 unk1C[0x124 - 0x1C];
    LocalMapObject *objects;
    FieldSystem *fieldSystem;
}; // size: 0x12c

typedef void (*LocalMapObject_UnkCallback)(LocalMapObject *object);

struct LocalMapObject { // declared field_system.h
    /*0x000*/ u32 flags;
    /*0x004*/ u32 flags2;
    /*0x008*/ int id;
    /*0x00C*/ int unkC; // some kind of flag ID
    /*0x010*/ int spriteId;
    /*0x014*/ int movement;
    /*0x018*/ int type;
    /*0x01C*/ int evFlagId;
    /*0x020*/ int scriptId;
    /*0x024*/ int initialFacing;
    /*0x028*/ int currentFacing;
    /*0x02C*/ int nextFacing;
    /*0x030*/ int currentFacingBak;
    /*0x034*/ int nextFacingBak;
    /*0x038*/ int param[3];
    /*0x044*/ int xrange;
    /*0x048*/ int yrange;
    /*0x04C*/ int initialX;
    /*0x050*/ int initialHeight;
    /*0x054*/ int initialY;
    /*0x058*/ int previousX;
    /*0x05C*/ int previousHeight;
    /*0x060*/ int previousY;
    /*0x064*/ int currentX;
    /*0x068*/ int currentHeight;
    /*0x06C*/ int currentY;
    /*0x070*/ VecFx32 positionVec;
    /*0x07C*/ VecFx32 facingVec;
    /*0x088*/ VecFx32 unk88;
    /*0x094*/ VecFx32 unk94;
    /*0x0A0*/ int unkA0;
    /*0x0A4*/ int movementCmd;
    /*0x0A8*/ int movementStep;
    /*0x0AC*/ u16 unkAC;
    /*0x0AE*/ u16 unkAE;
    /*0x0B0*/ SysTask *unkB0;
    /*0x0B4*/ MapObjectManager *manager;
    /*0x0B8*/ LocalMapObject_UnkCallback unkB8;
    /*0x0BC*/ LocalMapObject_UnkCallback unkBC;
    /*0x0C0*/ LocalMapObject_UnkCallback unkC0;
    /*0x0C4*/ LocalMapObject_UnkCallback unkC4;
    /*0x0C8*/ LocalMapObject_UnkCallback unkC8;
    /*0x0CC*/ LocalMapObject_UnkCallback unkCC;
    /*0x0D0*/ LocalMapObject_UnkCallback unkD0;
    /*0x0D4*/ LocalMapObject_UnkCallback unkD4;
    /*0x0D8*/ u8 unkD8[0x10];
    /*0x0E8*/ u8 unkE8[0x10];
    /*0x0F8*/ u8 unkF8[0x10];
    /*0x108*/ u8 unk108[0x20];
    /*0x128*/ u16 unk128;
};

typedef struct UnkLMOCallbackStruct {
    u32 unk0;
    LocalMapObject_UnkCallback unk4;
    LocalMapObject_UnkCallback unk8;
    LocalMapObject_UnkCallback unkC;
    LocalMapObject_UnkCallback unk10;
} UnkLMOCallbackStruct;

typedef struct UnkLMOCallbackStruct2 {
    LocalMapObject_UnkCallback unk0;
    LocalMapObject_UnkCallback unk4;
    LocalMapObject_UnkCallback unk8;
    LocalMapObject_UnkCallback unkC;
    LocalMapObject_UnkCallback unk10;
} UnkLMOCallbackStruct2;

typedef enum MapObjectFlagBits {
    MAPOBJECTFLAG_NONE            = 0,
    MAPOBJECTFLAG_ACTIVE          = (1 << 0),
    MAPOBJECTFLAG_SINGLE_MOVEMENT = (1 << 1),
    MAPOBJECTFLAG_UNK2            = (1 << 2),
    MAPOBJECTFLAG_UNK3            = (1 << 3),
    MAPOBJECTFLAG_UNK4            = (1 << 4),
    MAPOBJECTFLAG_UNK5            = (1 << 5),
    MAPOBJECTFLAG_MOVEMENT_PAUSED = (1 << 6),
    MAPOBJECTFLAG_UNK7            = (1 << 7),
    MAPOBJECTFLAG_UNK8            = (1 << 8),
    MAPOBJECTFLAG_VISIBLE         = (1 << 9),
    MAPOBJECTFLAG_UNK10           = (1 << 10),
    MAPOBJECTFLAG_UNK11           = (1 << 11),
    MAPOBJECTFLAG_UNK12           = (1 << 12),
    MAPOBJECTFLAG_UNK13           = (1 << 13),
    MAPOBJECTFLAG_UNK14           = (1 << 14),
    MAPOBJECTFLAG_UNK15           = (1 << 15),
    MAPOBJECTFLAG_UNK16           = (1 << 16),
    MAPOBJECTFLAG_UNK17           = (1 << 17),
    MAPOBJECTFLAG_UNK18           = (1 << 18),
    MAPOBJECTFLAG_UNK19           = (1 << 19),
    MAPOBJECTFLAG_UNK20           = (1 << 20),
    MAPOBJECTFLAG_UNK21           = (1 << 21),
    MAPOBJECTFLAG_UNK22           = (1 << 22),
    MAPOBJECTFLAG_IGNORE_HEIGHTS  = (1 << 23),
    MAPOBJECTFLAG_UNK24           = (1 << 24),
    MAPOBJECTFLAG_UNK25           = (1 << 25),
    MAPOBJECTFLAG_UNK26           = (1 << 26),
    MAPOBJECTFLAG_UNK27           = (1 << 27),
    MAPOBJECTFLAG_UNK28           = (1 << 28),
    MAPOBJECTFLAG_UNK29           = (1 << 29),
    MAPOBJECTFLAG_UNK30           = (1 << 30),
    MAPOBJECTFLAG_UNK31           = (1 << 31),
} MapObjectFlagBits;

#define MAP_OBJECT_GFX_ID_INVALID 0xFFFF

typedef struct MapObjectInitArgs {
    u32 mapNo;
    int objectEventCount;
    int index;
    MapObjectManager *manager;
    ObjectEvent *objectEvents;
} MapObjectInitArgs;

// FIXME: Some of these declarations are static and don't belong in here.
MapObjectManager *MapObjectManager_Init(FieldSystem *fieldSystem, u32 objectCount, u32 priority);
void MapObjectManager_Delete(MapObjectManager *manager);
void sub_0205E104(MapObjectManager *manager, u32 unused, u32 flagId, u32 objectCount, ObjectEvent *objectEvents);
LocalMapObject *MapObject_Create(MapObjectManager *manager, u32 x, u32 z, u32 direction, u32 sprite, u32 movement, u32 mapNo);
LocalMapObject *MapObject_CreateWithParams(MapObjectManager *manager, u32 x, u32 y, u32 direction, u32 sprite, u32 movement, u32 mapNo, u32 param0, u32 param1, u32 param2);
LocalMapObject *MapObject_CreateFromObjectEventWithId(MapObjectManager *mapObjectManager, u16 objectId, u32 numObjects, u32 mapId, const ObjectEvent *templates);
void sub_0205E38C(LocalMapObject *object, u32 sprite);
void sub_0205E3AC(LocalMapObject *object, u32 sprite);
void MapObject_Remove(LocalMapObject *object);
void MapObject_Delete(LocalMapObject *object);
void sub_0205E420(LocalMapObject *object);
void MapObjectManager_RemoveAllActiveObjects(MapObjectManager *manager);
void sub_0205E4C8(MapObjectManager *manager);
void sub_0205E520(MapObjectManager *manager);
void sub_0205E580(MapObjectManager *manager);
void FieldSystem_SyncMapObjectsToSaveEx(FieldSystem *fieldSystem, MapObjectManager *manager, SavedMapObject *savedObjects, s32 count);
void MapObjectManager_RestoreFromSave(MapObjectManager *mapObjectManager, SavedMapObject *savedObjects, u32 objectCount);
void MapObject_CreateFromMultipleObjectEvents(MapObjectManager *manager, u32 mapNo, u32 objectEventCount, ObjectEvent *objectEvents);
LocalMapObject *MapObjectManager_GetFirstActiveObjectByID(MapObjectManager *manager, u32 id);
LocalMapObject *MapObjectManager_GetFirstActiveObjectWithMovement(MapObjectManager *manager, u32 movement);
BOOL MapObjectManager_GetNextObjectWithFlagFromIndex(MapObjectManager *manager, LocalMapObject **objectDest, s32 *index, MapObjectFlagBits flag);
u32 sub_0205F09C(LocalMapObject *object, u32 param1);
BOOL sub_0205F0A8(LocalMapObject *object, u32 mapId, u32 flagId);
BOOL sub_0205F0F8(LocalMapObject *object, u32 spriteId, u32 id, u32 a3);
void sub_0205F12C(SysTask *task, LocalMapObject *object);
void sub_0205F148(LocalMapObject *object);
MapObjectManager *MapObjectManager_Get(MapObjectManager *manager);
void MapObjectManager_SetObjectCount(MapObjectManager *, u32);
u32 MapObjectManager_GetObjectCount(MapObjectManager *);
void sub_0205F16C(MapObjectManager *manager);
void sub_0205F174(MapObjectManager *manager);
void MapObjectManager_SetFlagsBits(MapObjectManager *manager, u32 bits);
void MapObjectManager_ClearFlagsBits(MapObjectManager *manager, u32 bits);
u32 MapObjectManager_GetFlagsBitsMask(MapObjectManager *manager, u32 bits);
void MapObjectManager_SetPriority(MapObjectManager *manager, u32 priority);
u32 MapObjectManager_GetPriority(MapObjectManager *manager);
void *sub_0205F1A0(MapObjectManager *);
void MapObjectManager_SetObjects(MapObjectManager *manager, LocalMapObject *objects);
LocalMapObject *sub_0205F1AC(MapObjectManager *manager);
const LocalMapObject *MapObjectManager_GetConstObjects(MapObjectManager *);
LocalMapObject *MapObjectManager_GetObjects(MapObjectManager *);
void sub_0205F1C4(LocalMapObject **objects);
void sub_0205F1D0(LocalMapObject **objects);
void MapObjectManager_SetFieldSystemPtr(MapObjectManager *, FieldSystem *);
FieldSystem *MapObjectManager_GetFieldSystemPtr(MapObjectManager *);
void FldObjSys_SetMModelNarc(MapObjectManager *manager, NARC *mmodel_narc);
NARC *FldObjSys_GetMModelNarc(MapObjectManager *manager);
void MapObject_SetFlagsWord(LocalMapObject *object, u32 bits);
u32 MapObject_GetFlagsWord(LocalMapObject *object);
void MapObject_SetFlagsBits(LocalMapObject *mapObject, MapObjectFlagBits bits);
void MapObject_ClearFlagsBits(LocalMapObject *mapObject, MapObjectFlagBits bits);
u32 MapObject_GetFlagsBits(LocalMapObject *object, MapObjectFlagBits mask);
BOOL MapObject_TestFlagsBits(LocalMapObject *object, MapObjectFlagBits bits);
void MapObject_SetFlags2Word(LocalMapObject *object, u32 bits);
u32 MapObject_GetFlags2Word(LocalMapObject *object);
u32 MapObject_GetFlags2Mask(LocalMapObject *object, u32 mask);
void MapObject_SetID(LocalMapObject *mapObject, u32 id);
u32 MapObject_GetID(LocalMapObject *obj);
void sub_0205F250(LocalMapObject *object, u32 a1);
u32 sub_0205F254(LocalMapObject *obj);
void MapObject_SetSpriteID(LocalMapObject *mapObject, u32 spriteId);
u32 MapObject_GetSpriteID(LocalMapObject *mapObject);
void MapObject_SetMovement(LocalMapObject *object, u32 movement);
u32 MapObject_GetMovement(LocalMapObject *mapObject);
void MapObject_SetType(LocalMapObject *mapObject, u32 type);
u32 MapObject_GetType(LocalMapObject *mapObject);
void MapObject_SetFlagID(LocalMapObject *mapObject, u32 flagId);
u32 MapObject_GetFlagID(LocalMapObject *object);
void MapObject_SetScriptID(LocalMapObject *mapObject, u32 script);
u32 MapObject_GetScriptID(LocalMapObject *mapObject);
void MapObject_SetInitialFacingDirection(LocalMapObject *object, u32 initial_facing);
u32 MapObject_GetInitialFacingDirection(LocalMapObject *object);
void MapObject_SetFacingDirectionDirect(LocalMapObject *object, u32 direction);
void MapObject_SetFacingDirection(LocalMapObject *object, u32 direction);
u32 MapObject_GetFacingDirection(LocalMapObject *object);
u32 MapObject_GetPreviousFacing(LocalMapObject *object);
void MapObject_SetNextFacingDirection(LocalMapObject *object, u32 direction);
u32 MapObject_GetNextFacingDirection(LocalMapObject *object);
void MapObject_SetOrQueueFacing(LocalMapObject *object, u32 direction);
void MapObject_SetParam(LocalMapObject *mapObject, u32 value, int which);
int MapObject_GetParam(LocalMapObject *mapObject, int which);
void MapObject_SetXRange(LocalMapObject *mapObject, u32 xRange);
u32 MapObject_GetXRange(LocalMapObject *object);
void MapObject_SetYRange(LocalMapObject *mapObject, u32 yRange);
u32 MapObject_GetYRange(LocalMapObject *object);
void sub_0205F328(LocalMapObject *obj, u32 a1);
// FIXME: Functions beyond here aren't organized according to the corresponding c file
void sub_0205FC94(LocalMapObject *mapObject, u32 movement);
void MapObject_ClearFlag18(LocalMapObject *mapObject, BOOL clear);
void MapObject_SetFlag29(LocalMapObject *mapObject, BOOL enable_bit);
BOOL MapObject_CheckVisible(LocalMapObject *mapObject);
BOOL MapObject_CheckFlag28(LocalMapObject *mapObject);
void MapObject_SetVisible(LocalMapObject *mapObject, BOOL enable_bit);
void MapObject_SetFlag19(LocalMapObject *mapObject, BOOL enable_bit);
SavedMapObject *SaveMapObjects_SearchSpriteId(SavedMapObject *list, u32 num_objects, u16 sprite_id);
void sub_0205F55C(MapObjectManager *man);
BOOL MapObject_IsMovementPaused(LocalMapObject *obj);
BOOL MapObject_IsSingleMovementActive(LocalMapObject *obj);
void MapObject_PauseMovement(LocalMapObject *obj);
void MapObject_UnpauseMovement(LocalMapObject *obj);
void MapObjectManager_PauseAllMovement(MapObjectManager *man);
void MapObjectManager_UnpauseAllMovement(MapObjectManager *man);
u32 MapObject_GetCurrentX(LocalMapObject *object);
u32 MapObject_GetCurrentY(LocalMapObject *object);
int MapObject_GetCurrentHeight(LocalMapObject *object);
VecFx32 *MapObject_GetPositionVecPtr(LocalMapObject *object);
void sub_0205F9A0(LocalMapObject *object, const VecFx32 *vec);
void MapObject_SetFlag10(LocalMapObject *object, BOOL set);
void sub_0205FC2C(LocalMapObject *obj, u32 x, u32 height, u32 y, u32 direction);
void MapObject_GetPositionVec(LocalMapObject *object, VecFx32 *dest);
void MapObject_SetPositionVec(LocalMapObject *object, VecFx32 *src);
u16 ObjectEvent_GetID(ObjectEvent *);
u16 ObjectEvent_GetFlagID_AssertScriptIDIsUnset(ObjectEvent *template);
BOOL ObjectEvent_ScriptIDIsUnset(ObjectEvent *template);
BOOL MapObject_IsInUse(LocalMapObject *object);
void ObjectEvent_SetId(ObjectEvent *template, u16);
void ObjectEvent_SetSprite(ObjectEvent *, u32);
void ObjectEvent_SetMovement(ObjectEvent *, u32);
void ObjectEvent_SetType(ObjectEvent *, u16);
void ObjectEvent_SetFlagId(ObjectEvent *, u16);
void ObjectEvent_SetScript(ObjectEvent *, u16);
void ObjectEvent_SetFacing(ObjectEvent *, u32);
void ObjectEvent_SetParam(ObjectEvent *, u32 value, int param);
void ObjectEvent_SetXRange(ObjectEvent *, s16);
void ObjectEvent_SetYRange(ObjectEvent *, s16);
void ObjectEvent_SetXCoord(ObjectEvent *, u32);
void ObjectEvent_SetYCoord(ObjectEvent *, u32);
void ObjectEvent_SetHeight(ObjectEvent *, u32);
u8 *sub_0205F3BC(LocalMapObject *object);
u8 *sub_0205F394(LocalMapObject *object);
BOOL MapObject_CheckIgnoreHeights(LocalMapObject *object);
BOOL MapObject_CheckFlag29(LocalMapObject *object);
fx32 MapObject_GetPosVecYCoord(LocalMapObject *object);
u32 MapObject_GetInitialY(LocalMapObject *object);
u32 MapObject_GetInitialHeight(LocalMapObject *object);
u32 MapObject_GetInitialX(LocalMapObject *object);
void sub_0205F4AC(LocalMapObject *object);
void sub_0205F4C0(LocalMapObject *object);
BOOL MapObject_CheckFlag14(LocalMapObject *object);
void MapObject_SetCurrentY(LocalMapObject *object, u32 y);
void MapObject_SetCurrentHeight(LocalMapObject *object, u32 height);
void MapObject_SetCurrentX(LocalMapObject *object, u32 x);
void MapObject_SetInitialY(LocalMapObject *object, u32 initial_y);
void MapObject_SetInitialHeight(LocalMapObject *object, u32 initial_height);
void MapObject_SetInitialX(LocalMapObject *object, u32 initial_x);
u16 ObjectEvent_GetSpriteID(ObjectEvent *);
u16 ObjectEvent_GetMovement(ObjectEvent *);
u16 ObjectEvent_GetType(ObjectEvent *);
s16 ObjectEvent_GetInitialFacingDirection(ObjectEvent *);
u16 ObjectEvent_GetScriptID(ObjectEvent *);
u16 ObjectEvent_GetParam(ObjectEvent *, int param);
s16 ObjectEvent_GetXRange(ObjectEvent *);
s16 ObjectEvent_GetYRange(ObjectEvent *);
void MapObject_SetPreviousY(LocalMapObject *object, u32 previous_y);
void MapObject_SetPreviousHeight(LocalMapObject *object, u32 previous_height);
void MapObject_SetPreviousX(LocalMapObject *object, u32 previous_x);
u32 sub_0205F544(LocalMapObject *object);
int MapObject_CheckFlag25(LocalMapObject *object);
void MapObject_SetFlag25(LocalMapObject *object, BOOL set);
void sub_0205FD20(LocalMapObject *object);
u8 *sub_0205F3E4(LocalMapObject *object);
u8 *sub_0205F40C(LocalMapObject *object);
void sub_0205F444(LocalMapObject *object);
void sub_0205F450(LocalMapObject *object);
void sub_0205F468(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F47C(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F490(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F498(LocalMapObject *object);
void sub_0205F4A4(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F4B8(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void MapObject_SetManager(LocalMapObject *object, MapObjectManager *manager);
MapObjectManager *MapObject_GetManager(LocalMapObject *object);
MapObjectManager *sub_0205F364(LocalMapObject *object);
ObjectEvent *ObjectEvent_GetById(u16 id, int num_templates, ObjectEvent *templates);
void sub_0205F338(LocalMapObject *object, SysTask *a1);
BOOL FieldSystem_FlagCheck(FieldSystem *, u16);
u16 ObjectEvent_GetXCoord(ObjectEvent *template);
u32 ObjectEvent_GetHeight(ObjectEvent *template);
u16 ObjectEvent_GetYCoord(ObjectEvent *template);
UnkLMOCallbackStruct *sub_0205FB00(u32 movement);
void *sub_0205F538(LocalMapObject *object);
void sub_0205F414(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F428(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F43C(LocalMapObject *object, LocalMapObject_UnkCallback callback);
void sub_0205F470(LocalMapObject *object);
void MapObject_SetFlag14(LocalMapObject *object);
LocalMapObject_UnkCallback sub_0205FB18(UnkLMOCallbackStruct *unk);
LocalMapObject_UnkCallback sub_0205FB1C(UnkLMOCallbackStruct *unk);
LocalMapObject_UnkCallback sub_0205FB20(UnkLMOCallbackStruct *unk);
LocalMapObject_UnkCallback sub_0205FB24(UnkLMOCallbackStruct2 *unk);
LocalMapObject_UnkCallback sub_0205FB28(UnkLMOCallbackStruct2 *unk);
LocalMapObject_UnkCallback sub_0205FB2C(UnkLMOCallbackStruct2 *unk);
LocalMapObject_UnkCallback sub_0205FB30(UnkLMOCallbackStruct2 *unk);
LocalMapObject_UnkCallback sub_0205FB34(UnkLMOCallbackStruct2 *unk);
UnkLMOCallbackStruct2 *sub_0205FB38(u32 spriteId);
void sub_0205F348(LocalMapObject *object);
u16 ObjectEvent_GetFlagID(ObjectEvent *);
FieldSystem *MapObject_GetFieldSystem(LocalMapObject *object);
void sub_0205FCC4(LocalMapObject *object);
void sub_0205FCC8(LocalMapObject *object);
void sub_0205FCCC(LocalMapObject *object);
void sub_0205FCD0(LocalMapObject *object);
BOOL sub_0205F73C(LocalMapObject *object);
BOOL sub_0205F5D4(MapObjectManager *manager);
u32 sub_0205F330(LocalMapObject *object);
SysTask *sub_0205F340(LocalMapObject *object);
u8 *sub_0205F370(LocalMapObject *object, s32 size);
u8 *sub_0205F398(LocalMapObject *object, s32 size);
u8 *sub_0205F3C0(LocalMapObject *object, s32 size);
u8 *sub_0205F3E8(LocalMapObject *object, s32 size);
void sub_0205F41C(LocalMapObject *object);
void sub_0205F430(LocalMapObject *object);
void sub_0205F484(LocalMapObject *object);
void MapObject_SetMovementCommand(LocalMapObject *object, u32 command);
u32 MapObject_GetMovementCommand(LocalMapObject *object);
void MapObject_SetMovementStep(LocalMapObject *object, u32 step);
void MapObject_IncMovementStep(LocalMapObject *object);
u32 MapObject_GetMovementStep(LocalMapObject *object);
void sub_0205F4FC(LocalMapObject *object, u16 a1);
u16 sub_0205F504(LocalMapObject *object);
void sub_0205F50C(LocalMapObject *object, u16 a1);
u16 sub_0205F514(LocalMapObject *object);
void sub_0205F51C(LocalMapObject *object, u16 a1);
u16 sub_0205F524(LocalMapObject *object);
void sub_0205F568(MapObjectManager *manager);
u32 sub_0205F5E8(LocalMapObject *object, u32 bits);
void sub_0205F5F8(MapObjectManager *manager, BOOL clear);
BOOL sub_0205F610(MapObjectManager *manager);
void MapObject_SingleMovementSetActive(LocalMapObject *object);
void MapObject_SingleMovementSetInactive(LocalMapObject *object);
void MapObject_SetFlag2(LocalMapObject *object);
void MapObject_ClearFlag3(LocalMapObject *object);
BOOL MapObject_CheckFlag19Is0(LocalMapObject *object);
BOOL sub_0205F714(LocalMapObject *object);
void MapObject_SetIgnoreHeights(LocalMapObject *object, BOOL set);
void MapObject_SetFlag26(LocalMapObject *object, BOOL set);
BOOL MapObject_CheckFlag26(LocalMapObject *object);
void MapObject_SetFlag28(LocalMapObject *object, BOOL set);
void MapObject_SetFlag24(LocalMapObject *object, BOOL set);
BOOL MapObject_CheckFlag24(LocalMapObject *object);
BOOL MapObject_CheckFlag4(LocalMapObject *object);
BOOL sub_0205F8D0(LocalMapObject *object);
u32 MapObject_GetPrevX(LocalMapObject *object);
u32 MapObject_GetPrevHeight(LocalMapObject *object);
u32 MapObject_GetPrevY(LocalMapObject *object);
void MapObject_AddCurrentX(LocalMapObject *object, u32 x);
void MapObject_AddCurrentHeight(LocalMapObject *object, u32 height);
void MapObject_AddCurrentY(LocalMapObject *object, u32 y);
void MapObject_GetFacingVec(LocalMapObject *object, VecFx32 *face_vec_dest);
void MapObject_SetFacingVec(LocalMapObject *object, VecFx32 *face_vec);
VecFx32 *MapObject_GetFacingVecPtr(LocalMapObject *object);
void sub_0205F990(LocalMapObject *object, VecFx32 *a1_dest);
void sub_0205F9B0(LocalMapObject *object, VecFx32 *a1_dest);
void sub_0205F9C0(LocalMapObject *object, VecFx32 *a1);
u32 sub_0205F9D0(LocalMapObject *object);
LocalMapObject *sub_0205FB58(MapObjectManager *manager, u32 x, u32 y, BOOL a3);
void sub_0205FBC0(LocalMapObject *object, VecFx32 *position_vec, u32 direction);
void sub_0205FCB4(LocalMapObject *object);
void sub_0205FCB8(LocalMapObject *object);
void sub_0205FCBC(LocalMapObject *object);
void sub_0205FCC0(LocalMapObject *object);
void sub_0205FCD4(LocalMapObject *object);

#endif // POKEHEARTGOLD_MAP_OBJECT_H
