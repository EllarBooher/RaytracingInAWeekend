# taken from CMacIonize

# Test macro
macro(add_unit_test)
    set(options PARALLEL)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}"
                               "${multiValueArgs}" ${ARGN})
    message(STATUS "generating ${TEST_NAME}")
    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${TEST_SOURCES})
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                          ${PROJECT_BINARY_DIR}/rundir/test)
    target_link_libraries(${TEST_NAME} ${TEST_LIBS})
    if(HAVE_MPI)
      target_link_libraries(${TEST_NAME}
                            ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
    endif(HAVE_MPI)

    if(TEST_PARALLEL AND HAVE_MPI)
      set(TESTCOMMAND ${MPIEXEC})
      set(TESTARGS ${MPIEXEC_NUMPROC_FLAG} 3 ${MPIEXEC_PREFLAGS}
                   "./${TEST_NAME}" ${MPIEXEC_POSTFLAGS})
      set(TESTCOMMAND ${TESTCOMMAND} ${TESTARGS})
    else(TEST_PARALLEL AND HAVE_MPI)
      set(TESTCOMMAND ${TEST_NAME})
    endif(TEST_PARALLEL AND HAVE_MPI)
    add_test(NAME ${TEST_NAME}
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/rundir/test
             COMMAND ${TESTCOMMAND})

    set(TESTNAMES ${TESTNAMES} ${TEST_NAME})

    # create a custom target for this test alone
    add_custom_target(check_${TEST_NAME}
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                              -R ${TEST_NAME}
                      DEPENDS ${TEST_NAME})
endmacro(add_unit_test)

# Add all unit tests here
set(DUMMYTEST "DummyTest.cpp")
add_unit_test(NAME "dummyTest" SOURCES ${DUMMYTEST})

# Finish up and build check command to run all the tests
# add_custom_target(buildTests ALL DEPENDS ${TESTNAMES})
add_custom_target(check ALL COMMAND ${CMAKE_CTEST_COMMAND}
                                  --parallel ${MAX_NUMBER_OF_THREADS}
                                  --output-on-failure
                        DEPENDS ${TESTNAMES})