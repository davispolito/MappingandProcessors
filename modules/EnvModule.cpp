//
// Created by Jeffrey Snyder on 7/9/24.
//


#include "EnvModule.h"

#include <assert.h>

void tEnvModule_init(void** const env, float* params, float id, LEAF* const leaf)
{
    tEnvModule_initToPool(env, params, id, &leaf->mempool);
}


void tEnvModule_free(void** const env)
{
    _tEnvModule* EnvModule =static_cast<_tEnvModule *>(*env);
    //tADSRT_free(&EnvModule->theEnv);
    mpool_free((char*)EnvModule, EnvModule->mempool);
}
//tick function
void tEnvModule_tick (tEnvModule const env)
{
    env->outputs[0] = tADSRT_tick(&env->theEnv);
}

//special noteOnFunction
void tEnvModule_onNoteOn(tEnvModule const env, float velocity)
{
    float envVel = velocity;
    if (velocity > 0.0001f)
    {
        if (env->params[EnvUseVelocity] == 0) envVel = 1.f; // any way to avoid this double branching?
        tADSRT_on(&env->theEnv, envVel);
    }
    else
    {
        tADSRT_off(&env->theEnv);
    }
}


// Non-modulatable setters
void tEnvModule_setExpTableLocation (tEnvModule const env, const float* tableAddress, uint32_t const tableSize)
{
    env->theEnv.exp_buff = tableAddress;
    env->theEnv.buff_size = tableSize;
    env->theEnv.buff_sizeMinusOne = tableSize - 1;
    env->theEnv.bufferSizeDividedBySampleRateInMs = env->theEnv.buff_size / (env->theEnv.sampleRate * 0.001f);
}

void tEnvModule_setTimeScalingTableLocation (tEnvModule const env, const float* tableAddress, uint32_t const tableSize)
{
    env->envTimeTableAddress = tableAddress;
    env->envTimeTableSizeMinusOne = (float) (tableSize - 1);
}

void tEnvModule_setSampleRate (tEnvModule const env, float sr)
{
    //how to handle this? if then cases for different types?

}
void tEnvModule_setParameter(tEnvModule const  env, int parameter_id, float input)
{
    switch (parameter_id)
    {
        case EnvEventWatchFlag:
        {
            // handled by onnoteon listener
            break;
        }

        case EnvAttack:
        {
            input *= env->envTimeTableSizeMinusOne;
            int const inputInt = (int)input;
            float const inputFloat = (float)inputInt - input;
            int const nextPos = LEAF_clip(0.0f, inputInt + 1.0f, env->envTimeTableSizeMinusOne);
            float const theValue = LEAF_clip(0.1f, (env->envTimeTableAddress[inputInt] * (1.0f - inputFloat)) + (env->envTimeTableAddress[nextPos] * inputFloat), 10.0f);
            tADSRT_setAttack(&env->theEnv, theValue + 0.001f);
            break;
        }

        case EnvDecay:
        {
            input *= env->envTimeTableSizeMinusOne;
            int const inputInt = (int)input;
            float const inputFloat = (float)inputInt - input;
            int const nextPos = LEAF_clip(0.0f, inputInt + 1.0f, env->envTimeTableSizeMinusOne);
            float const theValue = LEAF_clip(0.1f, (env->envTimeTableAddress[inputInt] * (1.0f - inputFloat)) + (env->envTimeTableAddress[nextPos] * inputFloat), 10.0f);
            tADSRT_setDecay(&env->theEnv, theValue + 0.001f);
            break;
        }

        case EnvSustain:
        {
            tADSRT_setSustain(&env->theEnv, LEAF_clip(0.0f, input, 1.0));
            break;
        }

        case EnvRelease:
        {
            input *= env->envTimeTableSizeMinusOne;
            int const inputInt = (int)input;
            float const inputFloat = (float)inputInt - input;
            int const nextPos = LEAF_clip(0.0f, inputInt + 1.0f, env->envTimeTableSizeMinusOne);
            float const theValue = LEAF_clip(0.1f, (env->envTimeTableAddress[inputInt] * (1.0f - inputFloat)) + (env->envTimeTableAddress[nextPos] * inputFloat), 10.0f);
            tADSRT_setRelease(&env->theEnv, theValue + 0.001f);
            break;
        }

        case EnvLeak:
        {
            tADSRT_setLeakFactor(&env->theEnv,  0.99995f + 0.00005f*(1.f-LEAF_clip(0.0f, input, 1.0)));
            break;
        }

        case EnvShapeAttack:
        {

            break;
        }

        case EnvShapeRelease:
        {

            break;
        }

        case EnvUseVelocity:
        {

            break;
        }

        default:
            break;
    }
}


void tEnvModule_initToPool(void** const env, float* const params, float id, tMempool** const mempool)
{
    tMempool* m = *mempool;
    _tEnvModule* EnvModule = static_cast<_tEnvModule *>(*env = (_tEnvModule*) mpool_alloc(sizeof(_tEnvModule), m));
#ifndef __cplusplus
    memcpy(EnvModule->params, params, EnvNumParams);
#endif __cplusplus
    EnvModule->mempool = m;

    EnvModule->uniqueID = id;
    tADSRT_set(&EnvModule->theEnv, 1.0f,1000.0f,1.0f,1000.0f, NULL, 2048,(*mempool)->leaf);

    EnvModule->tick = reinterpret_cast<tTickFuncReturningFloat>(tADSRT_tick);
    EnvModule->setterFunctions[EnvEventWatchFlag] =(tSetter) &tEnvModule_onNoteOn;

    tEnvModule_setExpTableLocation ( EnvModule, &__leaf_table_exp_decay[0], EXP_DECAY_TABLE_SIZE);
    tEnvModule_setTimeScalingTableLocation( EnvModule,&__leaf_table_attack_decay_inc[0], ATTACK_DECAY_INC_TABLE_SIZE);
    EnvModule->moduleType = ModuleTypeEnvModule;
}



void tEnvModule_processorInit(tEnvModule const env, leaf::tProcessor* processor)
{
    // Checks that arguments are valid
    assert (env != NULL);
    assert (processor != NULL);

    processor->processorUniqueID = env->uniqueID;
    processor->object = env;
    processor->numSetterFunctions = EnvNumParams;
    processor->tick = reinterpret_cast<tTickFuncReturningVoid>(tEnvModule_tick);
    processor->setterFunctions[EnvEventWatchFlag] =(tSetter) &tEnvModule_onNoteOn;

    processor->inParameters = env->params;
    processor->outParameters = env->outputs;
    processor->processorTypeID = ModuleTypeEnvModule;
}
