cmake_minimum_required(VERSION 3.16)
project(data-visualization LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/6.10.0/msvc2022_64" CACHE PATH "Qt6 installation path")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(src)
