add_library(Core
    quadtree.h
    quadtree.cpp

    dataset.h
    dataset.cpp

    graph.h
    graph.cpp

    core.cpp
    core.h
)

target_link_libraries(Core
    PUBLIC
        Qt6::Core
)

# Export include path for other targets
target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
