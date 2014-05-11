# Retrieve all descriptors tests source files

file(GLOB
     CHARTREUSE_TESTS_DESCRIPTORS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_TESTS_DESCRIPTORS_SRC
    ${CHARTREUSE_TESTS_DESCRIPTORS_SRC}
    PARENT_SCOPE
)
