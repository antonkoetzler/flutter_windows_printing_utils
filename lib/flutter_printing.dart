import 'dart:io';

import 'package:path/path.dart';
import 'package:flutter/services.dart';

/// Class storing all the this package's functionalities.
final class FlutterWindowsPrinting {
  /// Platform channel.
  static const platform = MethodChannel('antonkoetzler/flutter_windows_printing');

  /// Opens the print dialog with pre-print PDF styling (i.e. not just the printer dropdown & button to print).
  static Future<String?> openPrintDialog({String? filePath, Uint8List? fileBytes}) async {
    assert(
      (filePath != null) ^ (fileBytes != null),
      '[FlutterWindowsPrinting.openPrintDialog]: Either [filePath] (x)or [fileBytes].',
    );

    try {
      final result = await platform.invokeMethod<String?>(
        'openPrintDialog',
        {
          if (filePath != null) 'file_path': isAbsolute(filePath) ? filePath : normalize(join(Directory.current.path, filePath)),
          if (fileBytes != null) 'file_bytes': fileBytes,
        },
      );

      return result;
    } on PlatformException catch (error) {
      return 'Error launching printing dialog: "$error".';
    }
  }
}
