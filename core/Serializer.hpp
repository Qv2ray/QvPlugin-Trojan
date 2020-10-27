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
    const QString SerializeOutbound(const QString &protocol, const QString &name, const QString &group, const QJsonObject &object,
                                    const QJsonObject &) const override
    {
        if (protocol == "trojan")
        {
            Q_UNUSED(group)
            const auto o = TrojanObject::fromJson(object);

            QUrlQuery query;
            if (o.ignoreHostname)
                query.addQueryItem("allowInsecureHostname", "1");
            if (o.ignoreCertificate)
                query.addQueryItem("allowInsecureCertificate", "1");
            if (o.sessionTicket)
                query.addQueryItem("sessionTicket", "1");
            if (o.ignoreCertificate || o.ignoreHostname)
                query.addQueryItem("allowInsecure", "1");
            if (o.tcpFastOpen)
                query.addQueryItem("tfo", "1");

            if (!o.sni.isEmpty())
                query.addQueryItem("sni", o.sni);

            QUrl link;
            if (!o.password.isEmpty())
                link.setUserInfo(o.password);
            link.setPort(o.port);
            link.setHost(o.address);
            link.setFragment(name);
            link.setQuery(query);
            link.setScheme("trojan");

            return link.toString(QUrl::FullyEncoded);
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

    const void SetOutboundInfo(const QString &protocol, const OutboundInfoObject &info, QJsonObject &outbound) const override
    {
        if (protocol != "trojan")
            return;
        if (info.contains(INFO_SERVER))
            outbound["address"] = info[INFO_SERVER].toString();
        if (info.contains(INFO_PORT))
            outbound["port"] = info[INFO_PORT].toInt();
        if (info.contains(INFO_SNI))
            outbound["sni"] = info[INFO_SNI].toString();
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
