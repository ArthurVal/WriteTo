add_executable(${PROJECT_NAME}
  test_Toto.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE WriteTo::WriteTo
  PRIVATE GTest::gtest_main
  PRIVATE GTest::gmock_main
)

target_compile_options(${PROJECT_NAME}
  PRIVATE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic
)

target_compile_features(${PROJECT_NAME}
  PRIVATE cxx_std_20
)

if(${CMAKE_VERSION} VERSION_LESS "3.10.0")
  gtest_add_tests(TARGET ${PROJECT_NAME})
else()
  gtest_discover_tests(${PROJECT_NAME})
endif()
