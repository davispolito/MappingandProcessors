
//
// Created by Jeffrey Snyder on 7/9/24.
//

#pragma once
#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "leaf-mempool.h"
#include "leaf-envelopes.h"

#define EXP_BUFFER_SIZE 2048
#define DECAY_EXP_BUFFER_SIZE 2048
typedef enum {
    EnvEventWatchFlag, //all of them need this
    EnvAttack,
    EnvDecay,
    EnvSustain,
    EnvRelease,
    EnvLeak,
    EnvShapeAttack,
    EnvShapeRelease,
    EnvUseVelocity,
    EnvNumParams //all of them need this
} EnvParams;



typedef struct _tEnvModule {
    //start boilerplate  - processor represents all of these///
    ModuleHeader header;
    //if its a combo/multi object put them all here
    tADSRT theEnv;
    //end boilerplate  - processor represents all of these///
    //specific other variables
    const float* envTimeTableAddress;
    float envTimeTableSizeMinusOne;
    uint32_t tableSize;
    float expBuffer[EXP_BUFFER_SIZE];
    float expBufferSizeMinusOne;

    float decayExpBuffer[DECAY_EXP_BUFFER_SIZE];
    float decayExpBufferSizeMinusOne;
    //mempool
    tMempool* mempool;
} _tEnvModule;

typedef _tEnvModule* tEnvModule;

//init module
void tEnvModule_init(void** const env, float* const params, float id, LEAF* const leaf);
void tEnvModule_initToPool(void** const env, float* const params, float id, tMempool** const mempool);
void tEnvModule_free(void** const env);
void tEnvModule_tick (tEnvModule const env);

//note on action
void tEnvModule_onNoteOn(tEnvModule const env, float vel);

// Modulatable setters
void tEnvModule_setParameter(tEnvModule const env, int parameter_id, float input);

// Non-modulatable setters
void tEnvModule_setRateTableLocation (tEnvModule const env, float* tableAddress);
void tEnvModule_setSampleRate (tEnvModule const env, float sr);


