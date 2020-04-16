#include "Kernel.hpp"

void TrojanPluginKernelLogger(const std::string &str, Log::Level)
{
    TrojanKernelThread::self->OnKernelLogAvaliable_s(QString::fromStdString(str));
}

TrojanKernel::TrojanKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent), thread(this)
{
    connect(&thread, &TrojanKernelThread::OnKernelCrashed_s, this, &TrojanKernel::OnKernelCrashed);
    connect(&thread, &TrojanKernelThread::OnKernelLogAvaliable_s, this, &TrojanKernel::OnKernelLogAvaliable);
    connect(&thread, &TrojanKernelThread::OnKernelStatsAvailable_s, this, &TrojanKernel::OnKernelStatsAvailable);
}

bool TrojanKernel::StartKernel()
{
    if(hasHttpConfigured)
    {
        httpHelper.httpListen(QHostAddress(httpListenAddress), httpPort, socksPort);
    }
    thread.start();
    return true;
}
bool TrojanKernel::StopKernel()
{
    if(hasHttpConfigured){
        httpHelper.close();
    }
    thread.stop();
    return true;
}
void TrojanKernel::SetConnectionSettings(const QString &listenAddress, const QMap<QString, int> &inbound, const QJsonObject &settings)
{
    hasHttpConfigured = inbound.contains("http");
    hasSocksConfigured = inbound.contains("socks");
    //
    httpPort = inbound["http"];
    socksPort = inbound["socks"];
    httpListenAddress = listenAddress;
    //
    Config config;
    config.populate(QJsonDocument(settings).toJson().toStdString());
    config.local_addr = listenAddress.toStdString();
    config.local_port = 0;
    thread.SetConfig(config);
}

const QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> TrojanKernel::KernelOutboundCapabilities() const
{
    return { { "Trojan", "trojan" } };
}

TrojanKernel::~TrojanKernel()
{
}

// ================================================== Kernel Thread ==================================================

void TrojanKernelThread::stop()
{
    if (isRunning() && service)
    {
        service->stop();
        wait();
    }
}

void TrojanKernelThread::run()
{
    try
    {
        service = std::make_unique<Service>(config);
        service->run();
        Log::logger = TrojanPluginKernelLogger;
    }
    catch (const std::exception &e)
    {
        Log::log_with_date_time(std::string("fatal: ") + e.what(), Log::FATAL);
        emit OnKernelCrashed_s(QString(e.what()));
    }
}
