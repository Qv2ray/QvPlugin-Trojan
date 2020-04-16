#pragma once
#include "QvPluginProcessor.hpp"
#include "utils/HttpProxy.hpp"
#include "trojan/src/core/config.h"
#include "trojan/src/core/service.h"

void TrojanPluginKernelLogger(const std::string &, Log::Level);

class TrojanKernelThread : public QThread
{
    Q_OBJECT
  public:
    TrojanKernelThread(QObject *parent = nullptr) : QThread(parent){};
    ~TrojanKernelThread();
    void stop();
    static TrojanKernelThread *self;

  public:
    void SetConfig(const Config &conf)
    {
        config = conf;
    }

  protected:
    void run() override;

  signals:
    void OnKernelCrashed_s(const QString &);
    void OnKernelLogAvaliable_s(const QString &);
    void OnKernelStatsAvailable_s(quint64 upSpeed, quint64 downSpeed);

  private:
    std::unique_ptr<Service> service;
    Config config;
};

class TrojanKernel : public Qv2rayPlugin::QvPluginKernel
{
    Q_OBJECT
  public:
    explicit TrojanKernel(QObject *parent = nullptr);
    ~TrojanKernel();
    void SetConnectionSettings(const QString &listenAddress, const QMap<QString, int> &inbound, const QJsonObject &settings) override;
    bool StartKernel() override;
    bool StopKernel() override;
    const QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> KernelOutboundCapabilities() const override;

  private:
    Qv2rayPlugin::Utils::HttpProxy httpHelper;
    int httpPort;
    int socksPort;
    QString httpListenAddress;
    bool hasSocksConfigured;
    bool hasHttpConfigured;
    TrojanKernelThread thread;
};
