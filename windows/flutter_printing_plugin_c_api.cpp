#include "include/flutter_printing/flutter_printing_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "flutter_printing_plugin.h"

void FlutterPrintingPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  flutter_printing::FlutterPrintingPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
