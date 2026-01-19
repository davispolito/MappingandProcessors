//
// Created by Joshua Warner on 6/13/24.
//
#ifndef VCAERMODULE_H
#define VCAERMODULE_H

#include "leaf.h"
#include "defs.h"
#include "leaf-mempool.h"



typedef enum {
    VCAEventWatchFlag,
    VCAGain,
    VCARouting,
    VCAAudioInput,
    VCANumParams
} VCAParams;





typedef struct _tVCAModule {
    ModuleHeader header;
    void* theVCA;

    float amp;
    float external_input;

    tMempool* mempool;
} _tVCAModule;

typedef _tVCAModule* tVCAModule;

//init module
void tVCAModule_init(void** const VCA, float* const params, float id, LEAF* const leaf);
void tVCAModule_initToPool(void** const VCA, float* const params, float id, tMempool** const mempool);
void tVCAModule_free(void** const VCA);

// tick
void tVCAModule_tick (tVCAModule const VCA, float*);

void tVCAModule_setAudio(tVCAModule const VCA, float audio);
void tVCAModule_setGain(tVCAModule const VCA, float gain);

// Non-modulatable setters



#endif //VCAERMODULE_H
