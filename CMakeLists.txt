cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  ufm_client
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 9.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

# set(CMAKE_JAVA_INCLUDE_PATH "${CMAKE_PROGRAM_PATH}:${CMAKE_JAVA_INCLUDE_PATH}")
# find_program(OPENAPI_GENERATOR openapi-generator) set(OPENAPI_GENERATOR_CMD ${OPENAPI_GENERATOR}
# generate -i source/ufm_rest_server_api.yaml -g c -o ${CMAKE_CURRENT_BINARY_DIR})
# add_custom_target(ufm_client_generated COMMAND ${CMAKE_COMMAND} -E env "PATH=\"$ENV{PATH}\""
# ${OPENAPI_GENERATOR_CMD} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMENT "Generating client
# for UFM's REST API, with OpenAPI Generator" )

set(UFM_SERVER_REST_API_FILE source/ufm_rest_server_api.yaml)
set(UFM_CLIENT_GENERATED_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/ufm_client_generated")
set(GENERATE_REST_CLIENT
    openapi-generator
    generate
    -i
    ${UFM_SERVER_REST_API_FILE}
    -g
    c
    -o
    ${UFM_CLIENT_GENERATED_FOLDER}
)

add_custom_target(
  ufm_client_generated
  COMMAND ${GENERATE_REST_CLIENT}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${UFM_SERVER_REST_API_FILE}
  COMMENT "Generating client for UFM's REST API, with OpenAPI Generator"
)

if(NOT EXISTS ${UFM_CLIENT_GENERATED_FOLDER})
  message("ufm client doesn't exist, creating it at '${UFM_CLIENT_GENERATED_FOLDER}'")
  execute_process(
    COMMAND ${GENERATE_REST_CLIENT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
  )
endif()

add_subdirectory(${UFM_CLIENT_GENERATED_FOLDER})
# execute_process(COMMAND cmake -E remove_directory ${UFM_CLIENT_GENERATED_FOLDER})

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)

target_include_directories(${PROJECT_NAME} PRIVATE ${UFM_CLIENT_GENERATED_FOLDER})
# add_dependencies(${PROJECT_NAME} ufm_client_generated) # remove or fix to have a build-when-yaml-change (UFM_SERVER_REST_API_FILE)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 9.1.0"
)
