//
// Created by Joshua Warner on 6/13/24.
//

#include "LFOModule.h"

#include <assert.h>

void tLFOModule_init(void** const lfo, float* params, float id, LEAF* const leaf)
{
    if(leaf->lfoRateTable == NULL)
    {
        tLookupTable_create(&leaf->mempool, &leaf->lfoRateTable);
        tLookupTable_init(leaf,leaf->lfoRateTable, 0.3f, 30.f, 2.f,2048);
       // LEAF_generate_table_skew_non_sym(skewTableLFORate, 0.0f, 30.0f, 2.0f, 2048);
    }
    tLFOModule_initToPool(lfo, params, id,  &leaf->mempool, leaf->lfoRateTable);
}

void tLFOModule_blankFunction (tLFOModule const lfo, float freq)
{
    ;
}
void tLFOModule_initToPool(void** const lfo, float* const params, float id, tMempool** const mempool, tLookupTable* rateTable)
{
    tMempool* m = *mempool;
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo = (_tLFOModule*) mpool_alloc(sizeof(_tLFOModule), m));
    int type = 0;
#ifndef __cplusplus
    memcpy(LFOModule->params, params, LFONumParams*sizeof(ATOMIC_FLOAT));
    int type = roundf(CPPDEREF LFOModule->params[LFOType]);
