#include "TrojanPlugin.hpp"

#include "core/Kernel.hpp"
#include "ui/UIInterface.hpp"

#include <QDateTime>
#include <QMetaEnum>

bool TrojanPlugin::InitializePlugin(const QString &, const QJsonObject &settings)
{
    this->settings = settings;
    outboundHandler = std::make_shared<TrojanOutboundHandler>();
    eventHandler = std::make_shared<TrojanEventHandler>();
    kernelInterface = std::make_shared<KernelInterface>();
    guiInterface = new TrojanGUIInterface();
    return true;
}

// std::unique_ptr<QWidget> TrojanPlugin::GetSettingsWidget()
//{
//    auto t = std::make_unique<QTextBrowser>();
//    t->setText("Qv2ray-Trojan plugin was ported from the Trojan-Qt5:"
//               "\r\n"
//               "Credits should also be given to @TheWanderingCoel"
//               "\r\n"
//               "\r\n"
//               "Go and check more about Trojan-Qt5: "
//               "\r\n"
//               "https://github.com/TheWanderingCoel/Trojan-Qt5");
//    return t;
//}
