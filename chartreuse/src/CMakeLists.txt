# @brief Chartreuse main source folder cmake file

include_directories(
  ${CHARTREUSE_INCLUDE_DIR}
)
include_directories(
  SYSTEM
  ${KISSFFT_INCLUDE_DIRS}
)

# Retrieve externals source files
set(CHARTREUSE_EXTERNALS_KISSFFT_SRC
  ${KISSFFT_INCLUDE_DIRS}/tools/kiss_fftr.c
)
set(CHARTREUSE_EXTERNALS_KISSFFT_HDR
  ${KISSFFT_INCLUDE_DIRS}/tools/kiss_fftr.h
)
# Ignoring warnings for external source files
foreach(src_file ${CHARTREUSE_EXTERNALS_KISSFFT_SRC})
  disable_warnings_list(${src_file})
endforeach()

# Retrieve source files from subdirectories
add_subdirectory(algorithms)

# Group sources
source_group("algorithms"
  FILES
  ${CHARTREUSE_ALGORITHMS_SRC}
  ${CHARTREUSE_ALGORITHMS_HDR}
)
source_group("kissfft"
  FILES
  ${CHARTREUSE_EXTERNALS_KISSFFT_SRC}
  ${CHARTREUSE_EXTERNALS_KISSFFT_HDR}
)

# Sources
set(CHARTREUSE_SRC
  ${CHARTREUSE_ALGORITHMS_SRC}
  ${CHARTREUSE_EXTERNALS_KISSFFT_SRC}
)
set(CHARTREUSE_HDR
  configuration.h
  common.h
  ${CHARTREUSE_ALGORITHMS_HDR}
  ${CHARTREUSE_EXTERNALS_KISSFFT_HDR}
)

# Target
add_library(chartreuse_lib
  ${CHARTREUSE_SRC}
  ${CHARTREUSE_HDR}
)

set_target_mt(chartreuse_lib)
