cmake_minimum_required(VERSION 3.16)

project(ElGritoCimarron VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ElGritoCimarron
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/puntuacion.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/puntuacion.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/juego.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/juego.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/personaje.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/personaje.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/nivel.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/nivel.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/enemigo.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/enemigo.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/jugador.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/jugador.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/arma.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/arma.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/objeto.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/objeto.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/mejora.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/mejora.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/rehen.h build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/rehen.cpp
        build/Desktop_Qt_6_11_2_MinGW_64_bit_Debug/recursos.qrc

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ElGritoCimarron APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ElGritoCimarron SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ElGritoCimarron
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ElGritoCimarron PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ElGritoCimarron)
endif()
set_target_properties(ElGritoCimarron PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ElGritoCimarron
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ElGritoCimarron)
endif()
