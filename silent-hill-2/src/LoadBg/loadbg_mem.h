#ifndef LOADBG_MEM_H
#define LOADBG_MEM_H

#include "data/fs_structs.h"

// total size: 0x10
typedef struct _loadBgMem_UnitC {
    // Members
    fsFileIndex* file; // offset 0x0, size 0x4
    u_short unitID; // offset 0x4, size 0x2
    u_short unitNo; // offset 0x6, size 0x2
    struct _loadBgMem_UnitC* prevC; // offset 0x8, size 0x4
    struct _loadBgMem_UnitC* nextC; // offset 0xC, size 0x4
} loadBgMem_UnitC;

// total size: 0x18
typedef struct _loadBgMem_CacheCtrl {
    // Members
    int UnitSize; // offset 0x0, size 0x4
    int Units; // offset 0x4, size 0x4
    char* Buffer; // offset 0x8, size 0x4
    struct _loadBgMem_UnitC* UnitCArray; // offset 0xC, size 0x4
    struct _loadBgMem_UnitC* headC; // offset 0x10, size 0x4
    struct _loadBgMem_UnitC* tailC; // offset 0x14, size 0x4
} loadBgMem_CacheCtrl;

// total size: 0x10
typedef struct loadBgMem_File {
    // Members
    fsFileIndex* file; // offset 0x0, size 0x4
    int ofsS; // offset 0x4, size 0x4
    int ofsE; // offset 0x8, size 0x4
    void* addr; // offset 0xC, size 0x4
} loadBgMem_File;

// total size: 0x14
typedef struct loadBgMem_Sect {
    // Members
    int ofsS; // offset 0x0, size 0x4
    int ofsE; // offset 0x4, size 0x4
    int files; // offset 0x8, size 0x4
    struct loadBgMem_File* filelist; // offset 0xC, size 0x4
    s_char reduceRate8; // offset 0x10, size 0x1
    s_char upper; // offset 0x11, size 0x1
    s_char overwrite; // offset 0x12, size 0x1
    s_char sectID; // offset 0x13, size 0x1
} loadBgMem_Sect;

// total size: 0x10
typedef struct _loadBgMem_UnitR {
    // Members
    fsFileIndex* file; // offset 0x0, size 0x4
    u_short unitID; // offset 0x4, size 0x2
    u_short unitNo; // offset 0x6, size 0x2
    int sectID; // offset 0x8, size 0x4
    struct _loadBgMem_UnitR* nextR; // offset 0xC, size 0x4
} loadBgMem_UnitR;

// total size: 0x10
typedef struct _loadBgMem_UnitL {
    // Members
    fsFileIndex* file; // offset 0x0, size 0x4
    u_short unitID; // offset 0x4, size 0x2
    u_short unitNo; // offset 0x6, size 0x2
    int fid; // offset 0x8, size 0x4
    int pad; // offset 0xC, size 0x4
} loadBgMem_UnitL;

// total size: 0x18
typedef struct _loadBgMem_LoadCtrl {
    // Members
    int UnitSize; // offset 0x0, size 0x4
    int Units; // offset 0x4, size 0x4
    char* Buffer; // offset 0x8, size 0x4
    struct _loadBgMem_UnitL* UnitLArray; // offset 0xC, size 0x4
    struct _loadBgMem_UnitR* UnitRArray; // offset 0x10, size 0x4
    struct _loadBgMem_UnitR* headR; // offset 0x14, size 0x4
} loadBgMem_LoadCtrl;

// total size: 0x1101C
typedef struct D {
    // Members
    struct _loadBgMem_LoadCtrl ctrl; // offset 0x0, size 0x18
    struct _loadBgMem_UnitL UnitLArray[1024]; // offset 0x18, size 0x4000
    struct _loadBgMem_UnitR UnitRArray[1024]; // offset 0x4018, size 0x4000
    struct _loadBgMem_CacheCtrl cache; // offset 0x8018, size 0x18
    struct _loadBgMem_UnitC UnitCArray[4]; // offset 0x8030, size 0x40
    char c_buf[32768]; // offset 0x8070, size 0x8000
    struct loadBgMem_Sect* SectList[101]; // offset 0x10070, size 0x194
    struct loadBgMem_Sect* infosect[2]; // offset 0x10204, size 0x8
    struct loadBgMem_Sect SectListBuf[100]; // offset 0x1020C, size 0x7D0
    struct loadBgMem_File filelist[100]; // offset 0x109DC, size 0x640
} D;

