#pragma once

#include <QJsonObject>
#include <QString>

struct TrojanObject
{
    quint16 port;
    QString address;
    QString password;
    QString sni;
    bool ignoreCertificate = false;
    bool ignoreHostname = false;
    bool reuseSession = false;
    bool sessionTicket = false;
    bool reusePort = false;
    bool tcpFastOpen = false;

#define _X(name) json[#name] = name
    QJsonObject toJson() const
    {
        QJsonObject json;
        _X(port);
        _X(address);
        _X(password);
        _X(sni);
        _X(ignoreCertificate);
        _X(ignoreHostname);
        _X(reuseSession);
        _X(reusePort);
        _X(sessionTicket);
        _X(tcpFastOpen);
        return json;
    };
#undef _X

#define _X(name, type) name = root[#name].to##type()
    void loadJson(const QJsonObject &root)
    {
        _X(port, Int);
        _X(address, String);
        _X(password, String);
        _X(sni, String);
        _X(ignoreHostname, Bool);
        _X(ignoreCertificate, Bool);
        _X(reuseSession, Bool);
        _X(reusePort, Bool);
        _X(sessionTicket, Bool);
        _X(tcpFastOpen, Bool);
    }
#undef _X

    static TrojanObject fromJson(const QJsonObject &root)
    {
        TrojanObject o;
        o.loadJson(root);
        return o;
    }
};
