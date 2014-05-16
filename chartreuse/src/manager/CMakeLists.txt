# Retrieve all manager source files

file(GLOB
     CHARTREUSE_MANAGER_SRC
     *.cc
)

# Expose variables to parent CMake files
set(CHARTREUSE_MANAGER_SRC
    ${CHARTREUSE_MANAGER_SRC}
    PARENT_SCOPE
)

file(GLOB
     CHARTREUSE_MANAGER_HDR
     *.h
)

# Expose variables to parent CMake files
set(CHARTREUSE_MANAGER_HDR
    ${CHARTREUSE_MANAGER_HDR}
    PARENT_SCOPE
)
