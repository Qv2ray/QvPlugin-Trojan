#include "TrojanPlugin.hpp"

#include <QDateTime>
#include <QLabel>
#include <QMetaEnum>

std::shared_ptr<QvPluginKernel> TrojanPlugin::GetKernel()
{
    return kernel;
}

bool TrojanPlugin::UpdateSettings(const QJsonObject &conf)
{
    settings = conf;
    return true;
}

bool TrojanPlugin::Initialize(const QString &, const QJsonObject &settings)
{
    emit PluginLog("Initialize plugin.");
    this->settings = settings;
    serializer = std::make_shared<TrojanSerializer>(this);
    eventHandler = std::make_shared<TrojanEventHandler>(this);
    kernel = std::make_shared<TrojanKernel>(this);
    return true;
}

const QJsonObject TrojanPlugin::GetSettngs()
{
    return settings;
}

std::shared_ptr<QvPluginEventHandler> TrojanPlugin::GetEventHandler()
{
    return eventHandler;
}

std::unique_ptr<QWidget> TrojanPlugin::GetSettingsWidget()
{
    return std::make_unique<QLabel>("Text!");
}

std::unique_ptr<QvPluginEditor> TrojanPlugin::GetEditorWidget(UI_TYPE type)
{
    switch (type)
    {
        case UI_TYPE_INBOUND_EDITOR:
        case UI_TYPE_OUTBOUND_EDITOR:
        case UI_TYPE_GROUP_EDITOR: break;
    }
    return nullptr;
}

std::shared_ptr<QvPluginSerializer> TrojanPlugin::GetSerializer()
{
    return serializer;
}
