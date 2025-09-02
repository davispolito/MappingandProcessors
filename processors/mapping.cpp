/**********************************************************************/
/*              Mapping object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#include <assert.h>
#include "mapping.h"
#include "sysex_chunks.h"
#include "leaf.h"
#include "SimpleOscModule.h"
#include "VCAModule.h"
#include "EnvModule.h"
#include "FilterModule.h"
#include "StringModule.h"
#include "LFOModule.h"
// Process mapping function
#ifdef __cplusplus
namespace leaf
{
#endif


void tMapping_init (tMapping** const mapping, LEAF* const leaf)
{
	tMapping_initToPool(mapping, &leaf->mempool);
}

void tMapping_initToPool (tMapping** const mapping, tMempool* const mp)
{
    _tMempool* m = *mp;
    tMapping* map = *mapping = (tMapping*) mpool_alloc(sizeof(tMapping), m);
    map->mempool = m;
    map->numUsedSources = 0;
    map->uuid = 255;
}

void tMapping_setParameter(void* module, int paramID, float value) {
    uint32_t type = *((uint32_t*)module);
    switch (type)
    {
        case ModuleTypeOscModule:
            tOscModule_setParameter((tOscModule) module,(OscParams) paramID, value);
            break;

        case ModuleTypeLFOModule:
            tLFOModule_setParameter((tLFOModule) module, (LFOParams)paramID, value);
            break;

        case ModuleTypeEnvModule:
            tEnvModule_setParameter((tEnvModule) module, (EnvParams)paramID, value);
            break;

        case ModuleTypeFilterModule:
            tFiltModule_setParameter((tFiltModule) module,(FiltParams) paramID, value);
            break;

        case ModuleTypeStringModule:
            tStringModule_setParameter((tStringModule) module, (StringModelParams) paramID, value);
            break;

        default:
            // handle invalid type
            break;
    }
}


void processMapping (tMapping* mapping)
{
    float sum = *mapping->initialVal;

    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        sum += (*mapping->inSources[i] * CPPDEREF mapping->scalingValues[i]) + mapping->bipolarOffset[i];
    }
    tMapping_setParameter(mapping->destObject,mapping->paramID,sum);
    // mapping->setter(mapping->destObject, sum);
}
void tMapping_free (tMapping** const mapping) {
   mpool_free((char*)*mapping, (*mapping)->mempool);
}

void tMappingAdd_(tMapping *mapping, ATOMIC_FLOAT* insource, uint8_t insource_uuid,  ATOMIC_FLOAT* dest_param, uint8_t dest_uuid, tSetter setter, uint8_t dest_param_index, void* obj, LEAF* leaf, ATOMIC_FLOAT CPPDEREF scalingValue)
 {
     if (mapping->uuid == 255)
         mapping->uuid = getNextUuid(leaf);
    int currIndex = mapping->numUsedSources;
     // Updates the _tMapping struct with the given arguments
     mapping->inSources[currIndex] = insource;
     mapping->inUUIDS[currIndex] = insource_uuid;


 //    mapping->scalingValues[0] = scalingValues[0];
 //    mapping->scalingValues[1] = scalingValues[1];
 //    mapping->scalingValues[2] = scalingValues[2];

     mapping->initialVal = dest_param;
     // mapping->setter = setter;
     mapping->destinationProcessorUniqueID = dest_uuid;
     mapping->paramID = dest_param_index;
     mapping->destObject = obj;
#ifdef __cplusplus
     if (scalingValue != nullptr)
#endif

         mapping->scalingValues[currIndex] =  scalingValue;
     mapping->numUsedSources++;
 }



// Initializes the mapping `mapping` to map from the output of 
// `outputProcessor` to the input of `destProcessor` on the parameter 
// `destParam`.  The mapping scales this value by the factors in
// `scalingValues`.
// void tMappingAdd(tMapping *mapping, tProcessor *outputProcessor,
//     tProcessor *destProcessor, uint8_t destParam, uint8_t source, LEAF* leaf, ATOMIC_FLOAT CPPDEREF scalingValue)
//
// {
//
//     if (mapping->uuid == 255)
//         mapping->uuid = getNextUuid(leaf);
//     // Updates the _tMapping struct with the given arguments
//     mapping->inSources[source] = &outputProcessor->outParameters[0];
//     mapping->inUUIDS[source] = outputProcessor->processorUniqueID;
//     mapping->numUsedSources++;
//
// //    mapping->scalingValues[0] = scalingValues[0];
// //    mapping->scalingValues[1] = scalingValues[1];
// //    mapping->scalingValues[2] = scalingValues[2];
//
//     mapping->initialVal = CPPDEREF &destProcessor->inParameters[destParam];
//     mapping->setter = destProcessor->setterFunctions[destParam];
//     mapping->destinationProcessorUniqueID = destProcessor->processorUniqueID;
//     mapping->paramID = destParam;
//     mapping->destObject = destProcessor->object;
// #ifdef __cplusplus
//      if (scalingValue != nullptr)
// #endif
//         mapping->scalingValues[source] = scalingValue;
// }


//
// void tMappingUpdateDest(tMapping* mapping, uint8_t source,
//     tProcessor *newDestProcessor, uint8_t destParam, ATOMIC_FLOAT CPPDEREF scalingValue) {
//     mapping->initialVal = CPPDEREF &newDestProcessor->inParameters[destParam];
//     mapping->setter = newDestProcessor->setterFunctions[destParam];
//     mapping->destinationProcessorUniqueID = newDestProcessor->processorUniqueID;
// #ifdef __cplusplus
//      if (scalingValue != nullptr)
// #endif
//         mapping->scalingValues[source] = scalingValue;
// }
void mapping_to_preset(tMapping *mapping, tMappingPresetUnion * preset)
{
    preset->data.uuid = mapping->uuid;
    preset->data.destinationUUID = mapping->destinationProcessorUniqueID;
    preset->data.destParamID = mapping->paramID;
    preset->data.numUsedSources = mapping->numUsedSources;
    preset->data.index = mapping->index;
    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        preset->data.inUUIDs[i] = mapping->inUUIDS[i];
        preset->data.bipolarOffset[i] = mapping->bipolarOffset[i];
        preset->data.scalingValues[i] = CPPDEREF mapping->scalingValues[i];
    }
}

void preset_to_mapping(tMappingPresetUnion preset, tMapping *mapping)
{
    mapping->uuid = preset.data.uuid;
    mapping->destinationProcessorUniqueID = preset.data.destinationUUID;
    mapping->paramID = preset.data.destParamID;
    mapping->numUsedSources = preset.data.numUsedSources;
    mapping->index = preset.data.index;
    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        mapping->inUUIDS[i] = preset.data.inUUIDs[i];
        mapping->bipolarOffset[i] = preset.data.bipolarOffset[i];
        CPPDEREF mapping->scalingValues[i] =  preset.data.scalingValues[i];
    }
}
    void preset_to_mapping_(const tMappingPreset* preset, tMapping *mapping)
{
    mapping->uuid = preset->uuid;
    mapping->destinationProcessorUniqueID = preset->destinationUUID;
    mapping->paramID = preset->destParamID;
    mapping->numUsedSources = preset->numUsedSources;
    mapping->index = preset->index;
    for (int i = 0; i < mapping->numUsedSources; i++)
    {
        mapping->inUUIDS[i] = preset->inUUIDs[i];
        mapping->bipolarOffset[i] = preset->bipolarOffset[i];
       CPPDEREF mapping->scalingValues[i] = preset->scalingValues[i];
    }
}

void splitMappingPreset(const tMappingPreset* preset, tMappingPreset7Bit* preset7Bit) {
    // Split basic fields
    splitUint8To7bit(preset->index, preset7Bit->index);
    splitUint8To7bit(preset->uuid, preset7Bit->uuid);
    splitUint8To7bit(preset->destinationUUID, preset7Bit->destinationUUID);
    splitUint8To7bit(preset->destParamID, preset7Bit->destParamID);
    splitUint8To7bit(preset->numUsedSources, preset7Bit->numUsedSources);

    // Split inUUIDs
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitUint8To7bit(preset->inUUIDs[i], preset7Bit->inUUIDs[i]);
    }

    // Split bipolarOffset
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitFloatTo7bit(preset->bipolarOffset[i], preset7Bit->bipolarOffset[i]);
    }

    // Split scalingValues
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        splitFloatTo7bit(preset->scalingValues[i], preset7Bit->scalingValues[i]);
    }
}

void unsplitMappingPreset(const tMappingPreset7Bit* preset7Bit, tMappingPreset* preset) {
    // Reconstruct basic fields
    preset->index = unsplitUint8(preset7Bit->index);
    preset->uuid = unsplitUint8(preset7Bit->uuid);
    preset->destinationUUID = unsplitUint8(preset7Bit->destinationUUID);
    preset->destParamID = unsplitUint8(preset7Bit->destParamID);
    preset->numUsedSources = unsplitUint8(preset7Bit->numUsedSources);

    // Reconstruct inUUIDs
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->inUUIDs[i] = unsplitUint8(preset7Bit->inUUIDs[i]);
    }

    // Reconstruct bipolarOffset
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->bipolarOffset[i] = unsplitFloat(preset7Bit->bipolarOffset[i]);
    }

    // Reconstruct scalingValues
    for (int i = 0; i < MAX_NUM_SOURCES; i++) {
        preset->scalingValues[i] = unsplitFloat(preset7Bit->scalingValues[i]);
    }
}
#ifdef __cplusplus
}
#endif
