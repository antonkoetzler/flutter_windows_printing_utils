import 'package:flutter/services.dart';

/// Class storing all the this package's functionalities.
final class FlutterWindowsPrintingUtils {
  /// Platform channel.
  static const platform = MethodChannel('antonkoetzler/flutter_windows_printing_utils');

  /// Opens the print dialog with pre-print PDF styling (i.e. not just the printer dropdown & button to print).
  static Future<String?> openPrintDialog({String? filePath, Uint8List? fileBytes}) async {
    assert(
      (filePath != null) ^ (fileBytes != null),
      '[FlutterWindowsPrintingUtils.openPrintDialog]: Either [filePath] (x)or [fileBytes].',
    );

    try {
      final result = await platform.invokeMethod<String?>(
        'openPrintDialog',
        {
          if (filePath != null) 'file_path': filePath,
          if (fileBytes != null) 'file_bytes': fileBytes,
        },
      );
      print('YES HELLO WORLD THIS IS COOL: $result');
      return result;
    } on PlatformException catch (error) {
      return 'Error launching printing dialog: "$error".';
    }
  }
}
