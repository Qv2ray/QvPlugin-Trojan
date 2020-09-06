#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"

#include <QObject>
#include <QUrl>
#include <QUrlQuery>

using namespace Qv2rayPlugin;

class TrojanOutboundHandler : public Qv2rayPlugin::PluginOutboundHandler
{
  public:
    explicit TrojanOutboundHandler() : PluginOutboundHandler(){};
    const QString SerializeOutbound(const QString &protocol, const QString &name, const QString &group, const QJsonObject &object) const override
    {
        if (protocol == "trojan")
        {
            Q_UNUSED(group)
            TrojanObject o = TrojanObject::fromJson(object);
            QString trojanUri = o.password.toLocal8Bit().toPercentEncoding() +                                      //
                                "@" + o.address +                                                                   //
                                ":" + QString::number(o.port) +                                                     //
                                "?allowInsecure=" + QString::number(int(o.ignoreHostname || o.ignoreCertificate)) + //
                                "&allowInsecureHostname=" + QString::number(int(o.ignoreHostname)) +                //
                                "&allowInsecureCertificate=" + QString::number(int(o.ignoreCertificate)) +          //
                                "&sessionTicket=" + QString::number(int(o.sessionTicket)) +                         //
                                "&tfo=" + QString::number(o.tcpFastOpen) +                                          //
                                "#" + QUrl::toPercentEncoding(name.toUtf8());
            return "trojan://" + trojanUri;
        }
        return "";
    }

    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &link, QString *alias, QString *errorMessage) const override
    {
        const QString prefix = "trojan://";
        if (!link.startsWith(prefix))
        {
            *errorMessage = ("Invalid Trojan URI");
            return {};
        }
        //
        const auto trueList = QStringList{ "true", "1", "yes", "y" };
        const QUrl trojanUrl(link.trimmed());
        const QUrlQuery query(trojanUrl.query());
        *alias = trojanUrl.fragment(QUrl::FullyDecoded);

        auto getQueryValue = [&](const QString &key) {
            return query.queryItemValue(key, QUrl::FullyDecoded);
        };
        //
        TrojanObject result;
        result.address = trojanUrl.host();
        result.password = trojanUrl.userName();
        result.port = trojanUrl.port();
        result.sni = getQueryValue("sni");
        //
        result.tcpFastOpen = trueList.contains(getQueryValue("tfo").toLower());
        result.sessionTicket = trueList.contains(getQueryValue("sessionTicket").toLower());
        //
        bool allowAllInsecure = trueList.contains(getQueryValue("allowInsecure").toLower());
        result.ignoreHostname = allowAllInsecure || trueList.contains(getQueryValue("allowInsecureHostname").toLower());
        result.ignoreCertificate = allowAllInsecure || trueList.contains(getQueryValue("allowInsecureCertificate").toLower());
        //
        return { "trojan", result.toJson() };
    }

    const Qv2rayPlugin::OutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
        if (protocol == "trojan")
        {
            auto o = TrojanObject::fromJson(outbound);
            return { { INFO_SERVER, o.address },
                     { INFO_PROTOCOL, "trojan" },
                     { INFO_PORT, o.port },
                     { INFO_SNI, o.sni.isEmpty() ? o.address : o.sni } };
        }
        return {};
    }

    const QList<QString> SupportedLinkPrefixes() const override
    {
        return { "trojan://" };
    }

    const QList<QString> SupportedProtocols() const override
    {
        return { "trojan" };
    }
};
