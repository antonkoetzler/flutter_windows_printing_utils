#include "flutter_printing_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <fstream>

HWND hwnd = nullptr;

namespace flutter_printing
{

  // static
  void FlutterPrintingPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "flutter_printing",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<FlutterPrintingPlugin>();

    if (auto view = registrar->GetView())
    {
      hwnd = view->GetNativeWindow();
    }

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  FlutterPrintingPlugin::FlutterPrintingPlugin() {}

  FlutterPrintingPlugin::~FlutterPrintingPlugin() {}

  void FlutterPrintingPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    std::string method_name = method_call.method_name();

    if (method_name.compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    }
    else if (method_name.compare("openPrintDialog") == 0)
    {
      // Provided method channel arguments.
      const flutter::EncodableMap *arguments = std::get_if<flutter::EncodableMap>(method_call.arguments());

      // `file_path` not provided in the arguments.
      if (!arguments)
      {
        result->Error("BadArguments", "Expected map with valid file path.");
        return;
      }

      // Check if the file path or bytes are provided
      auto file_path_iterator = arguments->find(flutter::EncodableValue("file_path"));
      auto file_bytes_iterator = arguments->find(flutter::EncodableValue("file_bytes"));

      std::string file_path;

      if (file_path_iterator != arguments->end())
      {
        // If file path is provided, use it
        file_path = std::get<std::string>(file_path_iterator->second);
      }
      else if (file_bytes_iterator != arguments->end())
      {
        // If file bytes are provided, write them to a temporary file
        const auto &file_bytes = std::get<std::vector<int>>(file_bytes_iterator->second);

        // Create a temporary file path
        const std::string temp_file_path = "temp_print_file.pdf";
        std::ofstream output_file(temp_file_path, std::ios::binary);
        if (!output_file)
        {
          result->Error("FileError", "Failed to write temporary file.");
          return;
        }

        output_file.write(reinterpret_cast<const char *>(file_bytes.data()), file_bytes.size());
        file_path = temp_file_path;
      }
      else
      {
        result->Error("BadArguments", "Expected either file path or file bytes.");
        return;
      }

      // Call the print dialog function with the file path
      auto error = OpenPrintDialog(file_path);
      if (error.has_value())
      {
        std::remove(file_path.c_str());
        result->Error(error.value());
        return;
      }

      std::remove(file_path.c_str());
      result->Success("");
    }
    else
    {
      result->NotImplemented();
    }
  }

  // Function to retrieve a formatted error message from a DWORD error code.
  std::string GetErrorMessage(DWORD error_code)
  {
    char *message_buffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&message_buffer),
        0,
        nullptr);

    std::string message(message_buffer, size);
    LocalFree(message_buffer);
    return message;
  }

  // Function that launches the Windows print dialog for a given file.
  std::optional<std::string> OpenPrintDialog(const std::string &file_path)
  {
    PRINTDLG printDlg = {0};
    printDlg.lStructSize = sizeof(PRINTDLG);
    printDlg.Flags = PD_RETURNDC; // Request a device context
    printDlg.hwndOwner = nullptr; // No owner window

    if (PrintDlg(&printDlg))
    {
      // Successfully opened the print dialog
      // Clean up the allocated resources
      if (printDlg.hDevMode)
        GlobalFree(printDlg.hDevMode);
      if (printDlg.hDevNames)
        GlobalFree(printDlg.hDevNames);
      if (printDlg.hDC)
        DeleteDC(printDlg.hDC);

      return std::nullopt; // No error
    }

    DWORD errorCode = CommDlgExtendedError();
    if (errorCode != 0)
    {
      return "PrintDlg failed with error code: " + std::to_string(errorCode);
    }
    else
    {
      return "Print dialog was canceled by the user.";
    }
  }
} // namespace flutter_printing