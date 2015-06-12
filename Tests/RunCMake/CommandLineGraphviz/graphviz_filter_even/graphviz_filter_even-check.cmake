if(NOT EXISTS ${RunCMake_TEST_BINARY_DIR}/filter_even.dot)
  set(RunCMake_TEST_FAILED "graphviz file not created!")
endif()
