include(${CMAKE_CURRENT_LIST_DIR}/../../RunCMake.cmake)

set(RunCMake_TEST_OPTIONS --graphviz=graphviz_filter_even.dot)
run_cmake(graphviz_filter_even)
