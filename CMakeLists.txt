cmake_minimum_required(VERSION 3.9)

project(WriteTo
  VERSION 0.1.0
  DESCRIPTION "TODO"
  LANGUAGES C CXX
)

include(CMakePrintHelpers)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(cmake/DisableInSourceBuildDir.cmake)

add_subdirectory(src)

option(${PROJECT_NAME}_ENABLE_TESTING "Enable the unit tests build" OFF)
cmake_print_variables(${PROJECT_NAME}_ENABLE_TESTING)

if(${PROJECT_NAME}_ENABLE_TESTING)
  include(cmake/FetchGTest.cmake)
  enable_testing()
  add_subdirectory(tests)
endif()

install(DIRECTORY
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include/>
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PATTERN "*.in" EXCLUDE
  PATTERN "*.hpp"
)
