//
// Created by Jeffrey Snyder on 7/31/24.
//
//
// Created by Joshua Warner on 6/13/24.
//

#include "FilterModule.h"

#include <assert.h>

void tFiltModule_init(void** const filt, float* params, float id, LEAF* const leaf) {
	if(leaf->resTable == NULL)
		{
		tLookupTable_create(&leaf->mempool,&leaf->resTable);
		tLookupTable_init(leaf, leaf->resTable,  0.0f, 10.0f, 0.5f, 2048);
	}
    tFiltModule_initToPool(filt, params, id, &leaf->mempool, leaf->resTable);
}

float dbToATableLookupFunction(float const in, float const sizeMinusOne, float* const tableAddress)
{
    uint32_t inDBIndex = (uint32_t) in;
    uint32_t inDBIndexPlusOne = inDBIndex + 1;
    if (inDBIndexPlusOne > sizeMinusOne)
    {
        inDBIndexPlusOne = sizeMinusOne;
    }
    float alpha = in - (float)inDBIndex;
    return ((tableAddress[inDBIndex] * (1.0f - alpha)) + (tableAddress[inDBIndexPlusOne] * alpha));
}


float resTableLookupFunction (float input, float* resTableAddress, float resTableSizeMinusOne)
{
    input *= resTableSizeMinusOne;
	int inputInt = (int)input;
	float inputFloat = (float)inputInt - input;
	int nextPos = LEAF_clip(0.0f, inputInt + 1.0f, resTableSizeMinusOne);
	return LEAF_clip(0.1f, (resTableAddress[inputInt] * (1.0f - inputFloat)) + (resTableAddress[nextPos] * inputFloat), 10.0f);
}


