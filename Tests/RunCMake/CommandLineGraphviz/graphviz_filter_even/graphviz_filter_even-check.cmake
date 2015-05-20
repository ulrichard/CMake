if(NOT EXISTS ${RunCMake_TEST_BINARY_DIR}/graphviz_filter_even.dot)
    set(RunCMake_TEST_FAILED "graphviz file not created!")
else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_LIST_DIR}/expected.dot
      ${RunCMake_TEST_BINARY_DIR}/graphviz_filter_even.dot
      RESULT_VARIABLE result
      OUTPUT_VARIABLE stdout
      ERROR_VARIABLE stderr
    )
    if(result STREQUAL 0)
        # good
    else()
        set(RunCMake_TEST_FAILED "graphviz file not equal to expected!")
    endif()
endif()
