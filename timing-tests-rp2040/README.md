# MLSys Timing Tests for Raspberry Pi Pico (RP2040)

## Requirements

### Hardware

* [Raspberry Pi Pico](https://www.raspberrypi.org/products/raspberry-pi-pico/).

### Software

* [GNU ARM Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
* [CMake](https://cmake.org/install/).
* Rasperry Pi Pico SDK:
   ```bash
   git clone -b master https://github.com/raspberrypi/pico-sdk.git
   cd pico-sdk
   git submodule update --init
   ```

## Building the application

### Get the Edge Impulse SDK

Unzip the deployed `C++ library` from your Edge Impulse project and copy only the folders to the root directory of this repository:

   ```
   example-standalone-inferencing-pico/
   ├─ edge-impulse-sdk
   ├─ model-parameters
   ├─ source
   ├─ tflite-model
   ├─ .gitignore
   ├─ CMakeLists.txt
   ├─ LICENSE
   ├─ README.md
   └─ pico_sdk_import.cmake
   ```

### Compile

1. Define Pico SDK path environment variable:
   ```bash
   export PICO_SDK_PATH=<path-to-pico-sdk>
   ```
2. Create the `build` folder:
   ```bash
   mkdir build
   cd build
   ```
3. Compile:
   ```bash
   cmake .. 
   make -j4
   ```

### Flash

Connect the Raspberry Pi Pico to your computer using a micro-USB cable while pressing and holding the **BOOTSEL** button.

Drag and drop the `build/pico_standalone.uf2` file to the **RPI-RP2** disk in your file explorer.

### Serial connection

Use screen, minicom or Serial monitor in Arduino IDE to set up a serial connection over USB. The following UART settings are used: 115200 baud, 8N1.
