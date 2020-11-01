#pragma once
#include "Common.hpp"
#include "QvPluginProcessor.hpp"
#include "trojan/src/core/config.h"
#include "trojan/src/core/service.h"
#include "utils/HttpProxy.hpp"

#include <QThread>

void TrojanPluginKernelLogger(const std::string &, Log::Level);
void TrojanPluginAddSentAmout(unsigned long);
void TrojanPluginAddRcvdAmout(unsigned long);

class TrojanKernelThread : public QThread
{
    Q_OBJECT
  public:
    TrojanKernelThread(QObject *parent = nullptr) : QThread(parent)
    {
        Log::logger = TrojanPluginKernelLogger;
        Config::add_recv_len = TrojanPluginAddRcvdAmout;
        Config::add_sent_len = TrojanPluginAddSentAmout;
        self = this;
    }

    ~TrojanKernelThread();
    void stop();
    static TrojanKernelThread *self;

  public:
    void SetConfig(const TrojanObject &conf, const QString &listenIp, int listenPort)
    {
        _config = conf;
        this->listenIp = listenIp;
        this->listenPort = listenPort;
    }

  protected:
    void run() override;

  signals:
    void OnKernelCrashed_s(const QString &);
    void OnKernelLogAvailable_s(const QString &);
    void OnKernelStatsAvailable_s(quint64 upSpeed, quint64 downSpeed);

  private:
    std::unique_ptr<Service> service;
    TrojanObject _config;
    QString listenIp;
    int listenPort;
};

class TrojanKernel : public Qv2rayPlugin::PluginKernel
{
    Q_OBJECT
  public:
    explicit TrojanKernel();
    ~TrojanKernel();
    void SetConnectionSettings(const QMap<Qv2rayPlugin::KernelOptionFlags, QVariant> &options, const QJsonObject &settings) override;
    bool StartKernel() override;
    bool StopKernel() override;
    QString GetKernelName() const override
    {
        return "Trojan";
    }

  protected:
    void timerEvent(QTimerEvent *event) override;

  private:
    QJsonObject trojanConfig;
    unsigned long lastSent;
    unsigned long lastRcvd;
    int statsTimerId = -1;
    Qv2rayPlugin::Utils::HttpProxy httpHelper;
    int httpPort;
    int socksPort;
    QString listenAddress;
    TrojanKernelThread thread;
};

class KernelInterface : public Qv2rayPlugin::PluginKernelInterface
{
  public:
    virtual std::unique_ptr<Qv2rayPlugin::PluginKernel> CreateKernel() const override
    {
        return std::make_unique<TrojanKernel>();
    }
    virtual QList<QString> GetKernelProtocols() const override
    {
        return { "trojan" };
    }
};
