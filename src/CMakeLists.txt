add_subdirectory(WriteTo)

add_executable(${PROJECT_NAME}-fun
  some_fun.cpp
)

target_link_libraries(${PROJECT_NAME}-fun
  PRIVATE
  WriteTo::WriteTo
)

target_compile_options(${PROJECT_NAME}-fun
  PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wdouble-promotion
  -Wundef
  -Wconversion
  # -Werror
  )

target_compile_features(${PROJECT_NAME}-fun
  PRIVATE cxx_std_20
)
