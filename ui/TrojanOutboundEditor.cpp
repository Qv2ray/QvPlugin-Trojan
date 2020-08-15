#include "TrojanOutboundEditor.hpp"

#include "ui_TrojanOutboundEditor.h"

TrojanOutboundEditor::TrojanOutboundEditor(QWidget *parent) : Qv2rayPlugin::QvPluginEditor(parent), ui(new Ui::TrojanOutboundEditor)
{
    ui->setupUi(this);
}

TrojanOutboundEditor::~TrojanOutboundEditor()
{
    delete ui;
}

void TrojanOutboundEditor::SetHostAddress(const QString &address, int port)
{
    trojan.address = address;
    trojan.port = port;
}
void TrojanOutboundEditor::SetContent(const QJsonObject &o)
{
    trojan.loadJson(o);
    ui->sniTxt->setText(trojan.sni);
    ui->passwordTxt->setText(trojan.password);
    ui->ignoreCertCB->setChecked(trojan.ignoreCertificate);
    ui->ignoreHostCB->setChecked(trojan.ignoreHostname);
    ui->sessionReuseCB->setChecked(trojan.reuseSession);
    ui->sessionTicketCB->setChecked(trojan.sessionTicket);
    ui->portReuseCB->setChecked(trojan.reusePort);
    ui->tcpFastOpenCB->setChecked(trojan.tcpFastOpen);
}

void TrojanOutboundEditor::on_ignoreCertCB_stateChanged(int arg1)
{
    trojan.ignoreCertificate = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_ignoreHostCB_stateChanged(int arg1)
{
    trojan.ignoreHostname = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_sessionReuseCB_stateChanged(int arg1)
{
    trojan.reuseSession = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_sessionTicketCB_stateChanged(int arg1)
{
    trojan.sessionTicket = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_portReuseCB_stateChanged(int arg1)
{
    trojan.reusePort = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_tcpFastOpenCB_stateChanged(int arg1)
{
    trojan.tcpFastOpen = arg1 == Qt::Checked;
}

void TrojanOutboundEditor::on_passwordTxt_textEdited(const QString &arg1)
{
    trojan.password = arg1;
}

void TrojanOutboundEditor::on_sniTxt_textEdited(const QString &arg1)
{
    trojan.sni = arg1;
}
