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
            QString trojanUri = o.password.toLocal8Bit().toPercentEncoding() +               //
                                "@" + o.address +                                            //
                                ":" + QString::number(o.port) +                              //
                                "?allowinsecure=" + QString::number(int(o.ignoreHostname)) + //
                                "&tfo=" + QString::number(o.tcpFastOpen) +                   //
                                "#" + alias;
            return "trojan://" + trojanUri;
        }
        else
        {
            return "";
        }
    }
    const QPair<QString, QJsonObject> DeserializeOutbound(const QString &link, QString *alias, QString *errorMessage) const override
    {
        auto trojanUri = link.toStdString();
        std::string prefix = "trojan://";

        if (trojanUri.length() < 9)
        {
            *errorMessage = ("Trojan URI is too short");
            return {};
        }

        // prevent line separator casuing wrong password.
        if (!QString::fromStdString(trojanUri).startsWith("trojan://"))
        {
            *errorMessage = ("Invalid Trojan URI");
            return {};
        }

        TrojanObject result;

        // remove the prefix "trojan://" from uri
        std::string uri(trojanUri.data() + 9, trojanUri.length() - 9);
        size_t hashPos = uri.find_last_of('#');
        if (hashPos != std::string::npos)
        {
            // Get the name/remark
            *alias = QUrl::fromPercentEncoding(QString::fromStdString(uri.substr(hashPos + 1)).toLocal8Bit().data());
            uri.erase(hashPos);
        }

        size_t atPos = uri.find_first_of('@');
        if (atPos != std::string::npos)
        {
            result.password = QUrl::fromPercentEncoding(QString::fromStdString(uri.substr(0, atPos)).toLocal8Bit().data());
            uri.erase(0, atPos + 1);
            size_t colonPos = uri.find_last_of(':');
            if (colonPos == std::string::npos)
            {
                *errorMessage = ("Can't find the colon separator between hostname and port");
                return {};
            }
            result.address = QString::fromStdString(uri.substr(0, colonPos));
            result.port = std::stoi(uri.substr(colonPos + 1));
            uri.erase(0, colonPos + 4);
        }
        else
        {
            *errorMessage = ("Can't find the at separator between password and hostname");
            return {};
        }

        size_t ampersandPos = uri.find_first_of('&');
        if (ampersandPos != std::string::npos)
        {
            result.tcpFastOpen = std::stoi(uri.substr(ampersandPos + 5));
            uri.erase(ampersandPos, ampersandPos + 6);
        }

        size_t questionMarkPos = uri.find_last_of('?');
        if (questionMarkPos != std::string::npos)
        {
            result.ignoreHostname = std::stoi(uri.substr(questionMarkPos + 15));
            result.ignoreCertificate = result.ignoreHostname;
        }

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
