//
// Created by Davis Polito on 4/29/25.
//
#include "StringModule.h"
void tStringModule_init(void** const module, float* params, float id, LEAF* const leaf)
{
    tStringModule_initToPool(module, params, id, &leaf->mempool);
}

void tStringModule_free(void** const mod)
{
    _tStringModule* module =static_cast<_tStringModule *>(*mod);
    mpool_free((char*)module, module->mempool);
}
void tStringModule_initToPool(void** const env, float* const params, float id, tMempool** const mempool)
{
    tMempool* m = *mempool;
    _tStringModule* module = static_cast<_tStringModule *>(*env = (_tStringModule*) mpool_alloc(sizeof(_tStringModule), m));
    #ifndef __cplusplus
    memcpy(module->params, params, StringNumParams);
    #endif
    module->mempool = m;

    tSimpleLivingString3_create (mempool, &module->theString);
    tSimpleLivingString3_init   (m->leaf,
                                 module->theString,
                                 1,
                                 440.f,
                                 440.0,
                                 1.0,
                                 1.0,
                                 1.0,
                                 1.0,
                                 0);


}

void tStringModule_tick(tStringModule const s,float* value)
    {
        }

void tStringModule_processorInit(tStringModule const s, LEAF_NAMESPACE tProcessor* processor)
{

    // Checks that arguments are valid
    // assert(s != NULL);
    //assert(processor != NULL);

    processor->processorUniqueID = s->uniqueID;
    processor->object = s;
    processor->numSetterFunctions = StringNumParams;
    processor->tick = (tTickFuncReturningVoid)&tStringModule_tick;
    processor->inParameters = s->params;
    processor->outParameters = s->outputs;
    processor->processorTypeID = ModuleTypeStringModule;
}
void tStringModule_setParameter(tStringModule const s, StringModelParams param, float input)
{
    switch (param)
    {
        case StringEventWatchFlag:
            // TODO: implement EventWatchFlag inline if needed
            break;

        case StringOversample:
            *s->params[StringOversample] = input; // store directly or implement oversample logic
            break;

        case StringFreq:
            tSimpleLivingString3_setFreq(s->theString, input);
            break;

        case StringWaveLength:
            tSimpleLivingString3_setWaveLength(s->theString, input);
            break;

        case StringDampFreq:
            tSimpleLivingString3_setDampFreq(s->theString, input);
            break;

        case StringDecay:
            tSimpleLivingString3_setDecay(s->theString, input);
            break;

        case StringTargetLevel:
            tSimpleLivingString3_setTargetLev(s->theString, input);
            break;

        case StringLevelSmooth:
            tSimpleLivingString3_setLevSmoothFactor(s->theString, input);
            break;

        case StringLevelStrength:
            tSimpleLivingString3_setLevStrength(s->theString, input);
            break;

        case StringPickupPoint:
            tSimpleLivingString3_setPickupPoint(s->theString, input);
            break;

        case StringLevelMode:
            tSimpleLivingString3_setLevMode(s->theString, (int)input);
            break;

        case StringRippleGain:
            s->theString->rippleGain = input * -0.03f;
            s->theString->invOnePlusr = 1.0f / (1.0f + s->theString->rippleGain);
            break;

        case StringRippleDelay:
            s->theString->rippleDelay = input;
            break;

        case StringPluckPosition:
            // s->theString->pluckPosition = input;
            break;

        default:
            // unknown param, do nothing
            break;
    }
}
