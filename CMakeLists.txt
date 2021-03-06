cmake_minimum_required(VERSION 3.10)
project(femto LANGUAGES C)
enable_testing()

if (${CMAKE_C_COMPILER_ID} MATCHES GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -O0 -Wall")
elseif (${CMAKE_C_COMPILER_ID} MATCHES Intel)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wremarks -O0 -g")
endif()

message(${CMAKE_C_COMPILER_ID})

find_package(Doxygen)
doxygen_add_docs(doxygen include)

set(
        SOURCE_FILES
        src/basis/piecewise_poly.c
        src/basis/set.c
        src/calc/diff.c
        src/eqn/poisson.c
        src/fem/solver.c
        src/fem/stiffness.c
        src/geom/point.c
        src/geom/shapes.c
        src/linalg/matrix.c
        src/mesh/mesh.c
        src/node/nodeset.c
        src/poly/piecewise.c
        src/poly/poly.c
        src/quad/gauss.c
        src/function.c
        src/util.c
        src/util/array.c
        include/femto/basis/piecewise_poly.h
        include/femto/basis/set.h
        include/femto/calc/diff.h
        include/femto/fem/solver.h
        include/femto/fem/stiffness.h
        include/femto/geom/point.h
        include/femto/geom/shapes.h
        include/femto/linalg/matrix.h
        include/femto/mesh/mesh.h
        include/femto/node/nodeset.h
        include/femto/poly/piecewise.h
        include/femto/poly/poly.h
        include/femto/quad/gauss.h
        include/femto/function.h
        include/femto/util.h
        include/femto/util/array.h
)

set(
        LIBRARIES
        gc
        cmocka
        m
)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

add_library(femto SHARED ${SOURCE_FILES})
target_link_libraries(femto ${LIBRARIES})

set(
        TEST_FILES
        test/all_tests.h
        test/run_tests.c
        test/fto_test.c
        test/fto_test.h
        test/basis/test_piecewise_poly.c
        test/basis/test_piecewise_poly.h
        test/basis/test_set.c
        test/basis/test_set.h
        test/calc/test_diff.c
        test/calc/test_diff.h
        test/fem/test_solver.c
        test/fem/test_stiffness.c
        test/fem/test_stiffness.h
        test/poly/test_poly.c
        test/poly/test_poly.h
        test/quad/test_gauss.c
        test/quad/test_gauss.h
)

add_executable(fto_run_tests ${TEST_FILES})
target_link_libraries(fto_run_tests ${LIBRARIES} femto)
target_include_directories(fto_run_tests PRIVATE test)

add_test(TestSuite fto_run_tests)