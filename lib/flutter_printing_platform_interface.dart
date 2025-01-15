import 'dart:typed_data';

import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_printing_method_channel.dart';

abstract class FlutterPrintingPlatform extends PlatformInterface {
  /// Constructs a FlutterPrintingPlatform.
  FlutterPrintingPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterPrintingPlatform _instance = MethodChannelFlutterPrinting();

  /// The default instance of [FlutterPrintingPlatform] to use.
  ///
  /// Defaults to [MethodChannelFlutterPrinting].
  static FlutterPrintingPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [FlutterPrintingPlatform] when
  /// they register themselves.
  static set instance(FlutterPrintingPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  // TODO: Remove this.
  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  /// Opens the native printing dialog to print said file provided.
  Future<String?> openPrintDialog({String? filePath, Uint8List? fileBytes}) {
    throw UnimplementedError('openPrintDialog() has not been implemented.');
  }
}
