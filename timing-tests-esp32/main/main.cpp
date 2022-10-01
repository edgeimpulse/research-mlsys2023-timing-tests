/* Edge Impulse Espressif ESP32 Standalone Inference ESP IDF Example
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Test definitions
#define TEST_KWS        0
#define TEST_VWW        1
#define TEST_IMG        2

// Select test
#define TEST            TEST_KWS

/* Include ----------------------------------------------------------------- */
#include <stdio.h>

#include "ei_run_classifier.h"

#include "driver/gpio.h"
#include "sdkconfig.h"

// Import raw features based on chosen test
#if TEST == TEST_KWS
    #include "features-kws.h"
#elif TEST == TEST_VWW
    #include "features-vww.h"
#elif TEST == TEST_IMG
    #include "features-img.h"
#else:
    #error Unknown test type
#endif

// Setting
static const int num_tests = 100;
static const bool ei_debug = false;
static const int wdt_yield_ms = 10;

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr)
{
  memcpy(out_ptr, features + offset, length * sizeof(float));
  return 0;
}


extern "C" int app_main()
{
    uint64_t timing_dsp_us = 0;
    uint64_t timing_classification_us = 0;
    uint64_t timing_anomaly_us = 0;
    uint64_t timing_total_us = 0;
    uint64_t timestamp_us;
    EI_IMPULSE_ERROR res;

    ei_sleep(100);

    // Print test
    ei_printf("MLSys ESP32 timing test\r\n");

    // Check to make sure the size of the features is correct
    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE)
    {
        ei_printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
                EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    // Zero out the results
    ei_impulse_result_t result = { 0 };

    // Set up our signal struct to feed to inference
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = &raw_feature_get_data;

    // Run inference once (the first time takes longer, as buffers need to be setup)
    res = run_classifier(&features_signal, &result, ei_debug);
    
    // Print classification results
    ei_printf("Classification results:");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) 
    {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }
    #if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: %.3f\n", result.anomaly);
    #endif
    ei_printf("\r\n");

    // Run tests
    for (int i = 0; i < num_tests; i++) 
    {
        ei_printf("Running test %i / %i\r\n", (i + 1), num_tests);
        timestamp_us = ei_read_timer_us();
        res = run_classifier(&features_signal, &result, ei_debug);
        timing_total_us += ei_read_timer_us() - timestamp_us;
        timing_dsp_us += result.timing.dsp_us;
        timing_classification_us += result.timing.classification_us;
        timing_anomaly_us += result.timing.anomaly_us;

        // Yield for at least 10 ms to keep watchdog timer fed
        ei_sleep(wdt_yield_ms);
    }

    // Print timing test results
    ei_printf("\r\n");
    ei_printf("Timing results\r\n");
    ei_printf("---\r\n");
    ei_printf("Pre-processing: %lu us\r\n", (unsigned long)(timing_dsp_us / num_tests));
    ei_printf("Classification: %lu us\r\n", (unsigned long)(timing_classification_us / num_tests));
    ei_printf("Anomaly: %lu us\r\n", (unsigned long)(timing_anomaly_us / num_tests));
    ei_printf("Total: %lu us\r\n", (unsigned long)(timing_total_us / num_tests));

    // Do nothing
    while (true)
    {
        ei_sleep(1000);
    }
}