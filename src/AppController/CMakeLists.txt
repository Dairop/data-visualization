add_library(AppController
    appController.cpp
    appController.h
)

target_link_libraries(AppController
    PUBLIC
        Core
        Theme
        UI
        Qt6::Widgets
)

target_include_directories(AppController
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
