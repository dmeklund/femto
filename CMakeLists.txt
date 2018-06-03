project(femto LANGUAGES C)

set(
        SOURCE_FILES
        src/basis/piecewise_linear.c
        src/eqn/poisson.c
        src/node/nodeset.c
        include/femto/node/nodeset.h
)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

add_library(femto SHARED ${SOURCE_FILES})
