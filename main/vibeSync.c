#include <stdio.h>
#include "esp_log.h"
#include "bt_a2dp.h"
#include "bt_a2dp_sink.h"
void app_main(void)
{
    // bt_init();
    ESP_LOGI("MAIN", "Starting Bluetooth A2DP Sink...");

    bt_a2dp_sink_start(); // 🎉 Calls EVERYTHING

    ESP_LOGI("MAIN", "Bluetooth Setup Complete!");
}
