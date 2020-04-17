#pragma once
#include "QvPluginProcessor.hpp"
#include "core/Common.hpp"

#include <QWidget>

namespace Ui
{
    class TrojanOutboundEditor;
}

class TrojanOutboundEditor : public Qv2rayPlugin::QvPluginEditor
{
    Q_OBJECT

  public:
    explicit TrojanOutboundEditor(QWidget *parent = nullptr);
    ~TrojanOutboundEditor(); //
    void SetHostInfo(const QString &address, int port);
    void SetContent(const QJsonObject &);
    //
    void SwitchOutbound(const QString &){};
    QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> OutboundCapabilities() const
    {
        return { Qv2rayPlugin::QvPluginOutboundProtocolObject{ "Trojan", "trojan" } };
    }
    QPair<QString, int> GetHostInfo() const
    {
        return { trojan.address, trojan.port };
    }
    const QJsonObject GetContent() const
    {
        return trojan.toJson();
    };
    //

  private slots:
    void on_ignoreCertCB_stateChanged(int arg1);

    void on_ignoreHostCB_stateChanged(int arg1);

    void on_sessionReuseCB_stateChanged(int arg1);

    void on_sessionTicketCB_stateChanged(int arg1);

    void on_portReuseCB_stateChanged(int arg1);

    void on_tcpFastOpenCB_stateChanged(int arg1);

    void on_passwordTxt_textEdited(const QString &arg1);

    void on_sniTxt_textEdited(const QString &arg1);

  private:
    TrojanObject trojan;
    Ui::TrojanOutboundEditor *ui;
};
