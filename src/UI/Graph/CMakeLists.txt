add_library(Graph
    graphDisplay.h
    
)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

target_link_libraries(Graph
    PUBLIC
        Qt6::Widgets
        Qt6::Gui
        Qt6::OpenGLWidgets
        Core
        Theme
)

target_include_directories(Graph
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
