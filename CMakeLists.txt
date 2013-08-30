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
file (GLOB Source_Root
  "*.cpp"
  "*.h"
  )

#-------------------------------------------------------------------------------
# Spire module setup
#-------------------------------------------------------------------------------
# Ensure we were passed the spire header files. Linkage will be handled for this
# static module automatically higher in the chain.
set(SPIRE_CORE_INCLUDE_DIR "" CACHE INTERNAL FORCE "Path to core spire include directories.")

if(NOT SPIRE_CORE_INCLUDE_DIR/Spire/Interface.h)
  message(SEND_ERROR "Can't find Spire/Interface.h in ${SPIRE_CORE_INCLUDE_DIR}")
endif()

include_directories(${SPIRE_CORE_INCLUDE_DIR})

# Ensure we have been passed an output name for our module.
set(SPIRE_OUTPUT_MODULE_NAME "" CACHE INTERNAL FORCE "Output module name")

if ( SPIRE_OUTPUT_MODULE_NAME STREQUAL "" )
  message(SEND_ERROR "SPIRE_OUTPUT_MODULE_NAME not set. Set to a valid output name.")
endif()

#-------------------------------------------------------------------------------
# Static library setup
#-------------------------------------------------------------------------------

# Possibly use: http://www.cmake.org/Wiki/CMake/Tutorials/Object_Library
# if we have linkage problems. This OBJECT_LIBRARY is new in 2.8.8, but we
# require 2.8.11 anyways. But I don't see a pressing need for this right now
# as we shouldn't run into any linkage problems -- but it's interesting
# none-the-less.
add_library(${SPIRE_OUTPUT_MODULE_NAME}
  )

add_dependencies(spireview Spire)

target_link_libraries(spireview 
  ${SPIRE_LIBRARIES} 
  ${QT_OPENGL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES})

