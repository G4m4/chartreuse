# Retrieve all interface tests source files

file(GLOB
     CHARTREUSE_TESTS_INTERFACE_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_TESTS_INTERFACE_SRC
    ${CHARTREUSE_TESTS_INTERFACE_SRC}
    PARENT_SCOPE
)
