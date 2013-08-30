cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Spire_SCIRun)

#-------------------------------------------------------------------------------
# Spire Module setup
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



