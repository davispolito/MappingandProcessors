//
// Created by Jeff Snyder on 1/7/26.
//

#ifndef SOFTCLIPMODULE_H
#define SOFTCLIPMODULE_H

//
// Created by Jeffrey Snyder on 7/9/24.
//

#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "leaf-mempool.h"
#include "leaf-filters.h"
#include "processor.h"




typedef enum {
    SoftClipEventWatchFlag,
    SoftClipInputGain,
    SoftClipOffset,
    SoftClipShape,
    SoftClipNumParams
} SoftClipModuleParams;



typedef struct _tSoftClipModule {
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncReturningFloat tick; // The object's tick function
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    ATOMIC_FLOAT inputs[1];
    float shapeDivider;
    float inputGain;
    float offset;
    tHighpass highpass;

    tMempool* mempool;
} _tSoftClipModule;

typedef _tSoftClipModule* tSoftClipModule;

//init module
void tSoftClipModule_init(void** const c, float* const params, float id, LEAF* const leaf);
void tSoftClipModule_initToPool(void** const c, float* const params, float id, tMempool** const mempool);
void tSoftClipModule_free(void** const c);

void tSoftClipModule_tick (tSoftClipModule const c, float*);

// Modulatable setters
// void tEnvModule_setRate (tEnvModule const env, float rate);
void tSoftClipModule_setParameter(tSoftClipModule const c, int parameter_id, float input);
// Non-modulatable setters


//init processors
void tSoftClipModule_processorInit(tSoftClipModule const c, LEAF_NAMESPACE  tProcessor* processor);



#endif //SOFTCLIPMODULE_H
