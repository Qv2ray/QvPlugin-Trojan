#include "Kernel.hpp"

#include <atomic>

std::atomic_ulong sentSize{};
std::atomic_ulong rcvdSize{};

TrojanKernelThread *TrojanKernelThread::self = nullptr;

void TrojanPluginKernelLogger(const std::string &str, Log::Level)
{
    TrojanKernelThread::self->OnKernelLogAvailable_s(QString::fromStdString(str));
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
    connect(&thread, &TrojanKernelThread::OnKernelLogAvailable_s, this, &TrojanKernel::OnKernelLogAvailable);
    connect(&thread, &TrojanKernelThread::OnKernelStatsAvailable_s, this, &TrojanKernel::OnKernelStatsAvailable);
}

void TrojanKernel::SetConnectionSettings(const QMap<KernelSetting, QVariant> &options, const QJsonObject &settings)
{
    httpPort = options[KERNEL_HTTP_ENABLED].toBool() ? options[KERNEL_HTTP_PORT].toInt() : 0;
    socksPort = options[KERNEL_SOCKS_ENABLED].toBool() ? options[KERNEL_SOCKS_PORT].toInt() : 0;
    listenAddress = options[KERNEL_LISTEN_ADDRESS].toString();
    trojanConfig = settings;
}

bool TrojanKernel::StartKernel()
{
    if (socksPort == 0 && httpPort == 0)
    {
        emit OnKernelCrashed("Both HTTP and SOCKS inbounds are disabled.");
        return false;
    }
    if (socksPort == 0)
    {
        socksPort = httpPort + 100;
    }
    sentSize.store(0);
    rcvdSize.store(0);
    thread.SetConfig(TrojanObject::fromJson(trojanConfig), listenAddress, socksPort);
    statsTimerId = startTimer(1000);
    if (httpPort != 0)
    {
        httpHelper.httpListen(QHostAddress(listenAddress), httpPort, socksPort);
    }
    thread.start();
    return true;
}

bool TrojanKernel::StopKernel()
{
    killTimer(statsTimerId);
    if (httpPort != 0)
    {
        httpHelper.close();
    }
    thread.stop();
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
    Config configX;
    configX.run_type = Config::CLIENT;
    configX.log_level = Log::INFO;
    configX.password[Config::SHA224(_config.password.toStdString())] = _config.password.toStdString();
    configX.remote_addr = _config.address.toStdString();
    configX.remote_port = _config.port;
    configX.ssl.sni = _config.sni.toStdString();
    configX.ssl.verify = !_config.ignoreCertificate;
    configX.ssl.verify_hostname = !_config.ignoreHostname;
    configX.ssl.reuse_session = _config.reuseSession;
    configX.ssl.session_ticket = _config.sessionTicket;
    configX.tcp.reuse_port = _config.reusePort;
    configX.tcp.fast_open = _config.tcpFastOpen;
    //
    configX.local_addr = listenIp.toStdString();
    configX.local_port = listenPort;

    try
    {
        service = std::make_unique<Service>(configX);
        service->run();
    }
    catch (const std::exception &e)
    {
        // Log::log_with_date_time(std::string("fatal: ") + e.what(), Log::FATAL);
        emit OnKernelCrashed_s(QString(e.what()));
        stop();
    }
}
