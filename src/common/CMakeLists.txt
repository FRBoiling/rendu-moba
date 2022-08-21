#**********************************
#  Created by boil on 2022/8/14.
#**********************************

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE_SOURCES)
#  # Exclude
#        ${CMAKE_CURRENT_SOURCE_DIR}/debugging
#        ${CMAKE_CURRENT_SOURCE_DIR}/platform
#        ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

## Manually set sources for debugging directory as we don't want to include WheatyExceptionReport in common project
## It needs to be included both in authserver and worldserver for the static global variable to be properly initialized
## and to handle crash logs on windows
#list(APPEND PRIVATE_SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/debugging/errors.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/debugging/errors.h)

#if(USE_COREPCH)
#  set(PRIVATE_PCH_HEADER precompiled_headers/common_pch.h)
#endif(USE_COREPCH)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DRENDU_API_EXPORT_COMMON)

add_library(common
  ${PRIVATE_SOURCES}
)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC_INCLUDES
  # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/precompiled_headers)

target_include_directories(common
  PUBLIC
    # Provide the binary dir for all child targets
    ${CMAKE_BINARY_DIR}
    ${PUBLIC_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(common
  PRIVATE
    rendu-core-interface
  PUBLIC
    boost
)

set_target_properties(common
    PROPERTIES
      FOLDER
        "servers")

if(BUILD_SHARED_LIBS)
  if(UNIX)
    install(TARGETS common
      LIBRARY
        DESTINATION lib)
  elseif(WIN32)
    install(TARGETS common
      RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}")
  endif()
endif()

# Generate precompiled header
if(USE_COREPCH)
  add_cxx_pch(common ${PRIVATE_PCH_HEADER})
endif()
