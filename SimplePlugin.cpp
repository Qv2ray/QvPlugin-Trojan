#include "SimplePlugin.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

std::shared_ptr<QvPluginKernel> SimplePlugin::GetKernel()
{
    return kernel;
}

bool SimplePlugin::UpdateSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool SimplePlugin::Initialize(const QString &, const QJsonObject &settings)
{
    emit PluginLog("Initialize plugin.");
    this->settings = settings;
    serializer = std::make_shared<SimpleSerializer>(this);
    eventHandler = std::make_shared<SimpleEventHandler>(this);
    kernel = std::make_shared<SimpleKernel>(this);
    return true;
}

const QJsonObject SimplePlugin::GetSettngs()
{
    return settings;
}

std::shared_ptr<QvPluginEventHandler> SimplePlugin::GetEventHandler()
{
    return eventHandler;
}

std::unique_ptr<QWidget> SimplePlugin::GetSettingsWidget()
{
    return std::make_unique<QLabel>("Text!");
}

std::unique_ptr<QvPluginEditor> SimplePlugin::GetEditorWidget(UI_TYPE type)
{
    switch (type)
    {
        case UI_TYPE_INBOUND_EDITOR:
        case UI_TYPE_OUTBOUND_EDITOR:
        case UI_TYPE_GROUP_EDITOR: break;
    }
    return nullptr;
}

std::shared_ptr<QvPluginSerializer> SimplePlugin::GetSerializer()
{
    return serializer;
}
