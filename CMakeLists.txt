cmake_minimum_required(VERSION 3.5)

project(MapGenerator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.


find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    configure.h
    gridscene.h
    worldfilegenerator.h
    worldfilegenerator.cpp
    gridscene.cpp
    mainwindow.ui
    simulationlauncher.ui
    simulationlauncher.h
    simulationlauncher.cpp
    uavitem.ui
    uavitem.h
    uavitem.cpp)

qt5_add_resources(SOURCES resource.qrc)


add_executable(MapGenerator ${SOURCES})

target_link_libraries(MapGenerator PRIVATE Qt5::Widgets)
