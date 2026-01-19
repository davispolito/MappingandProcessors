cmake_minimum_required(VERSION 3.26.4)
project(LEAF_modules)
option(MODULE_ENABLE_TESTS "Enable tests modules" ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")


# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Globbing source files
file(GLOB SRC CONFIGURE_DEPENDS "*.h" "*.c")


file(GLOB _MODULES CONFIGURE_DEPENDS "modules/*.h" "modules/*.cpp")

# Add subdirectories for libraries and processors
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/LEAF/)
# Add library procUtils with processor.c and mapping.c
add_library(procUtils
        modules/EnvModule.cpp
        modules/SimpleOscModule.cpp
        modules/FilterModule.cpp
        modules/LFOModule.cpp
        modules/VCAModule.cpp
        modules/StringModule.cpp
        processors/mapping.cpp
        processors/sysex_chunks.cpp
        processors/midi_sysex.cpp
        processors/funcmaps.cpp
        processors/event_emitter.cpp
        modules/SoftClipModule.cpp
)

# Include directories for procUtils
target_include_directories(procUtils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        "${CMAKE_CURRENT_LIST_DIR}/processors/"
        "${CMAKE_CURRENT_LIST_DIR}/modules/"
        "${CMAKE_CURRENT_LIST_DIR}/LEAF/"

)

# Link library LEAF to procUtils
target_link_libraries(procUtils PRIVATE
        LEAF
)

# Create main library
add_library(${PROJECT_NAME} ${SRC})
#message("include ${CMAKE_CURRENT_SOURCE_DIR}/modules")
# Include directories for the main library
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/LEAF/leaf
        ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

# Link libraries to the main project
target_link_libraries(${PROJECT_NAME} PUBLIC LEAF procUtils)

if (MODULE_ENABLE_TESTS)

    # Enable testing
    enable_testing()

    # Add subdirectory for tests
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test")

endif()
