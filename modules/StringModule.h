//
// Created by Davis Polito on 4/29/25.
//

#ifndef STRINGMODULE_H
#define STRINGMODULE_H
#include "defs.h"
#include "Inc/leaf-mempool.h"
#include "Inc/leaf-physical.h"
#include "leaf.h"

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
    tSimpleLivingString3 theString;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    tMempool mempool;
} _tStringModule;

typedef _tStringModule* tStringModule;
void tStringModule_tick (tStringModule const filt, float*);
//init module
void tStringModule_init(void** const module, float* const params, float id, LEAF* const leaf);
void tStringModule_initToPool(void** const module, float* const params, float id, tMempool* const mempool);
void tStringModule_free(void** const env);
//init processors
// Non-modulatable setters for tStringModule
void tStringModule_setEventWatchFlag (tStringModule const s, float value);
void tStringModule_setOversample      (tStringModule const s, int value);
void tStringModule_setFreq            (tStringModule const s, float value);
void tStringModule_setWaveLength      (tStringModule const s, float value);
void tStringModule_setDampFreq        (tStringModule const s, float value);
void tStringModule_setDecay           (tStringModule const s, float value);
void tStringModule_setTargetLevel     (tStringModule const s, float value);
void tStringModule_setLevelStrength     (tStringModule const s, float value);
void tStringModule_setLevelSmooth     (tStringModule const s, float value);
void tStringModule_setPickupPoint     (tStringModule const s, float value);
void tStringModule_setLevelMode       (tStringModule const s, int value);
void tStringModule_setRippleGain      (tStringModule const s, float value);
void tStringModule_setRippleDelay     (tStringModule const s, float value);
void tStringModule_setPluckPosition   (tStringModule const s, float value);
#endif //STRINGMODULE_H
