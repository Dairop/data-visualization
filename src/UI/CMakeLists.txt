add_library(UI
    UiPages.h

    menuPage.cpp
    menuPage.h

    visualizePage.cpp
    visualizePage.h    

    dataPage.cpp
    dataPage.h

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

add_subdirectory(Graph)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

target_link_libraries(UI
    PUBLIC
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Core
        Theme
        Graph
)

target_include_directories(UI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
