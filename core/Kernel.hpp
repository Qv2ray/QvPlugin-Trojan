#pragma once
#include "QvPluginProcessor.hpp"

class SimpleKernel : public Qv2rayPlugin::QvPluginKernel
{
  public:
    explicit SimpleKernel(QObject *parent = nullptr);
    /// Kernel related operations
    void SetConnectionSettings(const QString &listenAddress, const QMap<QString, int> &inbound, const QJsonObject &settings) override;
    bool StartKernel() override;
    bool StopKernel() override;
    /// Key = DisplayName, Value = protocol.
    const QList<Qv2rayPlugin::QvPluginOutboundProtocolObject> KernelOutboundCapabilities() const override;
};
