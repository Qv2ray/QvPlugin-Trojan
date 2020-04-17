project(QvTrojanPlugin)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Boost_USE_STATIC_LIBS ON)
find_package(Threads REQUIRED)
find_package(Boost 1.66.0 REQUIRED COMPONENTS system program_options)

if(WIN32)
    find_library(OpenSSL REQUIRED)
else()
    find_package(OpenSSL REQUIRED)
endif()

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})

set(QVPLUGIN_INTERFACE_INCLUDE_DIR "interface/")
set(QVPLUGIN_HAS_HTTP_TO_SOCKS_HELPER ON)
include(interface/QvPluginInterface.cmake)
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
    core/Common.hpp
    core/Kernel.cpp
    core/Kernel.hpp
    resx.qrc
    ${QVPLUGIN_INTERFACE_HEADERS}
    ${TROJAN_SOURCES}
    ${TROJAN_HEADERS})

if(APPLE)
    find_library(CoreFoundation CoreFoundation)
    find_library(Security Security)
    target_link_libraries(${PROJECT_NAME} ${CoreFoundation} ${Security})
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32 crypt32)
endif()

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES})
