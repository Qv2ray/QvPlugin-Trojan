#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"

#include <QObject>
#include <QUrl>

class TrojanSerializer : public Qv2rayPlugin::QvPluginSerializer
{
    Q_OBJECT
  public:
    explicit TrojanSerializer(QObject *parent = nullptr) : QvPluginSerializer(parent){};
    const QString SerializeOutbound(const QString &protocol,  //
                                    const QString &alias,     //
                                    const QString &groupName, //
                                    const QJsonObject &object) const override
    {
        if (protocol == "trojan")
        {
            Q_UNUSED(groupName)
            TrojanObject o = TrojanObject::fromJson(object);
            QString trojanUri = o.password.toLocal8Bit().toPercentEncoding() +                                      //
                                "@" + o.address +                                                                   //
                                ":" + QString::number(o.port) +                                                     //
                                "?allowInsecure=" + QString::number(int(o.ignoreHostname || o.ignoreCertificate)) + //
                                "&allowInsecureHostname=" + QString::number(int(o.ignoreHostname)) +                //
                                "&allowInsecureCertificate=" + QString::number(int(o.ignoreCertificate)) +          //
                                "&sessionTicket=" + QString::number(int(o.sessionTicket)) +                         //
                                "&tfo=" + QString::number(o.tcpFastOpen) +                                          //
                                "#" + QUrl::toPercentEncoding(alias.toUtf8());
            return "trojan://" + trojanUri;
        }
        else
        {
            return "";
        }
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
        const QUrl trojanUrl(link);
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
    const Qv2rayPlugin::QvPluginOutboundInfoObject GetOutboundInfo(const QString &protocol, const QJsonObject &outbound) const override
    {
        if (protocol == "trojan")
        {
            auto o = TrojanObject::fromJson(outbound);
            return Qv2rayPlugin::QvPluginOutboundInfoObject{ o.address, "trojan", o.port };
        }
        else
        {
            return {};
        }
    }
    const QList<QString> ShareLinkPrefixes() const override
    {
        return { "trojan://" };
    }
    const QList<QString> OutboundProtocols() const override
    {
        return { "trojan" };
    }
};
