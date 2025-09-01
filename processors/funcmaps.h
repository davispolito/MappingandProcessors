//
// Created by Davis Polito on 7/11/24.
//

#ifndef UNTITLED_FUNCMAPS_H
#define UNTITLED_FUNCMAPS_H
#include "EnvModule.h"
#include "FilterModule.h"
#include "LFOModule.h"
#include "SimpleOscModule.h"
#include "StringModule.h"
#include "VCAModule.h"
#include "processor.h"


#ifdef __cplusplus
namespace leaf
{
#endif
    //each file that includes this will getits own copy of it. this wastes memory. Should find a singleton style solution
    extern createProcFunc proc_init_map[];
    extern createModuleFunc module_init_map[];

#ifdef __cplusplus
}
        #endif
#endif //UNTITLED_FUNCMAPS_H
