import 'package:flutter/services.dart';

/// Class storing all the this package's functionalities.
final class FlutterWindowsPrintingUtils {
  /// Platform channel.
  static const platform = MethodChannel('com.antonkoetzler/flutter_windows_printing_utils');

  /// Opens the print dialog with pre-print PDF styling (i.e. not just the printer dropdown & button to print).
  static Future<String> openPrintDialog() async {
    try {
      final result = await platform.invokeMethod('openPrintDialog');
      return result;
    } on PlatformException catch (error) {
      return 'Error launching printing dialog: "$error".';
    }
  }
}
