cmake_minimum_required(VERSION 3.16)

project(GritoCimarron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(GritoCimarron
    main.cpp
    arma.h arma.cpp
    enemigo.h enemigo.cpp
    juego.h juego.cpp
    jugador.h jugador.cpp
    mejora.h mejora.cpp
    nivel.h nivel.cpp
    objeto.h objeto.cpp
    personaje.h personaje.cpp
    puntuacion.h puntuacion.cpp
    rehen.h rehen.cpp
)

target_link_libraries(GritoCimarron PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS GritoCimarron
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)