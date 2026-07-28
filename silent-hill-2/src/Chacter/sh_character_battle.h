#ifndef SH_CHARACTER_BATTLE_H
#define SH_CHARACTER_BATTLE_H

#include "common.h"
#include "Chacter/character.h"
#include "Chacter/sh_character_status.h"

typedef struct shBattleShot {
    float test_a; // offset 0x0, size 0x4
    float test_b; // offset 0x4, size 0x4
    float test_c; // offset 0x8, size 0x4
} shBattleShot;

typedef struct shBattleFight {
    float test_a; // offset 0x0, size 0x4
    float test_b; // offset 0x4, size 0x4
    float test_c; // offset 0x8, size 0x4
} shBattleFight;

typedef struct shAttackInfo {
    u_short id;      // offset 0x0, size 0x2
    u_short kind;    // offset 0x2, size 0x2
    float ap;        // offset 0x4, size 0x4
    float sp;        // offset 0x8, size 0x4
    float min_range; // offset 0xC, size 0x4
    float max_range; // offset 0x10, size 0x4
    union {
        shBattleFight fight; // offset 0x0, size 0xC
        shBattleShot shot;   // offset 0x0, size 0xC
    } hit_check;             // offset 0x14, size 0xC
    u_char atk_start;        // offset 0x20, size 0x1
    u_char atk_end;          // offset 0x21, size 0x1
    u_char sd;               // offset 0x22, size 0x1
    u_char eff;              // offset 0x23, size 0x1
} shAttackInfo;

// total size: 0x8
typedef struct shAttackQueueOne {
    // Members
    SubCharacter* scp; // offset 0x0, size 0x4
    u_char wep_no;            // offset 0x4, size 0x1
    u_short atk_no;           // offset 0x6, size 0x2
} shAttackQueueOne;

// total size: 0xA4
typedef struct shAttackQueue {
    // Members
    int rest;                          // offset 0x0, size 0x4
    struct shAttackQueueOne queue[20]; // offset 0x4, size 0xA0
} shAttackQueue;

void shGetHumanAttackSprayPos(int i, float* s_pos, float* s_vec, float* result);

void shBattleAttackHitCheckInit(SubCharacter* scp);

void shBattleAttackHitCheckToEnemy(SubCharacter* scp, u_char wep_no, u_short atk_no);

void shBattleAttackHitCheckToHuman(SubCharacter* scp, u_short atk_no);

int shBattleRequestNextAttackIsOk(u_short atk, u_short frame);

void shBattleGetResult(SubCharacter* scp);

void shBattleInitAttackQueue(void);

void shBattleExecAttackQueue(void);

float shBattleGetJamesHP(void);

float shBattleGetJamesHP_Rate(void);

void shBattleSetJamesDamage(u_short id, float damage, float* vec);

float shBattleEventWallHitCheck(void);

int shBattleCheckAttackByEnemy(void);

#endif
