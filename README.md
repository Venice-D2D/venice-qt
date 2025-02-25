# venice-qt
Desktop implemetation of venice en C++ and QT

## Pre-requisites
- QMake version 3.1 or higher or CMake 3.29 or higuer
- Qt version 5.12.8 or higher
- Make version 4.2.1 or higher
- Protobuf 3.21.12 or higuer

## Update Git Submodules

After clonning the repository, execute the following commands

```
git submodule update --init --recursive
git submodule update --recursive
```

### Pre-requisites Installation in Ubuntu 22.04
- For installing QT et QMake with related Bluetooth dependencies :

```
sudo apt install qtbase5-dev qt5-qmake qtconnectivity5-qtconnectivity5-dev cmake
```
- If you find the issue `Project ERROR: Cannot run compiler 'g++'` you can also install `build-essential`

```
sudo apt install build-essential
```
- Once the project is cloned, you need to update the submodules:

```
git submodule update --init --recursive
```

## Build via the CLI with qmake
- Exeute the following commands:
```
cd qmake
./build.sh
```

## Build via the CLI with cmake
- Exeute the following commands:
```
cd cmake
chmod u+x build.sh
./build.sh
```

## Run via the CLI
- Execute the following commands:
```
cd build
chmod u+x build.sh
sudo ./venice-qt
```

  - `sudo` privileges are required in order to access the Bluetooth adapter


## Notes

- Service advertising is based on this [library](https://doc.qt.io/qt-5/qtbluetooth-le-overview.html#advertising-services).  
- `qtconnectivity5-dev`  is required.
- The current version only allows the advertising of the Venice File Service Exchange.
- You need to use a USB dongle Bluetooth. Currently, the internal Bluetooth adapter is not detected.
- To execute a local test by using Android Studio:
  - `Flutter 3.13.9` is required. `Flutter 3.16` and `3.19` produces an error : `file too long error` if the hard disk is encrypted.
  - For installing `SDK Android` packages via `Android Studio` : `tools->Android SDK Manager` and to look on the tabs the available packages
  - To avoid `Error: A value of type 'List<int>' can't be assigned to a variable of type 'Uint8List`
    - To take a look [here](https://coflutter.com/dart-flutter-how-to-convert-string-to-uint8list/)
      - Files to modify with the line `Uint8List.fromList(data.toString().codeUnits);`:
        - Dart Packages:
          - ble_bootstrap_channel-XXXX
          - ble_bootstrap_channel
          - views/copypaste/sender_view.dart
- To install by hand protobuf 3.21.12 execute the following commands:
```
wget https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.21.12.tar.gz
tar -xvf v3.21.12.tar.gz
cd protobuf-3.21.12/
./autogen.sh
./configure
make -j$(nproc)
sudo make install
sudo ldconfig
sudo ln -s /usr/local/bin/protoc /usr/bin/protoc
```
