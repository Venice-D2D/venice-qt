# venice-qt
Desktop implemetation of venice en C++ and QT

## Pre-requisites
- QMake version 3.1
- Qt version 5.12.8
- Make version 4.2.1

## Build via the CLI
- Create a directory `build` on the project directory
```
mkdir build
cd build
```
- Execute the following commands:

```
qmake ../venice-qt.pro
make
```
## Run via the CLI

- Execute the following command:
```
sudo ./venice-qt
```

  - `sudo` privileges are required in order to access the Bluetooth adapter



## Notes

- Service advertising is based on this [library](https://doc.qt.io/qt-5/qtbluetooth-le-overview.html#advertising-services).  
- `qtconnectivity5-dev`  is required.
- The current version only allows the advertising of the Venice File Service Exchange.
- You need to use a USB dongle Bluetooth. Currently, the internal Bluetooth adapter is not detected.
-   - To execute a local test by using Android Studio:
    - `Flutter 3.13.9` is required. `Flutter 3.16` and `3.19` produces an error : `file too long error` if the hard disk is encrypted.

- To execute a local test by using Android Studio:
  - `Flutter 3.13.9` is required. `Flutter 3.16` and `3.19` produces an error : `file too long error` if the hard disk is encrypted.
  - For install `SDK Android` packages via `Android Studio` : `tools->Android SDK Manager` and to look on the tabs the available packages
  - To avoid `Error: A value of type 'List<int>' can't be assigned to a variable of type 'Uint8List`
    - Take a look [here](https://coflutter.com/dart-flutter-how-to-convert-string-to-uint8list/)
      - Files to modify with the line `Uint8List.fromList(data.toString().codeUnits);`:
        - Dart Packages:
          - ble_bootstrap_channel-XXXX
          - ble_bootstrap_channel
          - views/copypaste/sender_view.dart
