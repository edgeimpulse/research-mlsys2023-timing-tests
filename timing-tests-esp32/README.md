# MLSys Timing Tests for ESP32

This repository runs an exported impulse on the Espressif ESP32. See the documentation at [Running your impulse locally](https://docs.edgeimpulse.com/docs/running-your-impulse-locally-1).

## Requirements

### Hardware

* [ESP-EYE](https://www.espressif.com/en/products/devkits/esp-eye/overview).

While the script is mainly tested with ESP-EYE, other ESP32-based development boards will work too.

### Software

* [ESP IDF 4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/get-started/index.html).

## Building the application

### Get the Edge Impulse SDK

Unzip the deployed `C++ library` from your Edge Impulse project and copy only the folders to the root directory of this repository:

   ```
   example-standalone-inferencing-espressif-esp32/
   ├─ edge-impulse-sdk
   ├─ model-parameters
   ├─ main
   ├─ tflite-model
   ├─ .gitignore
   ├─ CMakeLists.txt
   ├─ LICENSE
   ├─ README.md
   ├─ sdkconfig
   ├─ sdkconfig.old
   └─ partitions.csv   
   ```

IMPORTANT! For accurate timing, make sure that `*ctx_start_us = ei_read_timer_us();` is above `TfLiteStatus init_status = trained_model_init(ei_aligned_calloc);` in the `inference_tflite_setup(...)` function in *edge-impulse-sdk/classifier/inferencing_engines/tflite_eon.h*.

### Build and upload

Connect ESP32 board to your computer.

1. Set environment: run `install` and `export` scripts in `<espressif-install>/frameworks/esp-idf-vx.x.x/`
2. Navigate to this directory
3. Set target:
   ```bash
   idf.py set-target esp32
   ```
4. Build and flash:
   ```bash
   idf.py -p <serial port> build flash monitor
   ```

### Serial connection

The `monitor` argument should automatically open a serial connection to the ESP32. If you need to reconnect with another program, use screen, minicom or Serial monitor in Arduino IDE to set up a serial connection over USB. The following UART settings are used: 115200 baud, 8N1.
