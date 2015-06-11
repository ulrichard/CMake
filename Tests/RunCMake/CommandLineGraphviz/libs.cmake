
add_library(vendor_lib1 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor_lib1.cpp
)
set_target_properties(vendor_lib1 PROPERTIES LINKER_LANGUAGE CPP)

add_library(vendor_lib2 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor_lib2.cpp
)
set_target_properties(vendor_lib2 PROPERTIES LINKER_LANGUAGE CPP)

add_executable(myapp
    ${CMAKE_CURRENT_LIST_DIR}/myapp.cpp
)
target_link_libraries(myapp vendor_lib1 vendor_lib2)
set_target_properties(myapp PROPERTIES LINKER_LANGUAGE CPP)
