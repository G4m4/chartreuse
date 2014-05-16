# Retrieve all manager tests source files

file(GLOB
     CHARTREUSE_TESTS_MANAGER_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_TESTS_MANAGER_SRC
    ${CHARTREUSE_TESTS_MANAGER_SRC}
    PARENT_SCOPE
)
