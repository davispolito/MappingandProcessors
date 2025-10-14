//
// Created by Jeffrey Snyder on 7/31/24.
//
//
// Created by Joshua Warner on 6/13/24.
//

#include "VCAModule.h"

#include <assert.h>

void tVCAModule_init(void** const VCA, float* params, float id, LEAF* const leaf)
{
    tVCAModule_initToPool(VCA, params, id, &leaf->mempool);
}

void tVCAModule_blankFunction (tVCAModule const VCA, float freq)
{
    ;
}



void tVCAModule_initToPool(void** const VCA, float* const params, float id, tMempool** const mempool)
{
    tMempool* m = *mempool;
    _tVCAModule* VCAModule =static_cast<_tVCAModule *> ( *VCA = (_tVCAModule*) mpool_alloc(sizeof(_tVCAModule), m));
#ifndef __cplusplus
    memcpy(VCAModule->params, params, VCANumParams*sizeof(float));

#endif
    VCAModule->uniqueID = id;
    //CPPDEREF VCAModule->params[VCAAudioInput] = 0.0f;
    int type = 0.0f;//roundf(VCAModule->params[VCAType]);
    VCAModule->mempool = m;
    VCAModule->amp = 1.0f;
    VCAModule->tick = nullptr;//&tVCAModule_tick;
    VCAModule->moduleType = ModuleTypeVCAModule;
    VCAModule->external_input = 0;
    VCAModule->setterFunctions[VCAGain] = (tSetter)&(*tVCAModule_setGain);
    VCAModule->setterFunctions[VCAAudioInput] = (tSetter)&(*tVCAModule_setAudio);

}


void tVCAModule_free(void** const VCA)
{
    _tVCAModule* VCAModule =(_tVCAModule*) *VCA;
    mpool_free((char*)VCAModule, VCAModule->mempool);
}

void tVCAModule_setAudio(tVCAModule const VCA, float audio) {
    VCA->external_input = audio;
}
void tVCAModule_setGain(tVCAModule const VCA, float gain) {
    VCA->amp = gain;
}

// tick function
void tVCAModule_tick (tVCAModule const VCA, float* buffer)
{
    buffer[0] = VCA->outputs[0] = (*buffer   + VCA->external_input)*VCA->amp;
}

// Modulatable setters




// Non-modulatable setters

//be sure to set the tables before initing the processor
void tVCAModule_processorInit(tVCAModule const VCA, leaf::tProcessor* const processor)
{
    // Checks that arguments are valid
	if (VCA == NULL)
	{
		return;
	}
	if (processor == NULL)
	{
		return;
	}

    processor->processorUniqueID = VCA->uniqueID;
    processor->object = VCA;
    processor->numSetterFunctions = VCANumParams;
    processor->tick = (tTickFuncReturningVoid ) &tVCAModule_tick;

    // processor->setterFunctions[VCAGain] = (tSetter)&(*VCA->setterFunctions[VCAGain]);
    // processor->setterFunctions[VCAAudioInput] = (tSetter)&(*VCA->setterFunctions[VCAAudioInput]);

//    for (int i = 0; i < VCANumParams; i++)
//    {
//        processor->setterFunctions[i](VCA, VCA->params[i]);
//    }
    processor->inParameters = VCA->params;
    processor->outParameters = VCA->outputs;
    processor->audioInParameters = VCA->inputs;
    processor->processorTypeID = ModuleTypeVCAModule;
}



