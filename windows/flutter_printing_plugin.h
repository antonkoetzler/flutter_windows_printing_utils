#ifndef FLUTTER_PLUGIN_FLUTTER_PRINTING_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_PRINTING_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace flutter_printing
{

    class FlutterPrintingPlugin : public flutter::Plugin
    {
    public:
        static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

        FlutterPrintingPlugin();

        virtual ~FlutterPrintingPlugin();

        // Disallow copy and assign.
        FlutterPrintingPlugin(const FlutterPrintingPlugin &) = delete;
        FlutterPrintingPlugin &operator=(const FlutterPrintingPlugin &) = delete;

        // Called when a method is called on this plugin's channel from Dart.
        void HandleMethodCall(
            const flutter::MethodCall<flutter::EncodableValue> &method_call,
            std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
    };

} // namespace flutter_printing

#endif // FLUTTER_PLUGIN_FLUTTER_PRINTING_PLUGIN_H_
