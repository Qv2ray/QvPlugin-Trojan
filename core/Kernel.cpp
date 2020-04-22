#include "Kernel.hpp"

#include "Common.hpp"

#include <atomic>

std::atomic_ulong sentSize{};
std::atomic_ulong rcvdSize{};

TrojanKernelThread *TrojanKernelThread::self = nullptr;

void TrojanPluginKernelLogger(const std::string &str, Log::Level)
{
    TrojanKernelThread::self->OnKernelLogAvaliable_s(QString::fromStdString(str));
}

void TrojanPluginAddSentAmout(unsigned long s)
{
    sentSize += s;
}
void TrojanPluginAddRcvdAmout(unsigned long v)
{
    rcvdSize += v;
}

TrojanKernel::TrojanKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent), thread(this)
{
    connect(&thread, &TrojanKernelThread::OnKernelCrashed_s, this, &TrojanKernel::OnKernelCrashed);
    connect(&thread, &TrojanKernelThread::OnKernelLogAvaliable_s, this, &TrojanKernel::OnKernelLogAvaliable);
    connect(&thread, &TrojanKernelThread::OnKernelStatsAvailable_s, this, &TrojanKernel::OnKernelStatsAvailable);
}

bool TrojanKernel::StartKernel()
{
    sentSize.store(0);
    rcvdSize.store(0);
    statsTimerId = startTimer(1000);
    if (hasHttpConfigured)
    {
        httpHelper.httpListen(QHostAddress(httpListenAddress), httpPort, socksPort);
    }
    thread.start();
    return true;
}

void TrojanKernel::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == statsTimerId)
    {
        emit OnKernelStatsAvailable(rcvdSize.exchange(0), sentSize.exchange(0));
    }
    QObject::timerEvent(event);
}

bool TrojanKernel::StopKernel()
{
    killTimer(statsTimerId);
    if (hasHttpConfigured)
    {
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
    const auto o = TrojanObject::fromJson(settings);
    //
    Config config;
    config.run_type = Config::CLIENT;
    //
    config.password[Config::SHA224(o.password.toStdString())] = o.password.toStdString();
    config.remote_addr = o.address.toStdString();
    config.remote_port = o.port;
    config.ssl.sni = o.sni.toStdString();
    config.ssl.verify = !o.ignoreCertificate;
    config.ssl.verify_hostname = !o.ignoreHostname;
    config.ssl.reuse_session = o.reuseSession;
    config.ssl.session_ticket = o.sessionTicket;
    config.tcp.reuse_port = o.reusePort;
    config.tcp.fast_open = o.tcpFastOpen;
    //
    config.local_addr = listenAddress.toStdString();
    config.local_port = socksPort;
    thread.SetConfig(config);
}

const QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> TrojanKernel::KernelOutboundCapabilities() const
{
    return { { "Trojan", "trojan" } };
}

TrojanKernel::~TrojanKernel()
{
    thread.stop();
}

// ================================================== Kernel Thread ==================================================

void TrojanKernelThread::stop()
{
    if (isRunning() && service)
    {
        service->stop();
        wait();
        service.reset();
    }
}

TrojanKernelThread::~TrojanKernelThread()
{
}

void TrojanKernelThread::run()
{
    try
    {
        Log::level = Log::Level::INFO;
        Log::logger = TrojanPluginKernelLogger;
        Config::add_recv_len = TrojanPluginAddRcvdAmout;
        Config::add_sent_len = TrojanPluginAddSentAmout;
        service = std::make_unique<Service>(config);
        service->run();
    }
    catch (const std::exception &e)
    {
        // Log::log_with_date_time(std::string("fatal: ") + e.what(), Log::FATAL);
        emit OnKernelCrashed_s(QString(e.what()));
        stop();
    }
}
