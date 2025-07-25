//
// Created by Joshua Warner on 6/13/24.
//
#ifndef FILTERMODULE_H
#define FILTERMODULE_H

#include "leaf.h"
#include "defs.h"
#include "processor.h"
#include "Inc/leaf-mempool.h"
#include "Inc/leaf-filters.h"


typedef enum {
    FiltEventWatchFlag,
    FiltMidiPitch,
    FiltCutoff,
    FiltGain,
    FiltResonance,
    FiltKeyfollow,
    FiltType,
    FiltNumParams
} FiltParams;

typedef enum {
    FiltTypeLowpass,
    FiltTypeHighpass,
    FiltTypeBandpass,
    FiltTypeDiodeLowpass,
    FiltTypePeak,
    FiltTypeHighShelf,
    FiltTypeLowShelf,
    FiltTypeNotch,
    FiltTypeLadderLowpass,
    FiltNumTypes
} FiltTypes;

//the actual frequency setter function
typedef void (*tFiltInternalParamSetFunc)(void*, float);

typedef struct _tFiltModule {
    void* theFilt;
    uint32_t moduleType;
    uint32_t uniqueID;
    float* dbTableAddress;
    uint32_t dbTableScalar;
    float dbTableOffset;
    float dbTableSizeMinusOne;
    float* resTableAddress;
    float resTableSizeMinusOne;
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];
    ATOMIC_FLOAT inputs[1];
    uint32_t filtType;
    float amp;
    float cutoffKnob;
    float keyFollow;
    float inputNote;
    float sr;
    float invSr;
    tMempool mempool;
} _tFiltModule;

typedef _tFiltModule* tFiltModule;

//init module
void tFiltModule_init(void** const filt, float* const params, float id, LEAF* const leaf);
void tFiltModule_initToPool(void** const filt, float* const params, float id, tMempool* const mempool);
void tFiltModule_free(void** const filt);

// tick
void tFiltModule_tick (tFiltModule const filt, float*);


void tFiltModule_setParameter(tFiltModule const filt, FiltParams param_type,float input);

//Modulatable setters
void tFiltModule_setMIDIPitch (tFiltModule const filt, float const input);
void tFiltModule_setCutoff(tFiltModule const filt, float cutoff);

// Non-modulatable setters

void tFiltModule_setDBtoATableLocation (tFiltModule const filt, float* tableAddress, uint32_t tableSize);
void tFiltModule_setResTableLocation (tFiltModule const filt, float* tableAddress, uint32_t tableSize);
void tFiltModule_setSampleRate (tFiltModule const filt, float sr);

//init processors
void tFiltModule_processorInit(tFiltModule const filt, LEAF_NAMESPACE tProcessor* processor);



#endif //FILTERMODULE_H
