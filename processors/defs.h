/**********************************************************************/
/*                       Macros and TypeDefs                          */
/*              Authors: Davis Polito and Joshua Warner               */
/**********************************************************************/

#ifndef DEFS_HEADER
#define DEFS_HEADER
#ifdef __cplusplus
    #include <atomic>
    #define ATOMIC_FLOAT std::atomic<float>
#define CPPDEREF *
#define LEAF_NAMESPACE leaf::
#else
#define LEAF_NAMESPACE
#define CPPDEREF
    #define ATOMIC_FLOAT float
#endif
#ifdef __cplusplus
extern "C" {
#endif



// Maximum number of parameters for a processors object
#define MAX_NUM_PARAMS 16

// Maximum number of processors sources for a mapping object
#define MAX_NUM_SOURCES 3
#define MAX_NUM_VOICES 12

#define EVENT_WATCH_INDEX 0
#define MAX_EVENT_LISTENERS 30
// Defines 'pTickFunction' to be the name of a new function pointer type
typedef void (*tTickFuncReturningVoid)(void*, float*); // used for processors (don't return audio because that will be stored in the modules' output array

typedef float (*tTickFuncReturningFloat)(void*); //used inside modules to wrap internal tick functions as function pointers

    typedef float (*tTickFuncFloatInReturningFloat)(void*, float in); //used inside modules to wrap internal tick functions as function pointers
// Defines `_tParameter` to be the name of a new function pointer type
typedef void (*tSetter)(void*, float);

typedef enum {
    ModuleTypeOscModule,
    ModuleTypeLFOModule,
    ModuleTypeEnvModule,
    ModuleTypeFilterModule,
    ModuleTypeStringModule,
    ModuleTypeVCAModule,
    ModuleTypeSoftClipModule,

} ModuleType;

typedef enum {
    PROCTAG,
    MAPTAG
} BYTETAGS;
    typedef struct ModuleHeader {
        uint32_t moduleType;
        uint32_t uniqueID;
        ATOMIC_FLOAT CPPDEREF params[MAX_NUM_PARAMS];
        tSetter setterFunctions[MAX_NUM_PARAMS]; // Array containing setter functions
        ATOMIC_FLOAT inputs[1];
        ATOMIC_FLOAT outputs[1];
    }ModuleHeader;
#ifdef __cplusplus
}
#endif
#endif
