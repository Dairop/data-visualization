# Ajoute les sous-dossiers
add_subdirectory(AppController)
add_subdirectory(Core)
add_subdirectory(Theme)
add_subdirectory(UI)


# Crée l'exécutable
qt_add_executable(data-visualization
    main.cpp
)

find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

# Lie les bibliothèques internes et Qt
target_link_libraries(data-visualization PRIVATE
    AppController
    Core
    Theme
    UI
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# Configuration pour Windows
set_target_properties(data-visualization PROPERTIES
    WIN32_EXECUTABLE TRUE
)
