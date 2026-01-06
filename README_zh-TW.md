[English](README.md) | 中文

# Auto Bridge

讓 libhoudini 和 libndk_translation 可以在 Windows Android 子系統 (WSA) 內共存。

## 建置需求

### Windows

安裝下列工具：
- Android NDK
- CMake
- Ninja

設定環境變數：
- 新增 `ANDROID_NDK_HOME`，設定值為 Android NDK 的根資料夾
- 在 `Path` 內加入 CMake 和 Ninja 的執行檔所在位置

## 建置步驟

### Windows

1. 在 `ndk-apps.txt` 裡面加入要使用 NDK Translation 的 App 的 package 名稱 (一行一個)
2. 執行 `build.bat`，AutoBridge.zip 會儲存在 build 資料夾裡面

## 安裝方式

使用 Magisk 或 KernelSU 安裝 AutoBridge.zip

## 螢幕截圖

![preview](preview.jpg)

截圖環境
- 作業系統：Windows 10 22H2
- 處理器：AMD Ryzen 5 1600
- 顯示卡：AMD Radeon RX 460
- 記憶體：16GB
- WSA 版本：WSABuilds v2407.40000.0.0_LTS_7
- 測試 App：
  - 龍族拼圖 (日版)
    -  使用 NDK Translation 時會在啟動時閃退
  - 蔚藍檔案 (日版)
    -  使用 Houdini 時會在初始化時閃退

## 常見問題

要怎麼知道 App 是使用 Houdini 還是 NDK Translation？
- 預設使用 Houdini
- 只有在 ndk-apps.txt 內的 App 會使用 NDK Translation

安裝之後要怎麼新增要使用 NDK Translation 的 App？
1. 在模擬器內編輯 `/data/local/tmp/autobridge/ndk-apps.txt` 並儲存
2. 重新啟動 App

此模組似乎沒有包含 Houdini 的相關檔案？
- WSA 本身已經有了，所以沒有包含

此模組使用的 NDK Translation 版本？
- 此模組所使用的 NDK Translation 來自
  [vendor_google_proprietary_ndk_translation-prebuilt](https://github.com/supremegamers/vendor_google_proprietary_ndk_translation-prebuilt)
  的 chromeos_zork 分支

可以在其他模擬器使用嗎？
- 應該可以，但沒有測試過
- 需要注意以下幾點：
  1. Houdini 必須安裝在 `vendor` 分區
  2. 已經安裝 NDK Translation 的話會被此模組的版本取代，如果不想被取代的話：
     - 將 `/system/lib64` 的 `arm64` 資料夾重新命名為 `arm64_ndk`
     - 在安裝之前刪除在 AutoBridge.zip 內的相關檔案

## 銘謝

- [WSABuilds](https://github.com/MustardChef/WSABuilds)
- [vendor_google_proprietary_ndk_translation-prebuilt](https://github.com/supremegamers/vendor_google_proprietary_ndk_translation-prebuilt)
