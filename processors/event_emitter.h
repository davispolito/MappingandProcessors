//
// Created by Davis Polito on 5/5/25.
//

#ifndef EVENT_EMITTER_H
#define EVENT_EMITTER_H
#include "processor.h"
typedef struct tEventEmitter {
    uint8_t uuid; //ID for the mapping's parameter
    leaf::tProcessor listeners[MAX_NUM_VOICES][MAX_EVENT_LISTENERS];
    uint8_t numListeners;
} tEventEmitter;
#endif //NOTE_EMITTER_H
