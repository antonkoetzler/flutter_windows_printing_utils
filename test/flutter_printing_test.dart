import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_printing/flutter_printing.dart';
import 'package:flutter_printing/flutter_printing_platform_interface.dart';
import 'package:flutter_printing/flutter_printing_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockFlutterPrintingPlatform
    with MockPlatformInterfaceMixin
    implements FlutterPrintingPlatform {

  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final FlutterPrintingPlatform initialPlatform = FlutterPrintingPlatform.instance;

  test('$MethodChannelFlutterPrinting is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelFlutterPrinting>());
  });

  test('getPlatformVersion', () async {
    FlutterPrinting flutterPrintingPlugin = FlutterPrinting();
    MockFlutterPrintingPlatform fakePlatform = MockFlutterPrintingPlatform();
    FlutterPrintingPlatform.instance = fakePlatform;

    expect(await flutterPrintingPlugin.getPlatformVersion(), '42');
  });
}
