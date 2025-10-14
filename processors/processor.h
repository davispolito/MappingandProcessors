/**********************************************************************/
/*                          Processor header                          */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/

#ifndef PROCESSOR_HEADER
#define PROCESSOR_HEADER

#include "defs.h"
#include "leaf.h"
#ifdef __cplusplus
namespace leaf{
#endif
// Defines a struct Processor as a _tProcessor object
typedef struct Processor
{
    uint8_t processorUniqueID; // ID for this processors
    uint8_t processorTypeID; // ID for this processors
    uint8_t proc_chain;
    uint8_t index;
    void *object; // Processor's aliased Leaf module object
    tTickFuncReturningVoid tick; // The object's tick function
    uint8_t numSetterFunctions; // Number of setter functions used by
    // the processors's object
    tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing
    // pointers to the setter functions for the processors's object


    ATOMIC_FLOAT CPPDEREF * inParameters; //  points to parameter array of the actual object
    ATOMIC_FLOAT* outParameters; // OUT sources for the processors
    ATOMIC_FLOAT* audioInParameters; //audio in params
    tMempool* mempool;
} tProcessor;

typedef struct ProcessorPreset
{

    uint8_t processorTypeID; // ID for this processors
    uint8_t processorUniqueID; // ID for this processors
    uint8_t proc_chain;
    uint8_t index;
    float params[MAX_NUM_PARAMS];

} tProcessorPreset;

typedef union
{
    tProcessorPreset data;
    uint8_t bytes[sizeof(tProcessorPreset)];
} tProcessorPresetUnion;
// Struct for holding the 7-bit chunked version of tProcessorPreset
typedef struct _tProcessorPreset7Bit {
        // Chunks for each field
        uint8_t processorTypeID[2];             // 2 chunks of 7-bits
        uint8_t processorUniqueID[2];           // 2 chunks of 7-bits
        uint8_t proc_chain[2];                  // 2 chunks of 7-bits
        uint8_t index[2];                       // 2 chunks of 7-bits
        uint8_t params[MAX_NUM_PARAMS][5];      // Each float -> 5 chunks of 7-bits
    } tProcessorPreset7Bit;


    typedef struct _tProcessorReceiver {
       uint8_t receivedData[sizeof(tProcessorPreset7Bit)]; //
        size_t receivedDataSize;
    }tProcessorReceiver;

    //returns 1 if finished

// Function definitions for split and unsplit
void splitProcessorPreset(const tProcessorPreset *preset, tProcessorPreset7Bit *preset7Bit);
void unsplitProcessorPreset(const tProcessorPreset7Bit *preset7Bit, tProcessorPreset *preset);


typedef void (*createProcFunc)(void* const module, tProcessor* proc);


typedef void (*createModuleFunc)(void** const module, float* const params, float id, LEAF* const leaf);


void    tProcessor_init (tProcessor** const pr, LEAF* const leaf);


void    tProcessor_initToPool   (tProcessor** const pr, tMempool** const mp);

    void tProcessor_free(tProcessor** pr);


void processor_to_preset(tProcessor *, tProcessorPresetUnion * );

void preset_to_processor(tProcessorPresetUnion *, tProcessor *);

#ifdef __cplusplus
}
#endif
#endif
