//
// Created by Joshua Warner on 6/13/24.
//

#ifndef LFOMODULE_H
#define LFOMODULE_H

#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "leaf-mempool.h"
#include "leaf-oscillators.h"

typedef void (*tFreqSetFunc)(void*, float);
typedef enum {
    LFOEventWatchFlag,
    LFORateParam,
    LFOShapeParam,
    LFOPhaseParam,
    LFOType,//non modulatable
    LFONumParams
} LFOParams;

typedef enum {
    LFOTypeSineTri,
    LFOTypeSawSquare,
    LFOTypeSine,
    LFOTypeTri,
    LFOTypeSaw,
    LFOTypeSquare,
    LFONumTypes
} LFOTypes;

typedef struct _tLFOModule {
    ModuleHeader header;
   void* theLFO;
    uint32_t lfo_type;

    // tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions


    tLookupTable* table;
    tFreqSetFunc freq_set_func;
   tMempool* mempool;
} _tLFOModule;

typedef _tLFOModule* tLFOModule;

//init module
void tLFOModule_init(void** const lfo, float* const params, float id, LEAF* const leaf);
void tLFOModule_initToPool(void** const lfo, float* const params, float id, tMempool** const mempool, tLookupTable* );
void tLFOModule_free(void** const lfo);

//note on action
void tLFOModule_onNoteOn(tLFOModule const lfo, float pitch, float velocity);

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate);

void tLFOModule_setParameter(tLFOModule const, LFOParams param_type, float input);

// Non-modulatable setters
//void tLFOModule_setRateTableLocationAndSize (tLFOModule const lfo, float* tableAddress, uint32_t size);
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr);



#endif //LFOMODULE_H