void tFiltModule_setGain(tFiltModule const filt, float const gain)
{
	float floatIndex = 0.0f;
	switch(filt->filtType)
	 {
	 case FiltTypeLowpass:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 case FiltTypeHighpass:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 case FiltTypeBandpass:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 case FiltTypeDiodeLowpass:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 case FiltTypePeak:
		 floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		  tVZFilterBell_setGain((tVZFilterBell*)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
		 break;
	 case FiltTypeHighShelf:
		 floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f  * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 tVZFilterHS_setGain((tVZFilterHS*)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
		 break;
	 case FiltTypeLowShelf:
		 floatIndex = LEAF_clip (0, ((gain * 50.f) - 25.f  * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 tVZFilterLS_setGain((tVZFilterLS*)filt->theFilt, dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress));
		 break;
	 case FiltTypeNotch:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 case FiltTypeLadderLowpass:
		 floatIndex = LEAF_clip (0, ((gain * 24.0f) - 12.0f * filt->dbTableScalar) -  filt->dbTableOffset, filt->dbTableSizeMinusOne);
		 filt->amp = gain;//dbToATableLookupFunction(floatIndex, filt->dbTableSizeMinusOne, filt->dbTableAddress);
		 break;
	 default:
		 break;
	 }
}


void tFiltModule_setRes(tFiltModule const filt, float const res)
{
	 switch(filt->filtType)
	 {
	 case FiltTypeLowpass:
		 tSVF_setQ((tSVF*)filt->theFilt,resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeHighpass:
		 tSVF_setQ((tSVF*)filt->theFilt,resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeBandpass:
		 tSVF_setQ((tSVF*)filt->theFilt,resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeDiodeLowpass:
		 tDiodeFilter_setQ((tDiodeFilter*)filt->theFilt, resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypePeak:
		 tVZFilterBell_setBandwidth((tVZFilterBell*)filt->theFilt, resTableLookupFunction(res*20.0f, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeHighShelf:
		 tVZFilterHS_setResonance((tVZFilterHS*)filt->theFilt, resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeLowShelf:
		 tVZFilterLS_setResonance((tVZFilterLS*)filt->theFilt, resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeNotch:
		 tVZFilterBR_setResonance((tVZFilterBR*)filt->theFilt,resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 case FiltTypeLadderLowpass:
		 tLadderFilter_setQ((tLadderFilter*)filt->theFilt, resTableLookupFunction(res, filt->table->table, filt->table->tableSize-1));
		 break;
	 default:
		 break;
	 }
}


void tFiltModule_initToPool(void** const filt, float* const params, float id, tMempool** const mempool, tLookupTable* resTable)
{
    tMempool* m = *mempool;
    _tFiltModule* FiltModule =(_tFiltModule *) ( *filt = (_tFiltModule*) mpool_alloc(sizeof(_tFiltModule), m));
#ifndef __cplusplus
    memcpy(FiltModule->params, params, FiltNumParams*sizeof(float));

#endif
    FiltModule->header.uniqueID = id;
    //CPPDEREF FiltModule->params[FiltAudioInput] = 0.0f;
    int type = 0.0f;//roundf(FiltModule->params[FiltType]);
    FiltModule->mempool = m;
    FiltModule->amp = 1.0f;
    FiltModule->invSr = m->leaf->invSampleRate;
    FiltModule->sr = m->leaf->sampleRate;
	FiltModule->table = resTable;
    if (type == FiltTypeLowpass) {
        tSVF_create(mempool, (tSVF**)&FiltModule->theFilt);
    	tSVF_init(m->leaf,(tSVF*)FiltModule->theFilt, SVFTypeLowpass,10000.0f, 0.5f);
    }
    else if (type == FiltTypeHighpass)
    {
    	tSVF_create (mempool, (tSVF**)&FiltModule->theFilt);
    	tSVF_init   (m->leaf, (tSVF*)FiltModule->theFilt,
					 SVFTypeHighpass, 100.0f, 0.5f);
    }
    else if (type == FiltTypeBandpass)
    {
    	tSVF_create (mempool, (tSVF**)&FiltModule->theFilt);
    	tSVF_init   (m->leaf, (tSVF*)FiltModule->theFilt,
					 SVFTypeBandpass, 100.0f, 0.5f);
    }
    else if (type == FiltTypeDiodeLowpass)
    {
    	tDiodeFilter_create (mempool, (tDiodeFilter**)&FiltModule->theFilt);
    	tDiodeFilter_init   (m->leaf, (tDiodeFilter*)FiltModule->theFilt,
							 10000.0f, 0.5f);
    }
    else if (type == FiltTypePeak)
    {
    	tVZFilterBell_create (mempool, (tVZFilterBell**)&FiltModule->theFilt);
    	tVZFilterBell_init   (m->leaf, (tVZFilterBell*)FiltModule->theFilt,
							  100.0f, 0.5f, 1.0f);
    }
    else if (type == FiltTypeHighShelf)
    {
    	tVZFilterHS_create (mempool, (tVZFilterHS**)&FiltModule->theFilt);
    	tVZFilterHS_init   (m->leaf, (tVZFilterHS*)FiltModule->theFilt,
							100.0f, 0.5f, 1.0f);
    }
    else if (type == FiltTypeLowShelf)
    {
    	tVZFilterLS_create (mempool, (tVZFilterLS**)&FiltModule->theFilt);
    	tVZFilterLS_init   (m->leaf, (tVZFilterLS*)FiltModule->theFilt,
							100.0f, 0.5f, 1.0f);
    }
    else if (type == FiltTypeNotch)
    {
    	tVZFilterBR_create (mempool, (tVZFilterBR**)&FiltModule->theFilt);
    	tVZFilterBR_init   (m->leaf, (tVZFilterBR*)FiltModule->theFilt,
							100.0f, 0.5f);
    }
    else if (type == FiltTypeLadderLowpass)
    {
    	tLadderFilter_create (mempool, (tLadderFilter**)&FiltModule->theFilt);
    	tLadderFilter_init   (m->leaf, (tLadderFilter*)FiltModule->theFilt,
							  100.0f, 0.5f);
    }
    FiltModule->header.moduleType = ModuleTypeFilterModule;

#ifndef __cplusplus
    for (int i = 0; i < FiltNumParams; i++)
    {
    	tFiltModule_setParameter(FiltModule, i, FiltModule->params[i]);

    }
#endif

}


void tFiltModule_free(void** const filt)
{
    _tFiltModule* FiltModule =(_tFiltModule*) *filt;
    int type = roundf(CPPDEREF FiltModule->header.params[FiltType]);
    if (type == FiltTypeLowpass) {
        tSVF_free((tSVF**)FiltModule->theFilt);
    }
    else if (type == FiltTypeHighpass) {
        tSVF_free((tSVF**)FiltModule->theFilt);
    }
    else if (type == FiltTypeBandpass) {
        tSVF_free((tSVF**)FiltModule->theFilt);
    }
    else if (type == FiltTypeDiodeLowpass) {
        tDiodeFilter_free((tDiodeFilter**)FiltModule->theFilt);
    }
    else if (type == FiltTypePeak) {
        tVZFilterBell_free((tVZFilterBell**)FiltModule->theFilt);
    }
    else if (type == FiltTypeLowShelf) {
        tVZFilterLS_free((tVZFilterLS**)FiltModule->theFilt);
    }
    else if (type == FiltTypeHighShelf) {
        tVZFilterHS_free((tVZFilterHS**)FiltModule->theFilt);
    }
    else if (type == FiltTypeNotch) {
        tVZFilterBR_free((tVZFilterBR**)FiltModule->theFilt);
    }
    else if (type == FiltTypeLadderLowpass) {
        tLadderFilter_free((tLadderFilter**)FiltModule->theFilt);
    }
    mpool_free((char*)FiltModule, FiltModule->mempool);
}


// tick function
void tFiltModule_tick (tFiltModule const filt, float* buffer)
{
    //removing keyfollow since it is not set up to a parameter right now
    //TODO: add keyfollow
    float const cutoff  = filt->cutoffKnob + (filt->inputNote*filt->keyFollow);//  * CPPDEREF filt->params[FiltKeyfollow]); // TODO: should this be cutoffKnob * 137 to allow full range of knob turn to map to maxium freq?)
    switch(filt->filtType)
    {
    case FiltTypeLowpass:
    	tSVF_setFreqFast((tSVF*)filt->theFilt, cutoff);
        //buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
        buffer[0] = filt->header.outputs[0] = tSVF_tick((tSVF*)filt->theFilt,  buffer[0]) * filt->amp;
    	break;

    case FiltTypeHighpass:
    	tSVF_setFreqFast((tSVF*)filt->theFilt, cutoff);
        //buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
        buffer[0] = filt->header.outputs[0] = tSVF_tick((tSVF*)filt->theFilt,  buffer[0]) * filt->amp;
    	break;

    case FiltTypeBandpass:
    	tSVF_setFreqFast((tSVF*)filt->theFilt, cutoff);
        //buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
        buffer[0] = filt->header.outputs[0] = tSVF_tick((tSVF*)filt->theFilt,  buffer[0]) * filt->amp;
    	break;

    case FiltTypeDiodeLowpass:
    	tDiodeFilter_setFreqFast((tDiodeFilter*)filt->theFilt, cutoff);
        //buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
        buffer[0] = filt->header.outputs[0] = tDiodeFilter_tickEfficient((tDiodeFilter*)filt->theFilt,  buffer[0]) * filt->amp;
    	break;

    case FiltTypePeak:
    	tVZFilterBell_setFreqFast((tVZFilterBell*)filt->theFilt, cutoff);
		//buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
		buffer[0] = filt->header.outputs[0] = tVZFilterBell_tick((tVZFilterBell*)filt->theFilt,  buffer[0]) * filt->amp;
		break;

    case FiltTypeHighShelf:
    	tVZFilterHS_setFreqFast((tVZFilterHS*)filt->theFilt, cutoff);
		//buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
		buffer[0] = filt->header.outputs[0] = tVZFilterHS_tick((tVZFilterHS*)filt->theFilt,  buffer[0]) * filt->amp;
		break;

    case FiltTypeLowShelf:
    	tVZFilterLS_setFreqFast((tVZFilterLS*)filt->theFilt, cutoff);
		//buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
		buffer[0] = filt->header.outputs[0] = tVZFilterLS_tick((tVZFilterLS*)filt->theFilt,  buffer[0]) * filt->amp;
		break;

    case FiltTypeNotch:
    	tVZFilterBR_setFreqFast((tVZFilterBR*)filt->theFilt, cutoff);
		//buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
		buffer[0] = filt->header.outputs[0] = tVZFilterBR_tick((tVZFilterBR*)filt->theFilt,  buffer[0]) * filt->amp;
		break;

    case FiltTypeLadderLowpass:
    	tLadderFilter_setFreqFast((tLadderFilter*)filt->theFilt, cutoff);
		//buffer[0] += CPPDEREF filt->params[FiltAudioInput]; //buffer passed to function
		buffer[0] = filt->header.outputs[0] = tLadderFilter_tick((tLadderFilter*)filt->theFilt,  buffer[0]) * filt->amp;
		break;
    default:
    	break;


    }
}

// Modulatable setters


void tFiltModule_setParameter(tFiltModule const filt, FiltParams param_type,float input)
{
	switch (param_type) {
	case FiltEventWatchFlag:

		break;
	case FiltMidiPitch:
		filt->inputNote = input * 127.0f;
		break;
	case FiltCutoff:
		filt->cutoffKnob = input * 127.0f;
		break;
	case FiltGain:
		tFiltModule_setGain(filt, input);
		break;
	case FiltResonance:
		tFiltModule_setRes(filt, input);
		break;
	case FiltKeyfollow:
		filt->keyFollow = input;
		break;
	case FiltType:

		break;

	default:
		break;
	}
}

void tFiltModule_setMIDIPitch (tFiltModule const filt, float const input)
{
    filt->inputNote = input * 127.0f;
}

void tFiltModule_setCutoff (tFiltModule const filt, float const input)
{
    filt->cutoffKnob = input * 127.0f;
}

// Non-modulatable setters

void tFiltModule_setDBtoATableLocation (tFiltModule const filt, float* const tableAddress, uint32_t const tableSize)
{
    filt->dbTableAddress = tableAddress;
    filt->dbTableSizeMinusOne = (float)(tableSize - 1);
    filt->dbTableScalar = filt->dbTableSizeMinusOne/(4.0f-0.00001f);
    filt->dbTableOffset = 0.00001f * filt->dbTableScalar;
}



void tFiltModule_setSampleRate (tFiltModule const filt, float const sr)
{
    //tCycle_setSampleRate(filt->filts[0], sr);
}






