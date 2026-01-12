English | [繁體中文](README_zh-TW.md)

# Auto Bridge

Allows libhoudini and libndk_translation to coexist in Windows Subsystem for Android (WSA).

## Build requirements

### Windows

Install the following tools:
- Android NDK
- CMake
- Ninja

Set environment variables:
- Add `ANDROID_NDK_HOME` and set it to the Android NDK root directory.
- Add CMake and Ninja to `Path`.

## Build steps

### Windows

1. Add the package names of the apps that need to use NDK Translation to `ndk-apps.txt` (one per line).
2. Run `build.bat`, AutoBridge.zip will be saved in the build directory.

## Installation

Install AutoBridge.zip using Magisk or KernelSU.

## Screenshots

![preview](preview.jpg)

Screenshot environment:
- OS: Windows 10 22H2
- CPU: AMD Ryzen 5 1600
- GPU: AMD Radeon RX 460
- RAM: 16 GB
- WSA version: WSABuilds v2407.40000.4.0_LTS_7-with-KernelSU-v1.0.5 (June 02, 2025)
- Android version: 13
- Tested apps:
  - Puzzle & Dragons (JP), ver.22.9.1
    - Crashes on launch when using NDK Translation.
  - Blue Archive (JP), ver.1.64.395492
    - Crashes during initialization when using Houdini.

## FAQ

How can I tell an app is using Houdini or NDK Translation?
- Houdini is used by default.
- Only apps listed in ndk-apps.txt will use NDK Translation.

How to add new apps to use NDK Translation after installation?
  1. In the emulator, edit `/data/local/tmp/autobridge/ndk-apps.txt` and save the file.
  2. Restart the application.

This module does not seem to include Houdini binaries?
- Houdini is already included in WSA, so it is not bundled with this module.

Which version of NDK Translation does this module use?
- The NDK Translation used by this module is from
  [vendor_google_proprietary_ndk_translation-prebuilt](https://github.com/supremegamers/vendor_google_proprietary_ndk_translation-prebuilt),
  branch chromeos_zork.

Can this module be used on other Android emulators?
- It should work, but it has not been tested.
- Please note the following:
  1. Houdini must be installed in the `vendor` partition.
  2. If NDK Translation is already installed, it will be replaced by this module, to prevent replacement:
     - Rename the `arm64` directory under `/system/lib64` to `arm64_ndk`.
     - Remove the relevant files from AutoBridge.zip before installation.

## Credits
- [WSABuilds](https://github.com/MustardChef/WSABuilds)
- [vendor_google_proprietary_ndk_translation-prebuilt](https://github.com/supremegamers/vendor_google_proprietary_ndk_translation-prebuilt)
