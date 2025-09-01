//
// Created by Joshua Warner on 6/13/24.
//

#include "SimpleOscModule.h"

#include <assert.h>

void tOscModule_init(void** const osc, float* params, float id, LEAF* const leaf)
{
    tOscModule_initToPool(osc, params, id, &leaf->mempool);
}

void tOscModule_blankFunction (tOscModule const osc, float freq)
{
    ;
}

void tOscModule_setType (tOscModule const osc, float typefloat)
{
//destroy current oscillator object
    int type = osc->osctype;

    switch(type)
    {
		case OscTypeSawSquare:
			tPBSawSquare_free((tPBSawSquare*)&osc->theOsc);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_free((tPBSineTriangle*)&osc->theOsc);
			break;
		case OscTypeSaw:
			tPBSaw_free((tPBSaw*)&osc->theOsc);
			break;
		case OscTypePulse:
			tPBPulse_free((tPBPulse*)&osc->theOsc);
			break;
		case OscTypeSine:
			tCycle_free((tCycle*)&osc->theOsc);
			break;
		case OscTypeTri:
			tPBTriangle_free((tPBTriangle*)&osc->theOsc);
			break;
		default:
			break;
    }

    //set new oscillator type
    type = round(typefloat * (float)OscNumTypes);
    //create new oscillator object
    switch (type)
    {
		case OscTypeSawSquare:
			tPBSawSquare_initToPool((tPBSawSquare*)&osc->theOsc, &osc->mempool);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_initToPool((tPBSineTriangle*)&osc->theOsc, &osc->mempool);
			break;
		case OscTypeSaw:
			tPBSaw_initToPool((tPBSaw*)&osc->theOsc, &osc->mempool);
			break;
		case OscTypePulse:
			tPBPulse_initToPool((tPBPulse*)&osc->theOsc, &osc->mempool);
			break;
		case OscTypeSine:
			tCycle_initToPool((tCycle*)&osc->theOsc, &osc->mempool);
			break;
		case OscTypeTri:
			tPBTriangle_initToPool((tPBTriangle*)&osc->theOsc, &osc->mempool);
			break;
		default:
			break;
    }
    osc->osctype = type;
}
void tOscModule_setParameter(tOscModule const osc, OscParams param_type,float input)
{
	float factor;
	switch (param_type) {
	case OscEventWatchFlag:

		break;
	case OscMidiPitch:
		osc->inputNote = input * 127.0f;
		break;
	case OscHarmonic:
		input -= 0.5f;
		input *= 2.f;
		input *= 15.0f;
		if (osc->hStepped) {
			input = roundf(input);
		}

		if (input >= 0.0f) {
			osc->harmonicMultiplier = (input + 1.0f);
		} else {
			osc->harmonicMultiplier = (1.0f / fabsf((input - 1.0f)));
		}
		break;
	case OscPitchOffset:
		input -= 0.5f;
		input *= 24.0f;
		if (osc->pStepped) {
			input = roundf(input);
		}
		osc->pitchOffset = input;
		break;
	case OscPitchFine:
		osc->fine = (input - 0.5f) * 2.f;
		break;
	case OscFreqOffset:
		osc->freqOffset = (input * 4000.0f) - 2000.f;
		break;
	case OscShapeParam:
		break;
	case OscAmpParam:
		osc->amp = input;
		break;
	case OscGlide:
		factor = 1.0f;
		if (input >= 0.00001f) {
			factor = 1.0f - (expf(-osc->invSr / input));
		}
		tExpSmooth_setFactor(osc->pitchSmoother, factor);
		break;
	case OscSteppedHarmonic:
		osc->hStepped = roundf(input);
		break;
	case OscSteppedPitch:
		osc->pStepped = roundf(input);
		break;
	case OscSyncMode:
		osc->syncMode = roundf(input);
		break;
	case OscSyncIn:
		break;
	case OscType:
		tOscModule_setType(osc, input);
		break;
	default:
		break;
	}
}

