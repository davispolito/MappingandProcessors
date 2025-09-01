//
// Created by Joshua Warner on 6/13/24.
//
#ifndef VCAERMODULE_H
#define VCAERMODULE_H

#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "Inc/leaf-mempool.h"



typedef enum {
    VCAEventWatchFlag,
    VCAGain,
    VCARouting,
    VCAAudioInput,
    VCANumParams
} VCAParams;





typedef struct _tVCAModule {
    void* theVCA;
    uint32_t moduleType;
    uint32_t uniqueID;
    tTickFuncFloatInReturningFloat tick; // The object's tick function

    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions

    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    ATOMIC_FLOAT inputs[1];
    float amp;
    float external_input;

    tMempool mempool;
} _tVCAModule;

typedef _tVCAModule* tVCAModule;

//init module
void tVCAModule_init(void** const VCA, float* const params, float id, LEAF* const leaf);
void tVCAModule_initToPool(void** const VCA, float* const params, float id, tMempool* const mempool);
void tVCAModule_free(void** const VCA);

// tick
void tVCAModule_tick (tVCAModule const VCA, float*);

void tVCAModule_setAudio(tVCAModule const VCA, float audio);
void tVCAModule_setGain(tVCAModule const VCA, float gain);

// Non-modulatable setters
//init processors
// void tVCAModule_processorInit(tVCAModule const VCA, LEAF_NAMESPACE tProcessor* processor);



#endif //VCAERMODULE_H
