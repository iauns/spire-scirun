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

# Grab the spire core_prefix. We will construct the necessary include paths
# based off of this. Unless we make extensions shared libraries, I see no other
# way to have multiple dependencies modules of a different version co-existing
# at the same time. We may need to head down that path ... but then we would 
# need to link against spire core and then we run into all sorts of badness
# regarding the core context and passing things around... this implies
# that we really SHOULD do some sort of namespace manipulation in order
# to correctly version extensions and keep the code living in the same binary 
# without the need for shared libraries.
#
# But, only modules would need their namespaces mangled in order to contain
# different versions of modules. We can manage the include directories,
# but there would need to be some sort of namespace directive... The
# directive could be unique to each module, so all of the included modules
# are guaranteed to be in a separate namespace than those linked in prior.
#
# We would just keep a "using SPIRE_MODULE_NAMESPACE" in most files, alongside
# a definition for "namespace SPIRE_MODULE_NAMESPACE". This would also allow
# users of modules to rename their specific modules -- but only at the top level
# not inside the modules themselves. We would need an extra namespace for
# something like that, and I'm not sure it is worth it.
#
# We should just require the definition at the beginning of the program
# regardless for future compatibility.
#
# Need a way for packages to indicate whether multiple versions can be used,
# or whether just one can be used. Modules may be coming to C++ in future
# revisions, should we just wait and see how those turn out instead of
message("Spire core prefix: ${MOD_SPIRE_CORE_PREFIX}")
message("Spire src directory: ${MOD_SPIRE_CORE_SRC}")

# Setup SpirePM module currently in use.
message("Spire CMake module path: ${SPIRE_CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SPIRE_CMAKE_MODULE_PATH})
include(SpirePM)

Spire_BuildCoreThirdPartyIncludes(SPIRE_CORE_THIRD_PARTY)

# Ensure we were passed the spire header files. Linkage will be handled for this
# static module automatically higher in the chain.
message("Core directory: ${SPIRE_CORE_DIR}")
include_directories(${MOD_SPIRE_SRC_PREFIX})

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

