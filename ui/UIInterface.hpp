#pragma once
#include "interface/QvGUIPluginInterface.hpp"
#include "ui/TrojanOutboundEditor.hpp"

using namespace Qv2rayPlugin;

class TrojanGUIInterface : public PluginGUIInterface
{
    QIcon Icon() const override
    {
        return QIcon(":/assets/logo.png");
    }
    QList<PluginGuiComponentType> GetComponents() const override
    {
        return { GUI_COMPONENT_OUTBOUND_EDITOR };
    }
    std::unique_ptr<QvPluginSettingsWidget> createSettingsWidgets() const override
    {
        return {};
    }
    QList<typed_plugin_editor> createInboundEditors() const override
    {
        return {};
    }
    QList<typed_plugin_editor> createOutboundEditors() const override
    {
        return { MakeEditorInfoPair<TrojanOutboundEditor>("trojan", "Trojan") };
    }
    std::unique_ptr<QvPluginMainWindowWidget> createMainWindowWidget() const override
    {
        return nullptr;
    }
};
