cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Spire_SCIRun)

# Could potentially perform an include(SpirePM) here if we are given the
# appropriate module path. We could then use a macro for most of the code below,
# and only need to specify the source files to be compiled. Everything else
# would be handled by the spire module system.

#-------------------------------------------------------------------------------
# Module sources
#-------------------------------------------------------------------------------

# Feel free to use any method of defining sources.
file (GLOB Source
  "src/*.cpp"
  "src/*.h"
  )

# Expose header files at the root of the project (the public header files).
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#-------------------------------------------------------------------------------
# Spire module setup
#-------------------------------------------------------------------------------

# Test to see if we can see our parent spire core target (unlikely, since as
# an external project we will want a degree of separation between the code
# and the parent).
# If we can, it will be much easier to handle dependencies. Otherwise we
# will need to pass in the appropriate core parameters and create a dummy
# core target with which to add extensions.
# This almost suggests that extensions should be handled like subdirectories
# instead of external projects -- then we can share the benefits of a singular
# core.
message("Spire target: ${SPIRE_PARENT_TARGET}")
get_target_property(CORE_INCLUDE_DIRS ${SPIRE_PARENT_TARGET} SPIRE_CORE_INCLUDE_DIRS)
messoge("Core include dirs: ${CORE_INCLUDE_DIRS}")

# Setup SpirePM module currently in use.
message("Spire CMake module path: ${SPIRE_CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SPIRE_CMAKE_MODULE_PATH})
include(SpirePM)

# Ensure we were passed the spire header files. Linkage will be handled for this
# static module automatically higher in the chain.
message("Core directory: ${SPIRE_CORE_DIR}")
include_directories(${SPIRE_CORE_INCLUDE_DIRS})

message("Core include: ${SPIRE_OUTPUT_MODULE_NAME}")

#-------------------------------------------------------------------------------
# Static library setup
#-------------------------------------------------------------------------------

# Possibly use: http://www.cmake.org/Wiki/CMake/Tutorials/Object_Library
# if we have linkage problems. This OBJECT_LIBRARY is new in 2.8.8, but we
# require 2.8.11 anyways. But I don't see a pressing need for this right now
# as we shouldn't run into any linkage problems -- but it's interesting
# none-the-less.
add_library(${SPIRE_OUTPUT_MODULE_NAME} ${Source})

