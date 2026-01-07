
//
// Created by Jeffrey Snyder on 7/9/24.
//

#pragma once
#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "leaf-mempool.h"
#include "leaf-envelopes.h"
#include "processor.h"


typedef enum {
    EnvEventWatchFlag,
    EnvAttack,
    EnvDecay,
    EnvSustain,
    EnvRelease,
    EnvLeak,
    EnvShapeAttack,
    EnvShapeRelease,
    EnvUseVelocity,
    EnvNumParams
} EnvParams;



typedef struct _tEnvModule {
    uint32_t moduleType;
    tADSRT theEnv;

    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    const float* envTimeTableAddress;
    float envTimeTableSizeMinusOne;
    uint32_t tableSize;
    tMempool* mempool;
} _tEnvModule;

typedef _tEnvModule* tEnvModule;

//init module
void tEnvModule_init(void** const env, float* const params, float id, LEAF* const leaf);
void tEnvModule_initToPool(void** const env, float* const params, float id, tMempool** const mempool);
void tEnvModule_free(void** const env);

//note on action
void tEnvModule_onNoteOn(tEnvModule const env, float vel);

// Modulatable setters
// void tEnvModule_setRate (tEnvModule const env, float rate);
void tEnvModule_setParameter(tEnvModule const env, int parameter_id, float input);
// Non-modulatable setters
void tEnvModule_setRateTableLocation (tEnvModule const env, float* tableAddress);
void tEnvModule_setSampleRate (tEnvModule const env, float sr);

//init processors
void tEnvModule_processorInit(tEnvModule const env, LEAF_NAMESPACE  tProcessor* processor);