int _loadBgMem_CmpUnitLR(loadBgMem_UnitL* UnitL, loadBgMem_UnitR* UnitR);
int _loadBgMem_CheckUnitC(loadBgMem_UnitC* UnitC, fsFileIndex* file, u_short unitID);
int _loadBgMem_SyncUnitL(loadBgMem_UnitL* UnitL);
int _loadBgMem_ClearUnitL(loadBgMem_UnitL* UnitL);
int _loadBgMem_ClearUnitC(loadBgMem_UnitC* UnitC);
int _loadBgMem_AddUnitC(loadBgMem_UnitC* UnitC, loadBgMem_UnitC* prevC);
void _loadBgMem_TakeoutUnitC(loadBgMem_CacheCtrl* cache, loadBgMem_UnitC* UnitC);
void _loadBgMem_ShiftUnitC(loadBgMem_CacheCtrl* cache, loadBgMem_UnitC* UnitC);
void _loadBgMem_PushUnitC(loadBgMem_CacheCtrl* cache, loadBgMem_UnitC* UnitC);
loadBgMem_UnitC* _loadBgMem_SearchOldestUnitC(loadBgMem_CacheCtrl* cache);
loadBgMem_UnitC* _loadBgMem_SearchUnitC(loadBgMem_CacheCtrl* cache, fsFileIndex* file, u_short unitID);
loadBgMem_UnitC* _loadBgMem_SearchUnitR2C(loadBgMem_CacheCtrl* cache, loadBgMem_UnitR* UnitR);
loadBgMem_UnitC* _loadBgMem_SearchUnitL2C(loadBgMem_CacheCtrl* cache, loadBgMem_UnitL* UnitL);
void _loadBgMem_MoveMemC2L(loadBgMem_LoadCtrl* ctrl, loadBgMem_CacheCtrl* cache, loadBgMem_UnitL* UnitL, loadBgMem_UnitC* UnitC);
void _loadBgMem_MoveMemL2C(loadBgMem_LoadCtrl* ctrl, loadBgMem_CacheCtrl* cache, loadBgMem_UnitL* UnitL, loadBgMem_UnitC* UnitC);
void _loadBgMem_SwapMemLC(loadBgMem_LoadCtrl* ctrl, loadBgMem_CacheCtrl* cache, loadBgMem_UnitL* UnitL, loadBgMem_UnitC* UnitC);
int _loadBgMem_LoadMemR2L(loadBgMem_LoadCtrl* ctrl, loadBgMem_UnitL* UnitL, loadBgMem_UnitR* UnitR);
int _loadBgMem_InitLoad(loadBgMem_LoadCtrl* ctrl, int UnitSize, int Units, loadBgMem_UnitL* UnitLArray, loadBgMem_UnitR* UnitRArray, char* Buffer);
int _loadBgMem_InitCache(loadBgMem_CacheCtrl* cache, int UnitSize, int Units, loadBgMem_UnitC* UnitCArray, char* Buffer);
int _loadBgMem_ClearRequest(loadBgMem_LoadCtrl* ctrl);
int loadBgMem_SetRequest(loadBgMem_LoadCtrl* ctrl, loadBgMem_Sect** SectList);
int _loadBgMem_SyncLoadUnits(loadBgMem_LoadCtrl* ctrl);
int loadBgMem_CheckRequest(loadBgMem_LoadCtrl* ctrl, int* reqUnits);
int _loadBgMem_CleanupNonRequest(loadBgMem_LoadCtrl* ctrl);
int loadBgMem_LoadRequest(loadBgMem_LoadCtrl* ctrl, loadBgMem_CacheCtrl* cache, int cache_access_limit, int file_access_limit, int* cache_in_access_count, int* cache_out_access_count, int* file_access_count, int* miss_access_count);

extern D d;

#endif // LOADBG_MEM_H
