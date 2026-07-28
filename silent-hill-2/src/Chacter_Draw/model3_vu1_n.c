#include "sh2_common.h"

#include "sce/libdma.h"
#include "Chacter_Draw/model_common.h"
#include "GFW/sh2gfw_Init_ModelDrawData.h"

#include "Chacter/character.h"

#include "Chacter_Draw/model3_n.h"
#include "Chacter_Draw/model3_sub_n.h"
#include "Chacter_Draw/model_common.h"
#include "Chacter_Draw/model3_vu1_n.h"
#include "Chacter_Draw/light_n.h"
#include "Chacter_Draw/model3_structs.h"

#include "sce/libvifpk.h"

extern SprData* spr_data;
extern u_int xitop_0x0041BF08;
extern u_int prev_xtop;

#define xitop xitop_0x0041BF08

#define SHADING_TYPE_1            1
#define SHADING_TYPE_LAMBERTIAN_2 2
#define SHADING_TYPE_LAMBERTIAN_3 3
#define SHADING_TYPE_LAMBERTIAN_4 4

static void InitAllDataOne(AllData* p);
static void InitSprData(SprData* p);
static void InitData1(void);
static void MakeData1(void);
static void InitEnv1(sceVif1Packet* pk, int unused);
static void TiniEnv(sceVif1Packet* pk);
static void MakeVu1PartTransferPacket(Part* part, sceVif1Packet* pk);
static void MakeLambertShadingPacket_VU1(Part* part, sceVif1Packet* pk);
static void MakeNormalPacket(Part* part, sceVif1Packet* pk);
static void MakeEnvironPacket(Part* part, sceVif1Packet* pk);
static void MakeSpecularPacket(Part* part, sceVif1Packet* pk);
static void MakeBaseSpecularPacket(Part* part, sceVif1Packet* pk);
static void MakeOverPacket(Part* part, sceVif1Packet* pk);
static void MakeDrawPacket(Part* part, sceVif1Packet* pk);
static void DrawPart1(Part* part, sceVif1Packet* pk);
static void DrawParts1(sh_Model* model, ModelWork* work);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", Model3LoadMpg1);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", InitAllDataOne);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", InitSprData);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", InitData1);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeData1);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", InitEnv1);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", TiniEnv);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeVu1PartTransferPacket);

#ifdef NON_MATCHING
static void MakeLambertShadingPacket(Part* part /* r20 */, sceVif1Packet* pk /* r19 */) {
    int n_parallels = LightNValidParallelMatrices(); // r16
    int n_extras = LightNValidExtras(); // r17
    int i; // r18
    Light* light; // r16
    float brightness; // r20
    Data* lf_data; // r16

    for (i = 1; i < n_parallels; i++) {
        sceVif1PkRef(pk, (u_long128* ) &all_data->plight[i], 8, SCE_VIF1_SET_STCYCL(1, 1, 0), SCE_VIF1_SET_UNPACK(xitop, 8, SCE_VIF_UPK_V4_32, 0), 0);
        sceVif1PkCnt(pk, 0);
        sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(xitop, 0));
        sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(8, 0));
        xitop_0x0041BF08 ^= (1 << 9);
    }

    for (i = 0; i < n_extras; i++) {
        light = LightNthValidExtra(i);
        sceVif1PkRef(pk, (u_long128* ) &all_data->elight[i], 4, SCE_VIF1_SET_STCYCL(1, 1, 0), SCE_VIF1_SET_UNPACK(xitop, 4, SCE_VIF_UPK_V4_32, 0), 0);
        sceVif1PkCnt(pk, 0);
        sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(xitop, 0));

        switch (light->kind) {
            case 2:
                sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(10, 0));
                break;

            case 3:
                sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(12, 0));
                break;

            default:
                ASSERT_ON_LINE(0, 830);
        }
        
        xitop ^= (1 << 9);
    }

    brightness = LightReflectionBrightness();
    sceVif1PkRef(pk, (u_long128* ) all_data, 10, SCE_VIF1_SET_STCYCL(1, 1, 0), SCE_VIF1_SET_UNPACK(xitop, 10, SCE_VIF_UPK_V4_32, 0), 0);
    sceVif1PkCnt(pk, 0);
    sceVif1PkAddCode(pk, SCE_VIF1_SET_STCYCL(1, 1, 0));
    sceVif1PkAddCode(pk, SCE_VIF1_SET_UNPACK(xitop + 10, 3, SCE_VIF_UPK_V4_32, 0));
    lf_data = sceVif1PkReserve(pk, 12);

    ASSERT_ON_LINE(((u_int)lf_data & 0x03) == 0, 885);

    sceVu0CopyVector(lf_data->diffuse, part->diffuse);
    lf_data->diffuse[3] = 1.0f;

    sceVu0CopyVector(lf_data->ambient, part->ambient);
    lf_data->ambient[3] = 1.0f;

    lf_data->param[2] = part->phong_param_a * brightness;
    lf_data->param[3] = part->phong_param_b * brightness;

    sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(xitop, 0));
    sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(14, 0));

    xitop ^= (1 << 9);
}
#else
INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeLambertShadingPacket_VU1);
#endif

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeNormalPacket);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeEnvironPacket);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeSpecularPacket);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeBaseSpecularPacket);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeOverPacket);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", MakeDrawPacket);

