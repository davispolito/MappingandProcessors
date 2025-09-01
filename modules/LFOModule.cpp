//
// Created by Joshua Warner on 6/13/24.
//

#include "LFOModule.h"

#include <assert.h>

void tLFOModule_init(void** const lfo, float* params, float id, LEAF* const leaf)
{
    if(leaf->lfoRateTable == NULL)
    {
        tLookupTable_init(&leaf->lfoRateTable, 0.3f, 30.f, 2.f,2048,leaf);
       // LEAF_generate_table_skew_non_sym(skewTableLFORate, 0.0f, 30.0f, 2.0f, 2048);
    }
    tLFOModule_initToPool(lfo, params, id,  &leaf->mempool, leaf->lfoRateTable);
}

void tLFOModule_blankFunction (tLFOModule const lfo, float freq)
{
    ;
}
void tLFOModule_initToPool(void** const lfo, float* const params, float id, tMempool* const mempool, tLookupTable rateTable)
{
    _tMempool* m = *mempool;
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo = (_tLFOModule*) mpool_alloc(sizeof(_tLFOModule), m));
    int type = 0;
#ifndef __cplusplus
    memcpy(LFOModule->params, params, LFONumParams*sizeof(ATOMIC_FLOAT));
    int type = roundf(CPPDEREF LFOModule->params[LFOType]);
#endif
    LFOModule->uniqueID = id;
    LFOModule->table = rateTable;
    LFOModule->params[LFOType] = 0;


    LFOModule->mempool = m;
    LFOModule->setterFunctions[LFOEventWatchFlag] = (tSetter)(&tLFOModule_blankFunction);
    LFOModule->setterFunctions[LFOType] = (tSetter)(&tLFOModule_blankFunction);
    LFOModule->setterFunctions[LFORateParam] = (tSetter)(&tLFOModule_setRate);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_initToPool((tSineTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tSineTriLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tSineTriLFO_setShape);
        LFOModule->freq_set_func = (tSetter)(&tSineTriLFO_setFreq);
        LFOModule->tick = (tTickFuncReturningFloat)(&tSineTriLFO_tick);
    }
    else if (type == LFOTypeSawSquare) {

        tSawSquareLFO_initToPool((tSawSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tSawSquareLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tSawSquareLFO_setShape);
        LFOModule->freq_set_func = (tSetter)(&tSawSquareLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tSawSquareLFO_tick);
    }
    else if (type == LFOTypeSine) {

        tCycle_initToPool((tCycle*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tCycle_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func = (tSetter)(&tCycle_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tCycle_tick);
    }
    else if (type == LFOTypeTri) {

        tTriLFO_initToPool((tTriLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tTriLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func  = (tSetter)(&tTriLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tTriLFO_tick);
    }
    else if (type == LFOTypeSaw) {

        tIntPhasor_initToPool((tIntPhasor*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tIntPhasor_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tLFOModule_blankFunction);//better way to do this?
        LFOModule->freq_set_func = (tSetter)(&tIntPhasor_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat>(&tIntPhasor_tickBiPolar);
    }
    else if (type == LFOTypeSquare) {

        tSquareLFO_initToPool((tSquareLFO*)&LFOModule->theLFO, mempool);
        LFOModule->setterFunctions[LFOPhaseParam] = (tSetter)(&tSquareLFO_setPhase);
        LFOModule->setterFunctions[LFOShapeParam] = (tSetter)(&tSquareLFO_setPulseWidth);//better way to do this?
        LFOModule->freq_set_func = (tSetter)(&tSquareLFO_setFreq);
        LFOModule->tick = reinterpret_cast<tTickFuncReturningFloat >(&tSquareLFO_tick);
    }
    LFOModule->moduleType = ModuleTypeLFOModule;
}


void tLFOModule_free(void** const lfo)
{
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo);
    int type = roundf(CPPDEREF LFOModule->params[LFOType]);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_free((tSineTriLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSawSquare) {
        tSawSquareLFO_free((tSawSquareLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSine) {
        tCycle_free((tCycle*)LFOModule->theLFO);
    }
    else if (type == LFOTypeTri) {
        tTriLFO_free((tTriLFO*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSaw) {
        tIntPhasor_free((tIntPhasor*)LFOModule->theLFO);
    }
    else if (type == LFOTypeSquare) {
        tSquareLFO_free((tSquareLFO*)LFOModule->theLFO);
    }
    mpool_free((char*)LFOModule, LFOModule->mempool);
}
//tick function
void tLFOModule_tick (tLFOModule const lfo)
{
    lfo->outputs[0] = lfo->tick(lfo->theLFO);
}

//special noteOnFunction
void tLFOModule_onNoteOn(tLFOModule const lfo, float pitch, float velocity)
{
    lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, CPPDEREF lfo->params[LFOPhaseParam]); //call actual function
}

// Modulatable setters
void tLFOModule_setRate (tLFOModule const lfo, float rate)
{
    rate *= (float)lfo->table->tableSize;
    int inputInt = (int)rate;
    float inputFloat = (float)inputInt - rate;
    int nextPos = LEAF_clip(0, inputInt + 1, lfo->table->tableSize);
    float tempRate = (lfo->table->table[inputInt] * (1.0f - inputFloat)) + (lfo->table->table[nextPos] * inputFloat);
    lfo->freq_set_func (lfo->theLFO, tempRate);
}

void tLFOModule_setShape(tLFOModule const lfo, float shape)
{
    lfo->setterFunctions[LFOShapeParam](lfo->theLFO, shape);
}

void tLFOModule_setPhase (tLFOModule const lfo, float phase)
{
    lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, phase);
}

// Non-modulatable setters
//void tLFOModule_setRateTableLocationAndSize (tLFOModule const lfo, float* tableAddress, uint32_t size)
//{
//    lfo->rateTable = tableAddress;
//    lfo->rateTableSize = size-1;
//}
void tLFOModule_setSampleRate (tLFOModule const lfo, float sr)
{
    //how to handle this? if then cases for different types?

}
