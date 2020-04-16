#pragma once
#include "QvPluginProcessor.hpp"

class TrojanEventHandler : public Qv2rayPlugin::QvPluginEventHandler
{
  public:
    TrojanEventHandler(QObject *parent = nullptr);
};
