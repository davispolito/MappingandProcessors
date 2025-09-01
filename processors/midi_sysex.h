// Created by Davis Polito on 4/23/25.
//
//

#ifndef MIDI_SYSEX_H
#define MIDI_SYSEX_H
#include <mapping.h>
#include <SimpleOscModule.h>

#ifdef __cplusplus
namespace leaf {
#endif
    // void receiveProcessorPreset(tProcessorReceiver *receiver, tProcessor **output, uint8_t *data, size_t size,
    //                             LEAF *leaf);
    //
    void receiveMappingPreset(tMappingReceiver *receiver, tMapping **output, uint8_t *data, size_t size, LEAF *leaf);
#ifdef __cplusplus
}
#endif
#endif //
