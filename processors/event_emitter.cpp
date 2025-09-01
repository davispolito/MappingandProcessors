//
// Created by Davis Polito on 9/1/25.
//
#include "event_emitter.h"
#include "defs.h"
#include "EnvModule.h"

void callNoteOn(void* const module,  float velocity)
{

        if (!module) return;

        // Cast to base module type to access moduleType
        uint32_t type = *((uint32_t*)module); // assuming moduleType is the first member of all modules
        switch ((ModuleType)type)
        {
            case ModuleTypeOscModule:
            {
                // tOscModule* osc = (tOscModule*)module;
                // call osc-specific function, e.g.,
                // tOscModule_onNoteOn(osc, velocity);
                break;
            }
            case ModuleTypeLFOModule:
            {
                // tLFOModule* lfo = (tLFOModule*)module;
                // tLFOModule_onNoteOn(lfo, velocity);
                break;
            }
            case ModuleTypeEnvModule:
            {

                // call the function pointer stored in setterFunctions
                tEnvModule_onNoteOn((tEnvModule)module,velocity);
                break;
            }
            case ModuleTypeFilterModule:
            {
                // tFilterModule* filter = (tFilterModule*)module;
                // tFilterModule_onNoteOn(filter, velocity);
                break;
            }
            case ModuleTypeStringModule:
            {
                // tStringModule* str = (tStringModule*)module;
                // tStringModule_onNoteOn(str, velocity);
                break;
            }
            case ModuleTypeVCAModule:
            {
                // tVCAModule* vca = (tVCAModule*)module;
                // tVCAModule_onNoteOn(vca, velocity);
                break;
            }
            default:
                // unknown module type
                break;
        }
    }
