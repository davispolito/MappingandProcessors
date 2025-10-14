/**********************************************************************/
/*            Processor object definitions and functions              */
/*               Authors: Davis Polito and Joshua Warner              */
/**********************************************************************/
#include "processor.h"
#include "sysex_chunks.h"
#ifdef __cplusplus
namespace leaf
{
#endif



void    tProcessor_init (tProcessor** const pr, LEAF* const leaf)
{
	tProcessor_initToPool(pr, &leaf->mempool);
}

void    tProcessor_initToPool   (tProcessor** const pr, tMempool** const mp)
{
    tMempool* m = *mp;
    tProcessor* p = *pr = (tProcessor*) mpool_alloc(sizeof(tProcessor), m);
    p->mempool = m;
}


    void processor_to_preset (tProcessor* proc, tProcessorPresetUnion* preset)
    {

        preset->data.processorTypeID = proc->processorTypeID;
        preset->data.processorUniqueID = proc->processorUniqueID;
        preset->data.index = proc->index;
        preset->data.proc_chain = proc->proc_chain;
        for (int i = 0; i < MAX_NUM_PARAMS; i++)
        {
            preset->data.params[i] = CPPDEREF proc->inParameters[i];
        }
    }

    void preset_to_processor (tProcessorPresetUnion* preset, tProcessor* proc)
    {
        proc->processorTypeID = preset->data.processorTypeID;
        proc->processorUniqueID = preset->data.processorUniqueID;
        proc->index = preset->data.index;
        proc->proc_chain = preset->data.proc_chain;
        for (int i = 0; i < MAX_NUM_PARAMS; i++)
        {
            CPPDEREF proc->inParameters[i] =  preset->data.params[i];
        }
    }
    void preset_to_processor_ (tProcessorPreset* preset, tProcessor* proc)
{
    proc->processorTypeID = preset->processorTypeID;
    proc->processorUniqueID = preset->processorUniqueID;
    proc->index = preset->index;
    proc->proc_chain = preset->proc_chain;
    for (int i = 0; i < MAX_NUM_PARAMS; i++)
    {
       CPPDEREF  proc->inParameters[i] = preset->params[i];
    }
}

    void splitProcessorPreset (const tProcessorPreset* preset, tProcessorPreset7Bit* preset7Bit)
    {


        // Split processorTypeID (8 bits -> 2 chunks of 7 bits)
        splitUint8To7bit (preset->processorTypeID, preset7Bit->processorTypeID);

        // Split processorUniqueID (8 bits -> 2 chunks of 7 bits)
        splitUint8To7bit (preset->processorUniqueID, preset7Bit->processorUniqueID);

        // Split proc_chain (8 bits -> 2 chunks of 7 bits)
        splitUint8To7bit (preset->proc_chain, preset7Bit->proc_chain);

        // Split index (8 bits -> 2 chunks of 7 bits)
        splitUint8To7bit (preset->index, preset7Bit->index);

        // Split each parameter (float -> 5 chunks of 7 bits)
        for (int i = 0; i < MAX_NUM_PARAMS; i++)
        {
            splitFloatTo7bit (preset->params[i], preset7Bit->params[i]);
        }
    }
    void unsplitProcessorPreset (const tProcessorPreset7Bit* preset7Bit, tProcessorPreset* preset)
    {
        // Reconstruct processorTypeID (2 chunks -> 8 bits)
        preset->processorTypeID = unsplitUint8 (preset7Bit->processorTypeID);

        // Reconstruct processorUniqueID (2 chunks -> 8 bits)
        preset->processorUniqueID = unsplitUint8 (preset7Bit->processorUniqueID);

        // Reconstruct proc_chain (2 chunks -> 8 bits)
        preset->proc_chain = unsplitUint8 (preset7Bit->proc_chain);

        // Reconstruct index (2 chunks -> 8 bits)
        preset->index = unsplitUint8 (preset7Bit->index);

        // Reconstruct each parameter (5 chunks -> float)
        for (int i = 0; i < MAX_NUM_PARAMS; i++)
        {
            preset->params[i] = unsplitFloat (preset7Bit->params[i]);
        }
    }
void tProcessor_free (tProcessor** const pr) {
    tProcessor* p = *pr;
    mpool_free((char*)p, p->mempool);
    *pr = NULL;
}

#ifdef __cplusplus
}
#endif
