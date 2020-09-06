#pragma once

#include "QvPluginProcessor.hpp"
#include "core/Common.hpp"
#include "interface/QvGUIPluginInterface.hpp"

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
    ~TrojanOutboundEditor();
    //
    QPair<QString, int> GetHostAddress() const override
    {
        return { trojan.address, trojan.port };
    }
    void SetHostAddress(const QString &address, int port) override;
    //
    void SetContent(const QJsonObject &) override;
    const QJsonObject GetContent() const override
    {
        return trojan.toJson();
    };

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
