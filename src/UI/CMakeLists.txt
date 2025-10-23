add_library(UI
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(UI
    PUBLIC
        Qt6::Widgets
        Core
        Theme
)

target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
