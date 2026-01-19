//
// Created by Davis Polito on 4/23/25.
//
//
// Created by Davis Polito on 2/13/25.
//
#include "funcmaps.h"

#ifdef __cplusplus
namespace leaf
{
#endif
    //createProcFunc proc_init_map[] = {
    //    (createProcFunc)tOscModule_processorInit,
    //    (createProcFunc)tOscModule_processorInit,
    //    (createProcFunc)tOscModule_processorInit};
    //createModuleFunc module_init_map[] = {
    //    (createModuleFunc)tOscModule_init,
    //    (createModuleFunc)tOscModule_init,
    //    (createModuleFunc)tOscModule_init};

    //each file that includes this will getits own copy of it. this wastes memory. Should find a singleton style solution
     createProcFunc proc_init_map[] = {
        (createProcFunc) tOscModule_processorInit,
        (createProcFunc) tFiltModule_processorInit,
        (createProcFunc) tEnvModule_processorInit,
        (createProcFunc) tLFOModule_processorInit,
         (createProcFunc) tStringModule_processorInit,
         (createProcFunc) tVCAModule_processorInit,
         (createProcFunc) tSoftClipModule_processorInit,
    };
    createModuleFunc module_init_map[] = {
        (createModuleFunc) tOscModule_init,
        (createModuleFunc) tFiltModule_init,
        (createModuleFunc) tEnvModule_init,
        (createModuleFunc) tLFOModule_init,
        (createModuleFunc) tStringModule_init,
        (createModuleFunc) tVCAModule_init,
        (createModuleFunc) tSoftClipModule_init
    };
//
//void createProcessor(float *params, tProcessor* proc,LEAF* leaf)
//{
//    float type = params[0];
//    float id = params[1];
//    float *module_params = &params[2];
//    void* mod;
//    module_init_map[(int)type](&mod, module_params, id, leaf);
//    proc_init_map[(int)type](mod, proc);
//}


#ifdef __cplusplus
}
#endif