#ifdef NON_MATCHING
static void DrawPart1(Part* part /* r17 */, sceVif1Packet* pk /* r16 */) {
    Data* data; // r2

    xitop = 496;

    if (part->xtop == prev_xtop) {
        sceVif1PkCnt(pk, 0);
        sceVif1PkAddCode(pk, SCE_VIF1_SET_FLUSH(0));
    }

    prev_xtop = part->xtop;

    MakeVu1PartTransferPacket(part, pk);
    sceVif1PkCnt(pk, 0);
    sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(part->xtop, 0));
    sceVif1PkAddCode(pk, SCE_VIF1_SET_FLUSH(0));
    sceVif1PkAddCode(pk, SCE_VIF1_SET_UNPACK(xitop, 1, SCE_VIF_UPK_V4_32, 0));

    if (part->shading_type == SHADING_TYPE_1) {
        sceVif1PkAddData(pk, reinterpret_as_u_int(part->diffuse[0]));
        sceVif1PkAddData(pk, reinterpret_as_u_int(part->diffuse[1]));
        sceVif1PkAddData(pk, reinterpret_as_u_int(part->diffuse[2]));
        sceVif1PkAddData(pk, reinterpret_as_u_int(128.0f));
    } else {
        sceVif1PkAddData(pk, 0);
        sceVif1PkAddData(pk, 0);
        sceVif1PkAddData(pk, 0);
        sceVif1PkAddData(pk, 0);
    }

    sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(6, 0));
    xitop ^= (1 << 9);

    switch (part->shading_type) {
        case SHADING_TYPE_1:
            break;

        case SHADING_TYPE_LAMBERTIAN_2:
        case SHADING_TYPE_LAMBERTIAN_3:
        case SHADING_TYPE_LAMBERTIAN_4:
            MakeLambertShadingPacket_VU1(part, pk);
            break;

        default:
            ASSERT_ON_LINE(0, 1208);
            return;
    }

    if (part->envmap_param) {
        sceVif1PkRef(pk, (u_long128*) &all_data->emap, 6, SCE_VIF1_SET_STCYCL(1, 1, 0), SCE_VIF1_SET_UNPACK(xitop, 6, SCE_VIF_UPK_V4_32, 0), 0);
        sceVif1PkCnt(pk, 0);
        sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(xitop, 0));
        sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(16, 0));
        xitop ^= (1 << 9);
    }

    if (part->shading_type == SHADING_TYPE_LAMBERTIAN_4) {
        sceVif1PkRef(pk, (u_long128*) &all_data->smap, 4, SCE_VIF1_SET_STCYCL(1, 1, 0), SCE_VIF1_SET_UNPACK(xitop, 4, SCE_VIF_UPK_V4_32, 0), 0);
        sceVif1PkCnt(pk, 0);
        sceVif1PkAddCode(pk, SCE_VIF1_SET_STCYCL(1, 1, 0));
        sceVif1PkAddCode(pk, SCE_VIF1_SET_UNPACK(xitop + 4, 1, SCE_VIF_UPK_V4_32, 0));

        data = sceVif1PkReserve(pk, 4);
        ASSERT_ON_LINE(((u_int)data & 0x03) == 0, 1238);

        data->diffuse[3] = part->blinn_param;
        sceVif1PkAddCode(pk, SCE_VIF1_SET_ITOP(xitop, 0));
        sceVif1PkAddCode(pk, SCE_VIF1_SET_MSCAL(18, 0));
        xitop ^= (1 << 9);
    }

    MakeDrawPacket(part, pk);
}
#else
INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", DrawPart1);
#endif

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", DrawParts1);

INCLUDE_ASM("asm/nonmatchings/Chacter_Draw/model3_vu1_n", Model3DrawVu1Parts);


INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @873_0x0038D9B0);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1102_0x0038D9D0);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1103_0x0038D9F8);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1104_0x0038DA00);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1105_0x0038DA30);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @22);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @23);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @39);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1171_0x0038DAD0);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1190);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1216_0x0038DB30);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1290);

INCLUDE_RODATA("asm/nonmatchings/Chacter_Draw/model3_vu1_n", @1291);

