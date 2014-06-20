# Retrieve all interface source files

file(GLOB
     CHARTREUSE_INTERFACE_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_INTERFACE_SRC
    ${CHARTREUSE_INTERFACE_SRC}
    PARENT_SCOPE
)

file(GLOB
     CHARTREUSE_INTERFACE_HDR
     *.h
)

# Expose variables to parent CMake files
set(CHARTREUSE_INTERFACE_HDR
    ${CHARTREUSE_INTERFACE_HDR}
    PARENT_SCOPE
)
