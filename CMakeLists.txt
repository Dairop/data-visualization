cmake_minimum_required(VERSION 3.16)
project(data-visualization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt path
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(data-visualization
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(data-visualization PRIVATE Qt6::Widgets)

set_target_properties(data-visualization PROPERTIES
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS data-visualization
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin
)
