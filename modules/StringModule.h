//
// Created by Davis Polito on 4/29/25.
//

#ifndef STRINGMODULE_H
#define STRINGMODULE_H
#include "defs.h"
#include "leaf-mempool.h"
#include "leaf-physical.h"
#include "leaf.h"
#include "processor.h"

typedef enum {
    StringEventWatchFlag,
    StringOversample,
    StringFreq,
    StringWaveLength,
    StringDampFreq,
    StringDecay,
    StringTargetLevel,
    StringLevelSmooth,
    StringLevelStrength,
    StringPickupPoint,
    StringLevelMode,
    StringRippleGain,
    StringRippleDelay,
    StringPluckPosition,
    StringNumParams
} StringModelParams;




typedef struct _tStringModule {
    uint32_t moduleType;
    tSimpleLivingString3 *theString;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    tMempool* mempool;
} _tStringModule;

typedef _tStringModule* tStringModule;
void tStringModule_tick (tStringModule const filt, float*);
//init module
void tStringModule_init(void** const module, float* const params, float id, LEAF* const leaf);
void tStringModule_initToPool(void** const module, float* const params, float id, tMempool** const mempool);
void tStringModule_free(void** const env);
//init processors
// Non-modulatable setters for tStringModule
void tStringModule_setParameter(tStringModule const s, StringModelParams param, float input);


void tStringModule_processorInit(tStringModule const filt, LEAF_NAMESPACE tProcessor* processor);

#endif //STRINGMODULE_H
