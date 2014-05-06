# Retrieve all algorithms tests source files

file(GLOB
     CHARTREUSE_TESTS_ALGORITHMS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_TESTS_ALGORITHMS_SRC
    ${CHARTREUSE_TESTS_ALGORITHMS_SRC}
    PARENT_SCOPE
)
