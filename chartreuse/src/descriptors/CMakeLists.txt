# Retrieve all descriptors source files

file(GLOB
     CHARTREUSE_DESCRIPTORS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_DESCRIPTORS_SRC
    ${CHARTREUSE_DESCRIPTORS_SRC}
    PARENT_SCOPE
)

file(GLOB
     CHARTREUSE_DESCRIPTORS_HDR
     *.h
)

# Expose variables to parent CMake files
set(CHARTREUSE_DESCRIPTORS_HDR
    ${CHARTREUSE_DESCRIPTORS_HDR}
    PARENT_SCOPE
)
