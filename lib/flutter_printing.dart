import 'dart:typed_data';

import 'flutter_printing_platform_interface.dart';

class FlutterPrinting {
  // TODO: Remove this.
  Future<String?> getPlatformVersion() {
    return FlutterPrintingPlatform.instance.getPlatformVersion();
  }

  /// Opens the native printing dialog to print said file provided.
  Future<String?> openPrintDialog({String? filePath, Uint8List? fileBytes}) {
    return FlutterPrintingPlatform.instance.openPrintDialog(filePath: filePath, fileBytes: fileBytes);
  }
}
