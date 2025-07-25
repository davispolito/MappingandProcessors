/**********************************************************************/
/*                           Mapping header                           */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef MAPPING_HEADER
#define MAPPING_HEADER


#include <stdint.h>
#include "processor.h"
#include "defs.h"
    #ifdef __cplusplus
    #include <cstdio>
namespace leaf
{
    #endif

    typedef struct AudioRouting {
        ATOMIC_FLOAT* audio;
    } tAudioRouting;


    typedef struct Mapping
    {
        tSetter setter; // Setter function for the parameter of the mapping
        uint8_t uuid; //ID for the mapping's parameter
        uint8_t destinationProcessorUniqueID; // ID for destination processors that param is in
        uint8_t index;
        void* destObject; // OUT destination for the mapping
        uint8_t paramID;
        uint8_t numUsedSources; // Number of active sources for the mapping
        uint8_t inUUIDS[MAX_NUM_SOURCES];
        ATOMIC_FLOAT bipolarOffset[MAX_NUM_SOURCES];
        ATOMIC_FLOAT* inSources[MAX_NUM_SOURCES]; // IN sources from processors
        ATOMIC_FLOAT CPPDEREF scalingValues[MAX_NUM_SOURCES]; // Scaling for the IN sources
        ATOMIC_FLOAT* initialVal; // The mapping's initial value
        tMempool mempool;
    } tMapping;

    typedef struct MappingPreset
    {
        uint8_t index;
        uint8_t uuid; // ID for this processors
        uint8_t destinationUUID;
        uint8_t destParamID;
        uint8_t numUsedSources;
        uint8_t inUUIDs[MAX_NUM_SOURCES];
        float bipolarOffset[MAX_NUM_SOURCES];
        float scalingValues[MAX_NUM_SOURCES];

    } tMappingPreset;

    typedef union {
        tMappingPreset data;
        uint8_t bytes[sizeof (tMappingPreset)];
    } tMappingPresetUnion;


// 7-Bit representation of tMappingPreset
    typedef struct tMappingPreset7Bit {
        uint8_t index[2];                        // 2 chunks of 7 bits
        uint8_t uuid[2];                         // 2 chunks of 7 bits
        uint8_t destinationUUID[2];              // 2 chunks of 7 bits
        uint8_t destParamID[2];                  // 2 chunks of 7 bits
        uint8_t numUsedSources[2];               // 2 chunks of 7 bits
        uint8_t inUUIDs[MAX_NUM_SOURCES][2];     // Each UUID -> 2 chunks of 7 bits
        uint8_t bipolarOffset[MAX_NUM_SOURCES][5]; // Each float -> 5 chunks of 7 bits
        uint8_t scalingValues[MAX_NUM_SOURCES][5]; // Each float -> 5 chunks of 7 bits
    } tMappingPreset7Bit;


    typedef struct  _tMappingReceiver{
       uint8_t receivedData[sizeof(tMappingPreset7Bit)]; //
        size_t receivedDataSize;
    }tMappingReceiver;

    void splitMappingPreset(const tMappingPreset* preset, tMappingPreset7Bit* preset7Bit);
    void unsplitMappingPreset(const tMappingPreset7Bit* preset7Bit, tMappingPreset* preset);
    void mapping_to_preset(tMapping *mapping, tMappingPresetUnion * preset);
    
    void preset_to_mapping(tMappingPresetUnion preset, tMapping *mapping);
    void preset_to_mapping_(const tMappingPreset *preset, tMapping *mapping);
    void processMapping (tMapping* mapping);

    void tMapping_init (tMapping** const mapping, LEAF* const leaf);
    void tMapping_free (tMapping** const mapping);


    void tMapping_initToPool (tMapping** const mapping, tMempool* const mp);
    void tMappingAdd (tMapping* mapping, tProcessor* outputProcessor, tProcessor* destProcessor, uint8_t destParam,
        uint8_t source, LEAF *leaf, ATOMIC_FLOAT CPPDEREF scalingptr);
    void tMappingUpdateDest(tMapping* mapping, uint8_t source,
         tProcessor *newDestProcessor, uint8_t destParam,
        ATOMIC_FLOAT CPPDEREF scalingValue);
    void tMappingAdd_(tMapping *mapping, ATOMIC_FLOAT* insource, uint8_t insource_uuid,
        ATOMIC_FLOAT* dest_param, uint8_t dest_uuid,tSetter setter, uint8_t dest_param_index,
        void* obj, LEAF* leaf,
    ATOMIC_FLOAT CPPDEREF scalingValue);

#ifdef __cplusplus

}
#endif




#endif