void tOscModule_initToPool(void** const osc, float* const param, float id, tMempool* const mempool)
{
    _tMempool* m = *mempool;
    _tOscModule* OscModule = (_tOscModule*) (*osc = (_tOscModule*) mpool_alloc (sizeof (_tOscModule), m));
#ifndef __cplusplus
    memcpy(OscModule->params, param, OscNumParams*sizeof(float));
    int type = roundf(CPPDEREF OscModule->params[OscType]);
#endif __cplusplus
    OscModule->uniqueID = id;

    int type =OscTypeSawSquare;
    OscModule->osctype = type;
    OscModule->mempool = m;
    OscModule->invSr = m->leaf->invSampleRate;
    OscModule->sr = m->leaf->sampleRate;

    tExpSmooth_initToPool(&OscModule->pitchSmoother, 64.0f, 0.05f, mempool);
    switch (type)
    {
		case OscTypeSawSquare:
			tPBSawSquare_initToPool((tPBSawSquare*)&OscModule->theOsc, mempool);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_initToPool((tPBSineTriangle*)&OscModule->theOsc, mempool);
			break;
		case OscTypeSaw:
			tPBSaw_initToPool((tPBSaw*)&OscModule->theOsc, mempool);
			break;
		case OscTypePulse:
			tPBPulse_initToPool((tPBPulse*)&OscModule->theOsc, mempool);
			break;
		case OscTypeSine:
			tCycle_initToPool((tCycle*)&OscModule->theOsc, mempool);
			break;
		case OscTypeTri:
			tPBTriangle_initToPool((tPBTriangle*)&OscModule->theOsc, mempool);
			break;
		default:
			break;
    }

    OscModule->moduleType = ModuleTypeOscModule;
#ifndef __cplusplus
    for (int i = 0; i < OscNumParams; i++)
    {
    	tOscModule_setParameter(OscModule, i, OscModule->params[i]);

    }
#endif

}


void tOscModule_free(void** const osc)
{
    _tOscModule* OscModule = (_tOscModule*) (*osc);

    int type = OscModule->osctype;
    switch(type)
    {
		case OscTypeSawSquare:
			tPBSawSquare_free((tPBSawSquare*)&OscModule->theOsc);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_free((tPBSineTriangle*)&OscModule->theOsc);
			break;
		case OscTypeSaw:
			tPBSaw_free((tPBSaw*)&OscModule->theOsc);
			break;
		case OscTypePulse:
			tPBPulse_free((tPBPulse*)&OscModule->theOsc);
			break;
		case OscTypeSine:
			tCycle_free((tCycle*)&OscModule->theOsc);
			break;
		case OscTypeTri:
			tPBTriangle_free((tPBTriangle*)&OscModule->theOsc);
			break;
		default:
			break;
    }
    tExpSmooth_free(&OscModule->pitchSmoother);
    mpool_free((char*)OscModule, OscModule->mempool);
}


// tick function
void tOscModule_tick (tOscModule const osc,float* buffer)
{
	float freqToSmooth = (osc->inputNote + (osc->fine));
	    tExpSmooth_setDest(osc->pitchSmoother, freqToSmooth);
	    float tempMIDI =  tExpSmooth_tick(osc->pitchSmoother) + osc->pitchOffset + osc->octaveOffset;

	//    float tempIndexgit F = ((LEAF_clip(-163.0f, tempMIDI, 163.0f) * 100.0f) + 16384.0f);
	//    int tempIndexI = (int)tempIndexF;
	//    tempIndexF = tempIndexF -tempIndexI;
	//    float freqToSmooth1 = osc->mtofTable[tempIndexI & 32767];
	//    float freqToSmooth2 = osc->mtofTable[(tempIndexI + 1) & 32767];
	    float nowFreq = mtof(tempMIDI);// ((freqToSmooth1 * (1.0f - tempIndexF)) + (freqToSmooth2 * tempIndexF));

	    float finalFreq = (nowFreq * osc->harmonicMultiplier ) + osc->freqOffset;
	switch (osc->osctype) {
	case OscTypeSawSquare: {
		tPBSawSquare_setFreq((tPBSawSquare)osc->theOsc,finalFreq);
		*buffer = tPBSawSquare_tick((tPBSawSquare)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeSineTri: {
		tPBSineTriangle_setFreq((tPBSineTriangle)osc->theOsc,finalFreq);
		*buffer = tPBSineTriangle_tick((tPBSineTriangle)osc->theOsc)* osc->amp;
		break;
	}

	case OscTypeSaw: {
		tPBSaw_setFreq((tPBSaw)osc->theOsc,finalFreq);
		*buffer = tPBSaw_tick((tPBSaw)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypePulse: {
		tPBPulse_setFreq((tPBPulse)osc->theOsc,finalFreq);
		*buffer = tPBPulse_tick((tPBPulse)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeSine: {
		tCycle_setFreq((tCycle)osc->theOsc,finalFreq);
		*buffer = tCycle_tick((tCycle)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeTri: {
		tPBTriangle_setFreq((tPBTriangle)osc->theOsc,finalFreq);
		*buffer = tPBTriangle_tick((tPBTriangle)osc->theOsc)* osc->amp;
		break;
	}


	}


//    osc->freq_set_func(osc->theOsc, finalFreq);
    osc->outputs[0] = *buffer;
}








void tOscModule_setOctave (tOscModule const osc, float const oct)
{
	 osc->octaveOffset = (roundf(((oct - 0.5f) * 6.0f))) * 12.0f;
}


// Non-modulatable setters
void tOscModule_setMTOFTableLocation (tOscModule const osc, float* const tableAddress)
{
    osc->mtofTable = tableAddress;
}



