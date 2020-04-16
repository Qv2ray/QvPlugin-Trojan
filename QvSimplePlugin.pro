QT += core gui widgets
CONFIG += qt c++14 plugin

TEMPLATE = lib

include(interface/QvPluginInterface.pri)

SOURCES += \
    SimplePlugin.cpp \
    core/Kernel.cpp \
    core/EventHandler.cpp \
    core/Serializer.cpp

HEADERS += \
    SimplePlugin.hpp \
    core/Kernel.hpp \
    core/EventHandler.hpp \
    core/Serializer.hpp

RESOURCES += resx.qrc
