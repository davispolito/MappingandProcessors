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
			tPBSawSquare_free((tPBSawSquare**)&osc->theOsc);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_free((tPBSineTriangle**)&osc->theOsc);
			break;
		case OscTypeSaw:
			tPBSaw_free((tPBSaw**)&osc->theOsc);
			break;
		case OscTypePulse:
			tPBPulse_free((tPBPulse**)&osc->theOsc);
			break;
		case OscTypeSine:
			tCycle_free((tCycle**)&osc->theOsc);
			break;
		case OscTypeTri:
			tPBTriangle_free((tPBTriangle**)&osc->theOsc);
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
			tPBSawSquare_create (&osc->mempool, (tPBSawSquare**)&osc->theOsc);
			tPBSawSquare_init   (osc->mempool->leaf, (tPBSawSquare*)osc->theOsc);
			break;

		case OscTypeSineTri:
			tPBSineTriangle_create (&osc->mempool, (tPBSineTriangle**)&osc->theOsc);
			tPBSineTriangle_init   (osc->mempool->leaf, (tPBSineTriangle*)osc->theOsc);
			break;

		case OscTypeSaw:
			tPBSaw_create (&osc->mempool, (tPBSaw**)&osc->theOsc);
			tPBSaw_init   (osc->mempool->leaf, (tPBSaw*)osc->theOsc);
			break;

		case OscTypePulse:
			tPBPulse_create (&osc->mempool, (tPBPulse**)&osc->theOsc);
			tPBPulse_init   (osc->mempool->leaf, (tPBPulse*)osc->theOsc);
			break;

		case OscTypeSine:
			tCycle_create (&osc->mempool, (tCycle**)&osc->theOsc);
			tCycle_init   (osc->mempool->leaf, (tCycle*)osc->theOsc);
			break;

		case OscTypeTri:
			tPBTriangle_create (&osc->mempool, (tPBTriangle**)&osc->theOsc);
			tPBTriangle_init   (osc->mempool->leaf, (tPBTriangle*)osc->theOsc);
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
		tExpSmooth_setFactor(&osc->pitchSmoother, factor);
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

void tOscModule_initToPool(void** const osc, float* const param, float id, tMempool** const mempool)
{
    tMempool* m = *mempool;
    _tOscModule* OscModule = (_tOscModule*) (*osc = (_tOscModule*) mpool_alloc (sizeof (_tOscModule), m));
#ifndef __cplusplus
    memcpy(OscModule->params, param, OscNumParams*sizeof(float));
    int type = roundf(CPPDEREF OscModule->params[OscType]);
#endif __cplusplus
    OscModule->header.uniqueID = id;

    int type =OscTypeSawSquare;
    OscModule->osctype = type;
    OscModule->mempool = m;
    OscModule->invSr = m->leaf->invSampleRate;
    OscModule->sr = m->leaf->sampleRate;

	float val = 64.f;
	float factor = 0.05f;
OscModule->pitchSmoother.curr = val;
OscModule->pitchSmoother.dest = val;
	if (factor < 0.0f) factor = 0.0f;
	if (factor > 1.0f) factor = 1.0f;
	//smooth->baseFactor = factor;
OscModule->pitchSmoother.factor = factor;
	OscModule->pitchSmoother.oneminusfactor = 1.0f - factor;
	switch (type)
	{
		case OscTypeSawSquare:
			tPBSawSquare_create (&OscModule->mempool, (tPBSawSquare**)&OscModule->theOsc);
			tPBSawSquare_init   (OscModule->mempool->leaf, (tPBSawSquare*)OscModule->theOsc);
			break;

		case OscTypeSineTri:
			tPBSineTriangle_create (&OscModule->mempool, (tPBSineTriangle**)&OscModule->theOsc);
			tPBSineTriangle_init   (OscModule->mempool->leaf, (tPBSineTriangle*)OscModule->theOsc);
			break;

		case OscTypeSaw:
			tPBSaw_create (&OscModule->mempool, (tPBSaw**)&OscModule->theOsc);
			tPBSaw_init   (OscModule->mempool->leaf, (tPBSaw*)OscModule->theOsc);
			break;

		case OscTypePulse:
			tPBPulse_create (&OscModule->mempool, (tPBPulse**)&OscModule->theOsc);
			tPBPulse_init   (OscModule->mempool->leaf, (tPBPulse*)OscModule->theOsc);
			break;

		case OscTypeSine:
			tCycle_create (&OscModule->mempool, (tCycle**)&OscModule->theOsc);
			tCycle_init   (OscModule->mempool->leaf, (tCycle*)OscModule->theOsc);
			break;

		case OscTypeTri:
			tPBTriangle_create (&OscModule->mempool, (tPBTriangle**)&OscModule->theOsc);
			tPBTriangle_init   (OscModule->mempool->leaf, (tPBTriangle*)OscModule->theOsc);
			break;

		default:
			break;
	}

    OscModule->header.moduleType = ModuleTypeOscModule;
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
			tPBSawSquare_free((tPBSawSquare**)&OscModule->theOsc);
			break;
		case OscTypeSineTri:
			tPBSineTriangle_free((tPBSineTriangle**)&OscModule->theOsc);
			break;
		case OscTypeSaw:
			tPBSaw_free((tPBSaw**)&OscModule->theOsc);
			break;
		case OscTypePulse:
			tPBPulse_free((tPBPulse**)&OscModule->theOsc);
			break;
		case OscTypeSine:
			tCycle_free((tCycle**)&OscModule->theOsc);
			break;
		case OscTypeTri:
			tPBTriangle_free((tPBTriangle**)&OscModule->theOsc);
			break;
		default:
			break;
    }
    // tExpSmooth_free(&OscModule->pitchSmoother);
    mpool_free((char*)OscModule, OscModule->mempool);
}


