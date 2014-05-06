# Retrieve all algorithms source files

file(GLOB
     CHARTREUSE_ALGORITHMS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_ALGORITHMS_SRC
    ${CHARTREUSE_ALGORITHMS_SRC}
    PARENT_SCOPE
)

file(GLOB
     CHARTREUSE_ALGORITHMS_HDR
     *.h
)

# Expose variables to parent CMake files
set(CHARTREUSE_ALGORITHMS_HDR
    ${CHARTREUSE_ALGORITHMS_HDR}
    PARENT_SCOPE
)
