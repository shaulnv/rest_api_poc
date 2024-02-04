cmake_minimum_required(VERSION 3.14...3.22)

project(ufm_client_cli LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME ufm_client SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create cli executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11 OUTPUT_NAME "ufm_client")

target_link_libraries(${PROJECT_NAME} ufm_client::ufm_client cxxopts)
