add_library(Graph
    graphDisplay.h
    graphDisplay.cpp
)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(OpenGL REQUIRED)

target_link_libraries(Graph
    PUBLIC
        Qt6::Widgets
        Qt6::Gui
        Qt6::OpenGLWidgets
        Core
        Theme
        OpenGL::GL
)

target_include_directories(Graph
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
