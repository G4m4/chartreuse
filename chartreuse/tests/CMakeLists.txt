# @brief Build Chartreuse tests executable

# preventing warnings from external source files
include_directories(
  SYSTEM
  ${GTEST_INCLUDE_DIRS}
)

include_directories(
  ${CHARTREUSE_INCLUDE_DIR}
)

# Include all subdirectories tests source files
add_subdirectory(algorithms)

# Group sources
source_group("algorithms"
  FILES
  ${CHARTREUSE_TESTS_ALGORITHMS_SRC}
)

# Source files
set(CHARTREUSE_TESTS_SRC
    main.cc
    ${CHARTREUSE_TESTS_ALGORITHMS_SRC}
)

# Target
add_executable(chartreuse_tests
  ${CHARTREUSE_TESTS_SRC}
)

set_target_mt(chartreuse_tests)

target_link_libraries(chartreuse_tests
  chartreuse_lib
  gtest_main
)