#endif
    LFOModule->header.uniqueID = id;
    LFOModule->table = rateTable;
    LFOModule->header.params[LFOType] = 0;
    LFOModule->lfo_type = type;

    LFOModule->mempool = m;
    // LFOModule->setterFunctions[LFOEventWatchFlag] = (tSetter)(&tLFOModule_blankFunction);
    // LFOModule->setterFunctions[LFOType] = (tSetter)(&tLFOModule_blankFunction);
    // LFOModule->setterFunctions[LFORateParam] = (tSetter)(&tLFOModule_setRate);
    if (LFOModule->lfo_type == LFOTypeSineTri)
    {
        tSineTriLFO_create (mempool, (tSineTriLFO**)&LFOModule->theLFO);
        tSineTriLFO_init   (m->leaf, (tSineTriLFO*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tSineTriLFO_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tSineTriLFO_tick);
    }
    else if (LFOModule->lfo_type == LFOTypeSawSquare)
    {
        tSawSquareLFO_create (mempool, (tSawSquareLFO**)&LFOModule->theLFO);
        tSawSquareLFO_init   (m->leaf, (tSawSquareLFO*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tSawSquareLFO_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tSawSquareLFO_tick);
    }
    else if (LFOModule->lfo_type == LFOTypeSine)
    {
        tCycle_create (mempool, (tCycle**)&LFOModule->theLFO);
        tCycle_init   (m->leaf, (tCycle*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tCycle_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tCycle_tick);
    }
    else if (LFOModule->lfo_type == LFOTypeTri)
    {
        tTriLFO_create (mempool, (tTriLFO**)&LFOModule->theLFO);
        tTriLFO_init   (m->leaf, (tTriLFO*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tTriLFO_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tTriLFO_tick);
    }
    else if (LFOModule->lfo_type == LFOTypeSaw)
    {
        tIntPhasor_create (mempool, (tIntPhasor**)&LFOModule->theLFO);
        tIntPhasor_init   (m->leaf, (tIntPhasor*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tIntPhasor_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tIntPhasor_tickBiPolar);
    }
    else if (LFOModule->lfo_type == LFOTypeSquare)
    {
        tSquareLFO_create (mempool, (tSquareLFO**)&LFOModule->theLFO);
        tSquareLFO_init   (m->leaf, (tSquareLFO*)LFOModule->theLFO);

        LFOModule->freq_set_func = (tSetter)(&tSquareLFO_setFreq);
        // LFOModule->header.tick          = (tTickFuncReturningFloat)(&tSquareLFO_tick);
    }

    LFOModule->header.moduleType = ModuleTypeLFOModule;
}


void tLFOModule_free(void** const lfo)
{
    _tLFOModule* LFOModule = static_cast<_tLFOModule*>(*lfo);
    int type = roundf(CPPDEREF LFOModule->header.params[LFOType]);
    if (type == LFOTypeSineTri) {
        tSineTriLFO_free((tSineTriLFO**)LFOModule->theLFO);
    }
    else if (type == LFOTypeSawSquare) {
        tSawSquareLFO_free((tSawSquareLFO**)LFOModule->theLFO);
    }
    else if (type == LFOTypeSine) {
        tCycle_free((tCycle**)LFOModule->theLFO);
    }
    else if (type == LFOTypeTri) {
        tTriLFO_free((tTriLFO**)LFOModule->theLFO);
    }
    else if (type == LFOTypeSaw) {
        tIntPhasor_free((tIntPhasor**)LFOModule->theLFO);
    }
    else if (type == LFOTypeSquare) {
        tSquareLFO_free((tSquareLFO**)LFOModule->theLFO);
    }
    mpool_free((char*)LFOModule, LFOModule->mempool);
}
//tick function
void tLFOModule_tick (tLFOModule const lfo)
{

        switch (lfo->lfo_type)
        {
            case LFOTypeSineTri:
                lfo->header.outputs[0] = tSineTriLFO_tick((tSineTriLFO*)lfo->theLFO);
                break;

            case LFOTypeSawSquare:
                lfo->header.outputs[0] = tSawSquareLFO_tick((tSawSquareLFO*)lfo->theLFO);
                break;

            case LFOTypeSine:
                lfo->header.outputs[0] = tCycle_tick((tCycle*)lfo->theLFO);
                break;

            case LFOTypeTri:
                lfo->header.outputs[0] = tTriLFO_tick((tTriLFO*)lfo->theLFO);
                break;

            case LFOTypeSaw:
                lfo->header.outputs[0] = tIntPhasor_tickBiPolar((tIntPhasor*)lfo->theLFO);
                break;

            case LFOTypeSquare:
                lfo->header.outputs[0] = tSquareLFO_tick((tSquareLFO*)lfo->theLFO);
                break;

            default:
                lfo->header.outputs[0] = 0.0f;
                break;
        }
}

//special noteOnFunction
void tLFOModule_onNoteOn(tLFOModule const lfo, float pitch, float velocity)
{
    // lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, CPPDEREF lfo->params[LFOPhaseParam]); //call actual function
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
    // lfo->setterFunctions[LFOShapeParam](lfo->theLFO, shape);
}

void tLFOModule_setPhase (tLFOModule const lfo, float phase)
{
    // lfo->setterFunctions[LFOPhaseParam](lfo->theLFO, phase);
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

void tLFOModule_setParameter(tLFOModule const lfo, LFOParams param_type, float input)
{
    switch (param_type)
    {
        case LFOEventWatchFlag:
        {

            break;
        }

        case LFOType:
        {

            break;
        }

        case LFORateParam:
        {
            // Interpolate lookup table and set frequency
            input *= (float)lfo->table->tableSize;
            int inputInt = (int)input;
            float inputFloat = (float)inputInt - input;
            int nextPos = LEAF_clip(0, inputInt + 1, lfo->table->tableSize);
            float tempRate = (lfo->table->table[inputInt] * (1.0f - inputFloat))
                           + (lfo->table->table[nextPos] * inputFloat);
            lfo->freq_set_func(lfo->theLFO, tempRate);
            break;
        }

        case LFOShapeParam:
        {
            switch (lfo->header.moduleType)
            {
                case LFOTypeSineTri: {
                    tSineTriLFO_setPhase((tSineTriLFO*)lfo->theLFO,input);
                    break;
                }
                 case LFOTypeSquare:
                {
                    tSquareLFO_setPulseWidth((tSquareLFO*)lfo->theLFO, input);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case LFOPhaseParam:
        {
            switch (lfo->header.moduleType)
            {
                case LFOTypeSineTri:
                {
                    tSineTriLFO_setPhase((tSineTriLFO*)lfo->theLFO, input);
                    break;
                }

                case LFOTypeSawSquare:
                {
                    tSawSquareLFO_setPhase((tSawSquareLFO*)lfo->theLFO, input);
                    break;
                }

                case LFOTypeSine:
                {
                    tCycle_setPhase((tCycle*)lfo->theLFO, input);
                    break;
                }

                case LFOTypeTri:
                {
                    tTriLFO_setPhase((tTriLFO*)lfo->theLFO, input);
                    break;
                }

                case LFOTypeSaw:
                {
                    tIntPhasor_setPhase((tIntPhasor*)lfo->theLFO, input);
                    break;
                }

                case LFOTypeSquare:
                {
                    tSquareLFO_setPhase((tSquareLFO*)lfo->theLFO, input);
                    break;
                }

                default:
                    break;
            }
            break;
        }


        default:
            break;
    }
}