// tick function
void tOscModule_tick (tOscModule const osc,float* buffer)
{
	float freqToSmooth = (osc->inputNote + (osc->fine));
	    tExpSmooth_setDest(&osc->pitchSmoother, freqToSmooth);
	    float tempMIDI =  tExpSmooth_tick(&osc->pitchSmoother) + osc->pitchOffset + osc->octaveOffset;

	//    float tempIndexgit F = ((LEAF_clip(-163.0f, tempMIDI, 163.0f) * 100.0f) + 16384.0f);
	//    int tempIndexI = (int)tempIndexF;
	//    tempIndexF = tempIndexF -tempIndexI;
	//    float freqToSmooth1 = osc->mtofTable[tempIndexI & 32767];
	//    float freqToSmooth2 = osc->mtofTable[(tempIndexI + 1) & 32767];
	    float nowFreq = mtof(tempMIDI);// ((freqToSmooth1 * (1.0f - tempIndexF)) + (freqToSmooth2 * tempIndexF));

	    float finalFreq = (nowFreq * osc->harmonicMultiplier ) + osc->freqOffset;
	switch (osc->osctype) {
	case OscTypeSawSquare: {
		tPBSawSquare_setFreq((tPBSawSquare*)osc->theOsc,finalFreq);
		*buffer = tPBSawSquare_tick((tPBSawSquare*)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeSineTri: {
		tPBSineTriangle_setFreq((tPBSineTriangle*)osc->theOsc,finalFreq);
		*buffer = tPBSineTriangle_tick((tPBSineTriangle*)osc->theOsc)* osc->amp;
		break;
	}

	case OscTypeSaw: {
		tPBSaw_setFreq((tPBSaw*)osc->theOsc,finalFreq);
		*buffer = tPBSaw_tick((tPBSaw*)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypePulse: {
		tPBPulse_setFreq((tPBPulse*)osc->theOsc,finalFreq);
		*buffer = tPBPulse_tick((tPBPulse*)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeSine: {
		tCycle_setFreq((tCycle*)osc->theOsc,finalFreq);
		*buffer = tCycle_tick((tCycle*)osc->theOsc)* osc->amp;
		break;
	}
	case OscTypeTri: {
		tPBTriangle_setFreq((tPBTriangle*)osc->theOsc,finalFreq);
		*buffer = tPBTriangle_tick((tPBTriangle*)osc->theOsc)* osc->amp;
		break;
	}


	}


//    osc->freq_set_func(osc->theOsc, finalFreq);
    osc->header.outputs[0] = *buffer;
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




