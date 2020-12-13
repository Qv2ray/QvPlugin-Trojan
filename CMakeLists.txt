project(QvPlugin-Trojan)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

option(QVPLUGIN_USE_QT6 "Use Qt6")
if(QVPLUGIN_USE_QT6)
    set(QV_QT_LIBNAME Qt6)
else()
    set(QV_QT_LIBNAME Qt5)
endif()
find_package(${QV_QT_LIBNAME} COMPONENTS Core Widgets Gui Network REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(-DTROJAN_USE_EXTERNAL_LOGGER)

set(QVPLUGIN_INTERFACE_INCLUDE_DIR "interface/")
set(QVPLUGIN_HAS_HTTP_TO_SOCKS_HELPER ON)
include(interface/QvPluginInterface.cmake)
include(interface/QvGUIPluginInterface.cmake)
include_directories(${QVPLUGIN_INTERFACE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/trojan/src)

set(TROJAN_SOURCES
    trojan/src/core/authenticator.cpp
    trojan/src/core/config.cpp
    trojan/src/core/log.cpp
    trojan/src/core/service.cpp
    trojan/src/core/version.cpp
    trojan/src/proto/socks5address.cpp
    trojan/src/proto/trojanrequest.cpp
    trojan/src/proto/udppacket.cpp
    trojan/src/session/clientsession.cpp
    trojan/src/session/forwardsession.cpp
    trojan/src/session/natsession.cpp
    trojan/src/session/serversession.cpp
    trojan/src/session/session.cpp
    trojan/src/session/udpforwardsession.cpp
    trojan/src/ssl/ssldefaults.cpp
    trojan/src/ssl/sslsession.cpp
    )

set(TROJAN_HEADERS
    trojan/src/core/authenticator.h
    trojan/src/core/config.h
    trojan/src/core/log.h
    trojan/src/core/service.h
    trojan/src/core/version.h
    trojan/src/proto/socks5address.h
    trojan/src/proto/trojanrequest.h
    trojan/src/proto/udppacket.h
    trojan/src/session/clientsession.h
    trojan/src/session/forwardsession.h
    trojan/src/session/natsession.h
    trojan/src/session/serversession.h
    trojan/src/session/session.h
    trojan/src/session/udpforwardsession.h
    trojan/src/ssl/ssldefaults.h
    trojan/src/ssl/sslsession.h)

add_library(${PROJECT_NAME} MODULE
    TrojanPlugin.hpp
    TrojanPlugin.cpp
    core/EventHandler.cpp
    core/EventHandler.hpp
    core/Serializer.cpp
    core/Serializer.hpp
    ui/TrojanOutboundEditor.cpp
    ui/TrojanOutboundEditor.hpp
    ui/TrojanOutboundEditor.ui
    ui/UIInterface.hpp
    core/Common.hpp
    core/Kernel.cpp
    core/Kernel.hpp
    resx.qrc
    ${QVPLUGIN_INTERFACE_HEADERS}
    ${QVGUIPLUGIN_INTERFACE_HEADERS}
    ${TROJAN_SOURCES}
    ${TROJAN_HEADERS})

include(cmake/trojan.cmake)

if(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/" $<TARGET_FILE:${PROJECT_NAME}>)
endif()

target_link_libraries(${PROJECT_NAME}
    ${QV_QT_LIBNAME}::Core
    ${QV_QT_LIBNAME}::Gui
    ${QV_QT_LIBNAME}::Widgets
    ${QV_QT_LIBNAME}::Network
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION share/qv2ray/plugins)
