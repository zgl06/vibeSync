#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"

#include "esp_a2dp_api.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_main.h"
#include "esp_bt.h"

const char *TAG = "bluetooth_A2DP";

static void bt_connection_cb(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param)
{
    if (event == ESP_A2D_CONNECTION_STATE_EVT)
    {
        switch (param->conn_stat.state)
        {
        case ESP_A2D_CONNECTION_STATE_CONNECTING:
            ESP_LOGI(TAG, "CONNECTING TO BLUETOOTH...");
            break;
        case ESP_A2D_CONNECTION_STATE_CONNECTED:
            ESP_LOGI(TAG, "BLUETOOTH CONNECTED!");
            break;
        case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
            ESP_LOGI(TAG, "BBLUETOOTH DISCONNECTED!");
            break;
        case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
            ESP_LOGI(TAG, "BLUETOOTH DISCONNECTING...");
            break;
        }
    }
}

void bt_init(void)
{
    // init bluedroid, must have
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    // set the bluetooth mode to be discoverable and conectable
    ESP_ERROR_CHECK(esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE));
    ESP_ERROR_CHECK(esp_a2d_register_callback(&bt_connection_cb));
    ESP_ERROR_CHECK(esp_a2d_sink_init());
}