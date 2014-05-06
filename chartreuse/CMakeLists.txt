# @brief Chartreuse root cmake file

add_subdirectory(src)

if (${CHARTREUSE_HAS_GTEST} STREQUAL "ON")
  add_subdirectory(tests)
endif (${CHARTREUSE_HAS_GTEST} STREQUAL "ON")
