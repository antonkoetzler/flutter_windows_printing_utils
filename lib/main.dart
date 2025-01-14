import 'package:flutter/material.dart';
import 'package:flutter_windows_printing_utils/flutter_windows_printing_utils.dart';

void main() {
  runApp(const MainApp());
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: ElevatedButton(
            onPressed: () => FlutterWindowsPrintingUtils.openPrintDialog(
              filePath: './troll.pdf',
            ),
            child: const Text('Click for platform channels'),
          ),
        ),
      ),
    );
  }
}
