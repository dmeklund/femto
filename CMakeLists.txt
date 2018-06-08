cmake_minimum_required(VERSION 3.10)
project(femto LANGUAGES C)

set(
        SOURCE_FILES
        src/basis/piecewise_linear.c
        src/eqn/poisson.c
        src/node/nodeset.c
        src/linalg/matrix.c
        src/util.c
        include/femto/node/nodeset.h
        include/femto/linalg/matrix.h
        include/femto/util.h
)
set(
        LIBRARIES
        gc
)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

add_library(femto SHARED ${SOURCE_FILES})
target_link_libraries(femto ${LIBRARIES})

