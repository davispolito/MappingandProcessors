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
void tStringModule_initToPool(void** const env, float* const params, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tStringModule* module = static_cast<_tStringModule *>(*env = (_tStringModule*) mpool_alloc(sizeof(_tStringModule), m));
    #ifndef __cplusplus
    memcpy(module->params, params, StringNumParams);
    #endif
    module->mempool = m;

    tSimpleLivingString3_initToPool(&module->theString, 1,440.f, 440.0,
                                                 1.0, 1.0,1.0,1.0,
                                                 0,mempool);
    module->setterFunctions[StringEventWatchFlag] = (tSetter)&tStringModule_setEventWatchFlag;
    module->setterFunctions[StringOversample]      = (tSetter)&tStringModule_setOversample;
    module->setterFunctions[StringFreq]            = (tSetter)&tStringModule_setFreq;
    module->setterFunctions[StringWaveLength]      = (tSetter)&tStringModule_setWaveLength;
    module->setterFunctions[StringDampFreq]        = (tSetter)&tStringModule_setDampFreq;
    module->setterFunctions[StringDecay]           = (tSetter)&tStringModule_setDecay;
    module->setterFunctions[StringTargetLevel]     = (tSetter)&tStringModule_setTargetLevel;
    module->setterFunctions[StringLevelStrength]   = (tSetter)&tStringModule_setLevelStrength;
    module->setterFunctions[StringLevelSmooth]     = (tSetter)&tStringModule_setLevelSmooth;
    module->setterFunctions[StringPickupPoint]     = (tSetter)&tStringModule_setPickupPoint;
    module->setterFunctions[StringLevelMode]       = (tSetter)&tStringModule_setLevelMode;
    module->setterFunctions[StringRippleGain]      = (tSetter)&tStringModule_setRippleGain;
    module->setterFunctions[StringRippleDelay]     = (tSetter)&tStringModule_setRippleDelay;
    module->setterFunctions[StringPluckPosition]   = (tSetter)&tStringModule_setPluckPosition;


}
void tStringModule_setEventWatchFlag(tStringModule const s, float value)
{
    // TODO: Implement EventWatchFlag setter
}

void tStringModule_setOversample(tStringModule const s, int value)
{
    // TODO: Implement Oversample setter

}

void tStringModule_setFreq(tStringModule const s, float value)
{
    // TODO: Implement Freq setter
    tSimpleLivingString3_setFreq(s->theString, value);
}

void tStringModule_setWaveLength(tStringModule const s, float value)
{
    // TODO: Implement WaveLength setter
    tSimpleLivingString3_setWaveLength(s->theString, value);
}

void tStringModule_setDampFreq(tStringModule const s, float value)
{
    // TODO: Implement DampFreq setter
    tSimpleLivingString3_setDampFreq(s->theString, value);
}

void tStringModule_setDecay(tStringModule const s, float value)
{
    // TODO: Implement Decay setter
    tSimpleLivingString3_setDecay(s->theString, value);
}

void tStringModule_setTargetLevel(tStringModule const s, float value)
{
    // TODO: Implement TargetLevel setter
    tSimpleLivingString3_setTargetLev(s->theString, value);
}

void tStringModule_setLevelSmooth(tStringModule const s, float value)
{
    // TODO: Implement LevelSmooth setter
     tSimpleLivingString3_setLevSmoothFactor(s->theString, value);
}

void tStringModule_setLevelStrength     (tStringModule const s, float value)
{
    tSimpleLivingString3_setLevStrength(s->theString, value);
}
void tStringModule_setPickupPoint(tStringModule const s, float value)
{
    // TODO: Implement PickupPoint setter
    tSimpleLivingString3_setPickupPoint(s->theString, value);
}

void tStringModule_setLevelMode(tStringModule const s, int value)
{
    // TODO: Implement LevelMode setter
    tSimpleLivingString3_setLevMode(s->theString, value);
}

void tStringModule_setRippleGain(tStringModule const s, float value)
{
    // TODO: Implement RippleGain setter
   s->theString->rippleGain = value *-0.03;
   s->theString->invOnePlusr =1.0f / (1.0f +s->theString->rippleGain);
}

void tStringModule_setRippleDelay(tStringModule const s, float value)
{
    // TODO: Implement RippleDelay setter

    s->theString->rippleDelay = value;
}

void tStringModule_setPluckPosition(tStringModule const s, float value)
{
    // TODO: Implement PluckPosition setter
    //s->params[StringPluckPosition] = value;
}
void tStringModule_tick(tStringModule const s,float* value)
    {
        }

