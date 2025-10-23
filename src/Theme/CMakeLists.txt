add_library(Theme
    theme.cpp
    theme.h
)

target_link_libraries(Theme
    PUBLIC
        Qt6::Core
)

target_include_directories(Theme
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
