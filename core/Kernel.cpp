#include "Kernel.hpp"

SimpleKernel::SimpleKernel(QObject *parent) : Qv2rayPlugin::QvPluginKernel(parent)
{
}
bool SimpleKernel::StartKernel()
{
    return true;
}
void SimpleKernel::SetConnectionSettings(const QString &listenAddress, const QMap<QString, int> &inbound, const QJsonObject &settings)
{
    Q_UNUSED(inbound)
    Q_UNUSED(settings)
    Q_UNUSED(listenAddress)
}
bool SimpleKernel::StopKernel()
{
    emit OnKernelCrashed("Yay!");
    return true;
}
const QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> SimpleKernel::KernelOutboundCapabilities() const
{
    return { { "Fake outbound", "pseudo" } };
}
