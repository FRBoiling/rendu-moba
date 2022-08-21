#**********************************
#  Created by boil on 2022/8/14.
#**********************************

cmake_minimum_required(VERSION 3.23)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set projectname (must be done AFTER setting configurationtypes)
project(rendu-moba)

# CMake policies (can not be handled elsewhere)
cmake_policy(SET CMP0005 NEW)
cmake_policy(SET CMP0043 NEW) # Cotire isn't used so set to NEW
cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted - prevents intepreting if(SOME_STRING_VARIABLE MATCHES "MSVC") as if(SOME_STRING_VARIABLE MATCHES "1")

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables
endif()

# Set RPATH-handing (CMake parameters)
set(CMAKE_SKIP_BUILD_RPATH 0)
set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
  set(CMAKE_DEFAULT_BUILD_TYPE "RelWithDebInfo" CACHE INTERNAL "")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)
include(ConfigureScripts)
include(ConfigureBaseTargets)
include(CheckPlatform)
include(GroupSources)
include(AutoCollect)

# add dependencies
add_subdirectory(deps)
## add core sources
add_subdirectory(src)