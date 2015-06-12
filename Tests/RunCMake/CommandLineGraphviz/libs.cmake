
add_library(vendor_lib1 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor_lib1.cpp
)

add_library(vendor_lib2 STATIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor_lib2.cpp
)

add_executable(myappInt
    ${CMAKE_CURRENT_LIST_DIR}/myapp.cpp
)
target_link_libraries(myappInt
    vendor_lib1
    vendor_lib2
)

add_executable(myappExt
    ${CMAKE_CURRENT_LIST_DIR}/myapp.cpp
)
target_link_libraries(myappExt
    ext_libA
    ext_libB
)
