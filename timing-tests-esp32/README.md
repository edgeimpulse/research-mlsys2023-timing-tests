# MLSys Timing Tests for ESP32

This repository runs an exported impulse on the Espressif ESP32. See the documentation at [Running your impulse locally](https://docs.edgeimpulse.com/docs/running-your-impulse-locally-1).

## Requirements

### Hardware

* [ESP-EYE](https://www.espressif.com/en/products/devkits/esp-eye/overview).

While the script is mainly tested with ESP-EYE, other ESP32-based development boards will work too.

### Software

* [ESP IDF 4.4.4](https://docs.espressif.com/projects/esp-idf/en/v4.4.4/esp32/get-started/index.html).

Note that you should use v4.4.4. Version 5.x does not work with Edge Impulse C++ SDK at this time.

## Building the application

### Get the Edge Impulse SDK

Unzip the deployed `C++ library` from your Edge Impulse project and copy only the folders to the root directory of this directory:

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

For non-EON compiler SDK, make sure that `*ctx_start_us = ei_read_timer_us();` is above `#ifdef EI_CLASSIFIER_ALLOCATION_STATIC` in *edge-impulse-sdk/classifier/inferencing_engines/tflite_micro.h*.

### Choose test

In *main/main.cpp*, change `#define TEST` to your desired test (`TEST_KWS`, `TEST_VWW`, `TEST_IMG`).

### Build and upload

Connect ESP32 board to your computer. Do the following in a separate terminal window (NOT inside the VS Code terminal! That has issues with the ESP-IDF Python virtual environment):

1. Set environment: run `get_idf`
2. Navigate to this directory
3. Set target:
   ```bash
   idf.py set-target esp32
   ```
4. Build and flash:
   ```bash
   idf.py -p <serial port> build flash monitor
   ```

### Settings

*sdkconfig* is generated on build. To create a "permanent" set of config settings, change the name to *sdkconfig.defaults*. This project comes with *sdkconfig.defualts*. You should not need to change anything in these settings to run the test, but in case you do, here are some notes:

Some of the tests may fail with default ESP32 values. As such, you will need to enable/disable some settings in menuconfig. You should only need to do this once, but these should be set for all tests.

The watchdog timer should be disabled. If not, you will need to perform the following:

```
idf.py menuconfig
```

Search ('/') for "WDT." Disable the following:

```
[ ]      Watch CPU0 Idle Task (ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0)
[ ]      Watch CPU1 Idle Task (ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1)
```

Yes, this is dangerous, but we need them disabled for this test. You should use WDTs in a real application.

For some tests, you will need to enable larger flash and RAM space for the app. In *menuconfig*, search for "PARTITION_TABLE_TYPE and select **Single factory app (large), no OTA**. From top-level menu, select *Component config* -> *ESP32-specific* -> *Support for external, SPI-connected RAM* -> *SPI RAM Config*. Enable external RAM support (as per [this post](https://www.esp32.com/viewtopic.php?t=18950#p70249)).

### Serial connection

The `monitor` argument should automatically open a serial connection to the ESP32. If you need to reconnect with another program, use screen, minicom or Serial monitor in Arduino IDE to set up a serial connection over USB. The following UART settings are used: 115200 baud, 8N1. Press **ctrl+]** to exit.


### Troubleshooting

If CMake gets caught in an endless loop and complains about times not being synchronized, you might need to touch all the files in this directory. Run:

```
find . -type f -exec touch {} +  
```