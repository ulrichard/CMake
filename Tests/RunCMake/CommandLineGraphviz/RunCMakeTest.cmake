include(RunCMake)

run_cmake_command(filter_even   ${CMAKE_COMMAND} --graphviz=filter_even.dot)
run_cmake_command(filter_uneven ${CMAKE_COMMAND} --graphviz=filter_uneven.dot -D GRAPHVIZ_NODE_FILTER=".*\\.(lib|so|dll|a)")
