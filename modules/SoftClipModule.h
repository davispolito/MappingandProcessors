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




typedef enum {
    SoftClipEventWatchFlag,
    SoftClipInputGain,
    SoftClipOffset,
    SoftClipShape,
    SoftClipNumParams
} SoftClipModuleParams;



typedef struct _tSoftClipModule {
    ModuleHeader header;
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



#endif //SOFTCLIPMODULE_H
