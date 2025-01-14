#include "flutter_window.h"

#include <optional>
#include <fstream>

#include "flutter/generated_plugin_registrant.h"
#include <flutter/method_channel.h>
#include <flutter/standard_method_codec.h>
#include "../../build/windows/x64/runner/open_print_dialog.cpp"

FlutterWindow::FlutterWindow(const flutter::DartProject& project)
	: project_(project) {
}

FlutterWindow::~FlutterWindow() {}

bool FlutterWindow::OnCreate() {
	if (!Win32Window::OnCreate()) {
		return false;
	}

	RECT frame = GetClientArea();

	// The size here must match the window dimensions to avoid unnecessary surface
	// creation / destruction in the startup path.
	flutter_controller_ = std::make_unique<flutter::FlutterViewController>(
		frame.right - frame.left, frame.bottom - frame.top, project_);
	// Ensure that basic setup of the controller was successful.
	if (!flutter_controller_->engine() || !flutter_controller_->view()) {
		return false;
	}
	RegisterPlugins(flutter_controller_->engine());

	// Method channel of `flutter_windows_printing_utils`.
	flutter::MethodChannel<> channel(
		flutter_controller_->engine()->messenger(), "antonkoetzler/flutter_windows_printing_utils",
		&flutter::StandardMethodCodec::GetInstance());
	channel.SetMethodCallHandler(
		[](const flutter::MethodCall<>& call,
			std::unique_ptr<flutter::MethodResult<>> result) {
				if (call.method_name() == "openPrintDialog") {
					// Provided method channel arguments.
					const auto* arguments = std::get_if<flutter::EncodableMap>(call.arguments());

					// `file_path` not provided in the arguments.
					if (!arguments) {
						result->Error("BadArguments", "Expected map with valid file path.");
						return;
					}

					// Check if the file path or bytes are provided
					auto file_path_iterator = arguments->find(flutter::EncodableValue("file_path"));
					auto file_bytes_iterator = arguments->find(flutter::EncodableValue("file_bytes"));

					std::string file_path;

					if (file_path_iterator != arguments->end()) {
						// If file path is provided, use it
						file_path = std::get<std::string>(file_path_iterator->second);
					}
					else if (file_bytes_iterator != arguments->end()) {
						// If file bytes are provided, write them to a temporary file
						const auto& file_bytes = std::get<std::vector<int>>(file_bytes_iterator->second);

						// Create a temporary file path
						const std::string temp_file_path = "temp_print_file.pdf";
						std::ofstream output_file(temp_file_path, std::ios::binary);
						if (!output_file) {
							result->Error("FileError", "Failed to write temporary file.");
							return;
						}

						output_file.write(reinterpret_cast<const char*>(file_bytes.data()), file_bytes.size());
						file_path = temp_file_path;
					}
					else {
						result->Error("BadArguments", "Expected either file path or file bytes.");
						return;
					}

					// Call the print dialog function with the file path
					auto error = OpenPrintDialog(file_path);
					if (error.has_value()) {
						std::cout << "here" << error.value() << std::endl;
						std::remove(file_path.c_str());
						result->Error(error.value());
						return;
					}

					std::remove(file_path.c_str());
					result->Success("");
				}
				else {
					result->NotImplemented();
				}
		});


	SetChildContent(flutter_controller_->view()->GetNativeWindow());

	flutter_controller_->engine()->SetNextFrameCallback([&]() {
		this->Show();
		});

	// Flutter can complete the first frame before the "show window" callback is
	// registered. The following call ensures a frame is pending to ensure the
	// window is shown. It is a no-op if the first frame hasn't completed yet.
	flutter_controller_->ForceRedraw();

	return true;
}

void FlutterWindow::OnDestroy() {
	if (flutter_controller_) {
		flutter_controller_ = nullptr;
	}

	Win32Window::OnDestroy();
}

LRESULT
FlutterWindow::MessageHandler(HWND hwnd, UINT const message,
	WPARAM const wparam,
	LPARAM const lparam) noexcept {
	// Give Flutter, including plugins, an opportunity to handle window messages.
	if (flutter_controller_) {
		std::optional<LRESULT> result =
			flutter_controller_->HandleTopLevelWindowProc(hwnd, message, wparam,
				lparam);
		if (result) {
			return *result;
		}
	}

	switch (message) {
	case WM_FONTCHANGE:
		flutter_controller_->engine()->ReloadSystemFonts();
		break;
	}

	return Win32Window::MessageHandler(hwnd, message, wparam, lparam);
}
