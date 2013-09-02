cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Spire_SCIRun)

#-------------------------------------------------------------------------------
# Spire module setup
#-------------------------------------------------------------------------------

message("Spire src directory: ${MOD_SPIRE_CORE_SRC}")

# Setup SpirePM module currently in use.
message("Spire CMake module path: ${MOD_SPIRE_CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MOD_SPIRE_CMAKE_MODULE_PATH})
include(SpirePM)

# Build 3rd party include directories (this kind of hack will be replaced
# in any subsquent manager -- it has to deal with externalproject and the
# way it handles parsing its arguments).
Spire_BuildCoreThirdPartyIncludes(SPIRE_CORE_THIRD_PARTY ${MOD_SPIRE_CORE_SRC})

# Ensure we were passed the spire header files. Linkage will be handled for this
# static module automatically higher in the chain.
include_directories(${MOD_SPIRE_CORE_SRC})
include_directories(SYSTEM ${SPIRE_CORE_THIRD_PARTY})

message("Core include: ${SPIRE_OUTPUT_MODULE_NAME}")

#-------------------------------------------------------------------------------
# Compiler settings.
#-------------------------------------------------------------------------------

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wall -Wextra -Werror -Wconversion -Wshadow")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
endif ()

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
# Static library setup
#-------------------------------------------------------------------------------

# Possibly use: http://www.cmake.org/Wiki/CMake/Tutorials/Object_Library
# if we have linkage problems. This OBJECT_LIBRARY is new in 2.8.8, but we
# require 2.8.11 anyways. But I don't see a pressing need for this right now
# as we shouldn't run into any linkage problems -- but it's interesting
# none-the-less.
add_library(${SPIRE_OUTPUT_MODULE_NAME} ${Source})

