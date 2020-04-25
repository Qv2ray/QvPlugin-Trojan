#include "TrojanPlugin.hpp"

#include "ui/TrojanOutboundEditor.hpp"

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
    auto t = std::make_unique<QTextBrowser>();
    t->setText("Qv2ray-Trojan plugin was ported from the Trojan-Qt5:"
               "\r\n"
               "Credits should also be given to @TheWanderingCoel"
               "\r\n"
               "\r\n"
               "Go and check more about Trojan-Qt5: "
               "\r\n"
               "https://github.com/TheWanderingCoel/Trojan-Qt5");
    return t;
}

std::unique_ptr<QvPluginEditor> TrojanPlugin::GetEditorWidget(UI_TYPE type)
{
    switch (type)
    {
        case UI_TYPE_INBOUND_EDITOR:
        case UI_TYPE_GROUP_EDITOR: break;
        case UI_TYPE_OUTBOUND_EDITOR: return std::make_unique<TrojanOutboundEditor>();
    }
    return nullptr;
}

std::shared_ptr<QvPluginSerializer> TrojanPlugin::GetSerializer()
{
    return serializer;
}
