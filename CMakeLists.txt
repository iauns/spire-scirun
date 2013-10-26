cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Spire_SCIRun)

#-----------------------------------------------------------------------
# CPM configuration
#-----------------------------------------------------------------------
set(CPM_MODULE_NAME spire_scirun)
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
  set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})

  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
  include(CPM)
else()
  set (CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
  find_package(Git)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "CPM requires Git.")
  endif()
  if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
      RESULT_VARIABLE error_code
      OUTPUT_VARIABLE head_sha
      )
    if(error_code)
      message(FATAL_ERROR "CPM failed to get the hash for HEAD")
    endif()
  endif()
  include(${CPM_DIR}/CPM.cmake)
endif()

# Ensure spire module is present. We will use a pre-existing verision if found.
CPM_AddModule("spire"
  GIT_REPOSITORY "https://github.com/SCIInstitute/spire"
  GIT_TAG "origin/master"
  USE_EXISTING_VER TRUE
  )

CPM_InitModule(${CPM_MODULE_NAME})

#-------------------------------------------------------------------------------
# Copy shaders and assets to OUTPUT_SHADER_DIR and OUTPUT_ASSET_DIR.
#-------------------------------------------------------------------------------
function (CopyShaders src_shader_dir dest_shader_dir)
  if (NOT dest_shader_dir STREQUAL "")
    file(COPY ${src_shader_dir}/ DESTINATION ${dest_shader_dir}
      FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
      DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ
                            WORLD_READ WORLD_EXECUTE)
  endif()
endfunction()

function (CopyAssets src_asset_dir dest_asset_dir)
  if (NOT dest_asset_dir STREQUAL "")
    file(COPY ${src_asset_dir}/ DESTINATION ${dest_asset_dir}
      FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
      DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                            GROUP_READ
                            WORLD_READ WORLD_EXECUTE)
  endif()
endfunction()

if (DEFINED OUTPUT_SHADER_DIR)
  CopyShaders(${CMAKE_CURRENT_SOURCE_DIR}/shaders ${OUTPUT_SHADER_DIR})
else()
  message(WARNING "Not copying shader files. OUTPUT_SHADER_DIR variable not found.")
endif()

if (DEFINED OUTPUT_ASSET_DIR)
  CopyAssets(${CMAKE_CURRENT_SOURCE_DIR}/assets ${OUTPUT_ASSET_DIR})
else()
  message(WARNING "Not copying asset files. OUTPUT_ASSET_DIR variable not found.")
endif()

#-------------------------------------------------------------------------------
# Compiler settings.
#-------------------------------------------------------------------------------
if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wall -Wextra -Werror -Wconversion -Wshadow")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  endif ()
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
add_library(${CPM_LIB_TARGET_NAME} ${Source})
target_link_libraries(${CPM_LIB_TARGET_NAME} ${CPM_LIBRARIES})

