# dartcef
High-performance Dart bindings for Chromium Embedded Framework (CEF)

**Experimental and it works damn good.**

> [!INFO]
> Similar to [pybindcef](https://github.com)

## Financial Support
If and only if you want to support me anyhow:

[![Donate via](https://img.shields.io/badge/Donate%20via-Wise-9FE870?style=for-the-badge&logo=wise&labelColor=163300)](https://wise.com/pay/business/kartavyashukla)

[![Donate via PayPal](https://img.shields.io/badge/Donate%20via-PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://www.paypal.me/KARTAVYASHUKLA)

[![GitHub Sponsors](https://img.shields.io/github/sponsors/Novfensec?style=for-the-badge&label=Sponsor%20Novfensec&logo=github&color=000000)](https://github.com/sponsors/Novfensec)

## Build Instructions
Better watch a video and there is not a video yet lol!!

You cann follow build instructions for `libcef_dll_wrapper` from here: https://youtu.be/3ZYGRoq0yno?si=SHUavAi3QQssk8rD

### Building `libcef_dll_wrapper`

- Linux

    Install necessary build tools:
    ```
    sudo apt update
    sudo apt install -y \
        build-essential \
        cmake \
        ninja-build
    ```

    Download cef minimal build for your system architecture via https://cef-builds.spotifycdn.com/index.html

    ```bash
    mkdir -p ~/Downloads/cef_binary

    wget https://cef-builds.spotifycdn.com/cef_binary_146.0.9%2Bg3ca6a87%2Bchromium-146.0.7680.165_linux64_minimal.tar.bz2 -O ~/Downloads/cef_binary.tar.bz2

    tar -xjf ~/Downloads/cef_binary.tar.bz2 -C ~/Downloads/cef_binary --strip-components=1
    ```

    Now build the `libcef_dll_wrapper`

    ```bash
    mkdir ~/Downloads/cef_binary/build
    cd ~/Downloads/cef_binary/build

    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    make -j$(nproc)
    ```

    Confirm the build

    ```
    ls ~/Downloads/cef_binary/build/libcef_dll_wrapper/
    ```

    A file named `libcef_dll_wrapper.a` will be listed.

- Windows

    Install the necessary C++ build tools and CMake using Windows Package Manager (`winget`):
    
    ```powershell
    # Install Visual C++ Build Tools
    winget install Microsoft.VisualStudio.BuildTools
    
    # Install CMake
    winget install Kitware.CMake
    ```

    Download cef minimal build for your system architecture via https://cef-builds.spotifycdn.com/index.html

    > **Powershell** commands below

    ```powershell
    mkdir $env:USERPROFILE\Downloads\cef_binary

    wget "https://cef-builds.spotifycdn.com/cef_binary_146.0.9%2Bg3ca6a87%2Bchromium-146.0.7680.165_windows64_minimal.tar.bz2" `
     -O $env:USERPROFILE\Downloads\cef_binary.tar.bz2

    tar -xjf $env:USERPROFILE\Downloads\cef_binary.tar.bz2 -C $env:USERPROFILE\Downloads\cef_binary --strip-components=1
    ```

    Now build the `libcef_dll_wrapper`

    ```powershell
    mkdir $env:USERPROFILE\Downloads\cef_binary\build
    cd $env:USERPROFILE\Downloads\cef_binary\build

    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    cmake --build . --config Release --parallel
    ```

    Confirm the build

    ```
    Get-ChildItem $env:USERPROFILE\Downloads\cef_binary\build\libcef_dll_wrapper\Release
    ```

    A file named `libcef_dll_wrapper.lib` will be listed.

### Building `dartcef` library

- Linux

    Clone this repository.

    ```bash
    git clone https://github.com/Novfensec/dartcef -b main --single-branch --depth 1
    ```

    Now build the library:

    ```bash
    mkdir dartcef/build
    cd dartcef/build

    cmake ..
    make
    ```

- Windows

    Clone this repository.

    ```bash
    git clone https://github.com/Novfensec/dartcef -b main --single-branch --depth 1
    ```

    Now build the extension.

    ```powershell
    mkdir dartcef/build
    cd dartcef/build

    cmake ..
    cmake --build . --config Release
    ```

Navigate to build/Release folder to find `libdartcef.so` (linux) or `dartcef.dll`(windows) library.

### Building `cef_worker`

- Linux

    ```bash
    mkdir dartcef/cef_worker/build
    cd dartcef/cef_worker/build

    cmake ..
    make
    ```

- Window

    ```powershell
    mkdir dartcef/cef_worker/build
    cd dartcef/cef_worker/build

    cmake ..
    cmake --build . --config Release
    ```

### Extracting resources

- Linux

    Copy all files under `cef_binary/Resources` to `cef_binary/Release`

    ```bash
    cp -r ~/Downloads/cef_binary/Resources/* ~/Downloads/cef_binary/Release/
    ```

- Windows

    Copy all files under `cef_binary/Resources` and `cef_binary/Release` right next to the `dartcef.dll` library from wherever you are accessing it.
    All you need to do is make `libcef.dll` available in LD_LIBRARY_PATH and place the files under `cef_binary/Resources` next to `libcef.dll` that's it.
