//
// Created by Jeffrey Snyder on 7/9/24.
//


#include "SoftClipModule.h"

#include <assert.h>


void tSoftClipModule_init(void** const c, float* params, float id, LEAF* const leaf)
{
    tSoftClipModule_initToPool(c, params, id, &leaf->mempool);
}


void tSoftClipModule_free(void** const c)
{
    _tSoftClipModule* SoftClipModule = static_cast<_tSoftClipModule *>(*c);

    mpool_free((char*)SoftClipModule, SoftClipModule->mempool);
}
//tick function
void tSoftClipModule_tick (tSoftClipModule const c, float* buffer)
{
    float sample = buffer[0];
    sample = sample * c->inputGain * 5.0f;
    sample = sample + (c->offset * 2.0f) - 1.0f;

    if (sample <= -1.0f)
    {
        sample = -1.0f;
    } else if (sample >= 1.0f)
    {
        sample = 1.0f;
    }
    {
        sample = 1.5f * (sample) - (((sample * sample * sample))* 0.3333333f);
        sample = sample * c->shapeDivider;
    }

    sample = tHighpass_tick(&c->highpass, sample);
    buffer[0] = sample;

}


void tSoftClipModule_setParameter(tSoftClipModule const  c, int parameter_id, float input)
{
    switch (parameter_id)
    {
        case SoftClipEventWatchFlag:
        {
            // handled by onnoteon listener
            break;
        }

        case SoftClipInputGain:
        {
            c->inputGain = input;
            break;
        }

        case SoftClipOffset:
        {
            c->offset = input;
            break;
        }

        case SoftClipShape:
        {
            float shape = (input * .99f) + 0.01f;
            c->shapeDivider = 1.0f / (shape - ((shape*shape*shape) * 0.3333333f));
            break;
        }

        case SoftClipOutputGain:
        {
            c->outputGain = input;
            break;
        }

        default:
            break;
    }
}


void tSoftClipModule_initToPool(void** const c, float* const params, float id, tMempool** const mempool)
{
    tMempool* m = *mempool;
    _tSoftClipModule* SoftClipModule = static_cast<_tSoftClipModule *>(*c = (_tSoftClipModule*) mpool_alloc(sizeof(_tSoftClipModule), m));
#ifndef __cplusplus
    memcpy(SoftClipModule->params, params, SoftClipNumParams);
#endif __cplusplus
    SoftClipModule->mempool = m;
    tHighpass_init   (SoftClipModule->mempool->leaf, &SoftClipModule->highpass, 20.0f);
    SoftClipModule->mempool = m;
    SoftClipModule->inputGain = 1.0f;
    SoftClipModule->outputGain = 1.0f;
    SoftClipModule->offset = 0.0f;
    SoftClipModule->shapeDivider = 1.0f;

    SoftClipModule->uniqueID = id;



    SoftClipModule->moduleType = ModuleTypeSoftClipModule;
}



void tSoftClipModule_processorInit(tSoftClipModule const c, leaf::tProcessor* processor)
{
    // Checks that arguments are valid
    assert (c != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = c->uniqueID;
    processor->object = c;
    processor->numSetterFunctions = SoftClipNumParams;
    processor->tick = reinterpret_cast<tTickFuncReturningVoid>(tSoftClipModule_tick);


    processor->inParameters = c->params;
    processor->outParameters = c->outputs;
    processor->processorTypeID = ModuleTypeSoftClipModule;
}

