include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(2-3_tree_impl/)
add_subdirectory(gui/)

add_executable(2-3_tree
    main.cpp
)
target_link_libraries(2-3_tree
    2-3_tree_lib
    2-3_tree_gui
)

