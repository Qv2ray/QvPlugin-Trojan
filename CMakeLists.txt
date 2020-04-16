project(QvSimplePlugin)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(QVPLUGIN_INTERFACE_INCLUDE_DIR "interface/")
include(interface/QvPluginInterface.cmake)
include_directories(${QVPLUGIN_INTERFACE_INCLUDE_DIR})

add_library(${PROJECT_NAME} MODULE
    SimplePlugin.hpp
    SimplePlugin.cpp
    core/EventHandler.cpp
    core/EventHandler.hpp
    core/Serializer.cpp
    core/Serializer.hpp
    core/Kernel.cpp
    core/Kernel.hpp
    resx.qrc
    ${QVPLUGIN_INTERFACE_HEADERS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets)
