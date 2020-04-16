#pragma once

#include "QvPluginInterface.hpp"
#include "core/EventHandler.hpp"
#include "core/Kernel.hpp"
#include "core/Serializer.hpp"

#include <QObject>
#include <QtPlugin>

class QLabel;
using namespace Qv2rayPlugin;

class TrojanPlugin
    : public QObject
    , Qv2rayInterface
{
    Q_INTERFACES(Qv2rayPlugin::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
  public:
    //
    // Basic metainfo of this plugin
    const QvPluginMetadata GetMetadata() const override
    {
        return QvPluginMetadata{
            "Trojan Plugin",                      //
            "Qv2ray Workgroup",                   //
            "qvtrojan_plugin",                    //
            "Connect to Trojan server in Qv2ray", //
            QIcon(":/assets/logo.png"),           //
            {},                                   //
            { SPECIAL_TYPE_KERNEL,                //
              SPECIAL_TYPE_SERIALIZOR }           //
        };
    }
    //
    std::shared_ptr<QvPluginKernel> GetKernel() override;
    std::shared_ptr<QvPluginSerializer> GetSerializer() override;
    std::shared_ptr<QvPluginEventHandler> GetEventHandler() override;
    std::unique_ptr<QvPluginEditor> GetEditorWidget(UI_TYPE) override;
    std::unique_ptr<QWidget> GetSettingsWidget() override;
    //
    bool UpdateSettings(const QJsonObject &) override;
    bool Initialize(const QString &, const QJsonObject &) override;
    const QJsonObject GetSettngs() override;
    //
  signals:
    void PluginLog(const QString &) const override;
    void PluginErrorMessageBox(const QString &) const override;

  private:
    QJsonObject settings;
    std::shared_ptr<TrojanSerializer> serializer;
    std::shared_ptr<TrojanEventHandler> eventHandler;
    std::shared_ptr<TrojanKernel> kernel;
};
