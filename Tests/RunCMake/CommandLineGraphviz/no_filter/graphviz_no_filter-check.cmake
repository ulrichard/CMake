if(NOT EXISTS ${RunCMake_TEST_BINARY_DIR}/no_filter.dot)
  set(RunCMake_TEST_FAILED "graphviz file not created!")
endif()
