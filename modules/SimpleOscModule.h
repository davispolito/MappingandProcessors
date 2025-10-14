//
// Created by Joshua Warner on 6/13/24.
//

#ifndef SIMPLEOSCMODULE_H
#define SIMPLEOSCMODULE_H
#include "leaf.h"
#include "defs.h"
// #include "processor.h"
#include "Inc/leaf-mempool.h"
#include "Inc/leaf-oscillators.h"
#include "processor.h"

typedef enum {
    OscEventWatchFlag,
    OscMidiPitch,
    OscHarmonic,
    OscPitchOffset,
    OscPitchFine,
    OscFreqOffset,
    OscShapeParam,
    OscAmpParam,
    OscGlide,
    OscSteppedHarmonic,
    OscSteppedPitch,
    OscSyncMode,
    OscSyncIn,
    OscType,
    OscNumParams
} OscParams;

typedef enum {
    OscTypeSawSquare,
    OscTypeSineTri,
    OscTypeSaw,
    OscTypePulse,
    OscTypeSine,
    OscTypeTri,
    OscNumTypes
} OscTypes;

//the actual frequency setter function
typedef void (*tFreqSetFunc)(void*, float);

typedef struct _tOscModule {
    uint32_t moduleType;
    void* theOsc;
    uint32_t uniqueID;
    ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
    ATOMIC_FLOAT outputs[1];

    uint32_t osctype;

//    tTickFuncReturningFloat tick; // The object's tick function
//    tFreqSetFunc freq_set_func;

//    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions

    float fine;
    float harmonicMultiplier;
    float pitchOffset;
    float freqOffset;
    tExpSmooth pitchSmoother;
    float octaveOffset;
    float inputNote;
    float finalFreq;
    float amp;
    float* mtofTable;
    float sr;
    float invSr;
    int hStepped;
    int pStepped;
    int syncMode;

    tMempool* mempool;
} _tOscModule;

typedef _tOscModule* tOscModule;

//init module
void tOscModule_init(void** const osc, float* const params, float id, LEAF* const leaf);
void tOscModule_initToPool(void** const osc, float* const params, float id, tMempool** const mempool);
void tOscModule_free(void** const osc);
void tOscModule_setParameter(tOscModule const osc, OscParams param_type,float input);
// Modulatable setters
void tOscModule_tick (tOscModule const osc, float*);


// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* tableAddress);
void tOscModule_setSampleRate (tOscModule const osc, float sr);

//init processors
void tOscModule_processorInit(tOscModule const osc, LEAF_NAMESPACE tProcessor* processor);

#endif //SIMPLEOSCMODULE_H
