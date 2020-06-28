add_library(2-3_tree_gui
    main_window.cpp
    two_three_tree_canvas.cpp
    main_window_control_panel.cpp
)
# HACK: Compiler doesn't see generated ui-headers. Fix it
target_include_directories(2-3_tree_gui
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/2-3_tree_gui_autogen/include"
)
target_link_libraries(2-3_tree_gui
    2-3_tree_lib
    Qt5::Widgets
)

