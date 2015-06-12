include(${CMAKE_CURRENT_LIST_DIR}/../../RunCMake.cmake)

set(RunCMake_TEST_OPTIONS --graphviz=filter_uneven.dot)
run_cmake(graphviz_filter_uneven)
