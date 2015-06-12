include(${CMAKE_CURRENT_LIST_DIR}/../../RunCMake.cmake)

set(RunCMake_TEST_OPTIONS --graphviz=graphviz_no_filter.dot)
run_cmake(graphviz_no_filter)
