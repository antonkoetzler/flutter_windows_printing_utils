import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:path/path.dart';

import 'flutter_printing_platform_interface.dart';

/// An implementation of [FlutterPrintingPlatform] that uses method channels.
class MethodChannelFlutterPrinting extends FlutterPrintingPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_printing');

  @override
  Future<String?> getPlatformVersion() async {
    final version = await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String?> openPrintDialog({String? filePath, Uint8List? fileBytes}) async {
    final String? error = await methodChannel.invokeMethod<String?>(
      'openPrintDialog',
      {
        if (filePath != null) 'file_path': isAbsolute(filePath) ? filePath : normalize(join(Directory.current.path, filePath)),
        if (fileBytes != null) 'file_bytes': fileBytes,
      },
    );
    return error;
  }
}
