cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${LLVM_INCLUDE_DIRS})

add_executable(
  ptrVision_tests
  test_pointer_arithmetic.cpp
  test_pointer_dereference.cpp
  test_references.cpp
  test_memory_management.cpp
  test_bitwise_operations.cpp
  test_goto_statements.cpp
  test_array_operations.cpp
  test_complex_scenarios.cpp
  test_error_handling.cpp
)

target_link_libraries(
  ptrVision_tests
  GTest::gtest_main
  ptrVision
)

include(GoogleTest)
gtest_discover_tests(ptrVision_tests)
