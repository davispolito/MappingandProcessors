//
// Created by Davis Polito on 4/23/25.
//
#include "midi_sysex.h"

#include <mapping.h>

#include "funcmaps.h"
#include "SimpleOscModule.h"
#ifdef __cplusplus
namespace leaf {
#endif


    // void receiveProcessorPreset(tProcessorReceiver *receiver, tProcessor **output, uint8_t *data, size_t size,
    //                             LEAF *leaf) {
    //     if (receiver->receivedDataSize + (size - 4) <= sizeof(tProcessorPreset7Bit)) {
    //         memcpy(receiver->receivedData + receiver->receivedDataSize,
    //             data + 3,//skip sysex tag, type tag, and info tag
    //             size - 4);
    //         receiver->receivedDataSize += size - 4;
    //     }
    //     if (receiver->receivedDataSize == sizeof(tProcessorPreset7Bit)) {
    //         tProcessorPreset7Bit preset7Bit;
    //         memcpy(&preset7Bit, receiver->receivedData, sizeof(tProcessorPreset7Bit));
    //         tProcessorPreset preset;
    //         unsplitProcessorPreset(&preset7Bit, &preset);
    //         tProcessor_init(output, leaf);
    //         void *module;
    //         module_init_map[preset.processorTypeID]((void **) &module, preset.params, preset.processorUniqueID, leaf);
    //         proc_init_map[preset.processorTypeID]((module), *output);
    //         //preset_to_processor_(&preset,output);
    //         (*output)->processorUniqueID = preset.processorUniqueID;
    //         (*output)->index = preset.index;
    //         (*output)->proc_chain = preset.proc_chain;
    //         receiver->receivedDataSize = 0;
    //
    //         //  for (int i = 0; i < sizeof(tProcessorPreset7Bit); i++) {
    //         //    receiver->receivedData[i] = 0;
    //         //}
    //     }
    // }


    void receiveMappingPreset(tMappingReceiver *receiver, tMapping **output, uint8_t *data, size_t size, LEAF *leaf) {
        //does not contain an extra header right now bcus it can be packed into a single chunk
        if (receiver->receivedDataSize + (size - 4) <= sizeof(tMappingPreset7Bit)) {
            memcpy(receiver->receivedData + receiver->receivedDataSize,
                data + 3,//skip sysex tag, type tag, and info tag
                size - 4);
            receiver->receivedDataSize += size - 4;
        }
        if (receiver->receivedDataSize == sizeof(tMappingPreset7Bit)) {
            tMappingPreset7Bit preset7Bit;
            memcpy(&preset7Bit, receiver->receivedData, sizeof(tMappingPreset7Bit));
            tMappingPreset preset;
            unsplitMappingPreset(&preset7Bit, &preset);
            tMapping_init(output, leaf);
            preset_to_mapping_(&preset,*output);
        }
    }

#ifdef __cplusplus
}
#endif
