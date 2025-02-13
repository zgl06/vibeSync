#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_a2dp_api.h"
#include "esp_log.h"

static const char *TAG = "BT_A2DP_SINK";

// 🔹 Bluetooth Event Handler
void a2dp_event_handler(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param)
{
    switch (event)
    {
    case ESP_A2D_CONNECTION_STATE_EVT:
        if (param->conn_stat.state == ESP_A2D_CONNECTION_STATE_CONNECTED)
        {
            ESP_LOGI(TAG, "Bluetooth Connected to Phone!");
        }
        else if (param->conn_stat.state == ESP_A2D_CONNECTION_STATE_DISCONNECTED)
        {
            ESP_LOGI(TAG, "Bluetooth Disconnected.");
        }
        break;

    case ESP_A2D_AUDIO_STATE_EVT:
        ESP_LOGI(TAG, "A2DP Audio Stream State Changed.");
        break;

    default:
        ESP_LOGW(TAG, "Unhandled Bluetooth Event: %d", event);
        break;
    }
}

// 🔹 Process Incoming Audio Data (for future visualization)
void audio_data_callback(const uint8_t *data, uint32_t len)
{
    ESP_LOGI("AUDIO", "Received %d bytes of audio data", len);
    // TODO: Process audio data (FFT for visualization)
}

// 🔹 Bluetooth Initialization
void bluetooth_init()
{
    ESP_LOGI(TAG, "Initializing Bluetooth...");

    if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED)
    {
        ESP_LOGW(TAG, "Bluetooth already enabled, skipping initialization...");
        return;
    }

    // Configure the Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if (esp_bt_controller_init(&bt_cfg) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize controller");
        return;
    }

    if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to enable controller");
        return;
    }

    // Initialize Bluedroid stack
    if (esp_bluedroid_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize Bluedroid");
        return;
    }

    if (esp_bluedroid_enable() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to enable Bluedroid");
        return;
    }

    ESP_LOGI(TAG, "Bluetooth Initialized Successfully!");
}

// 🔹 Initialize A2DP Sink (Enable Audio Reception)
void a2dp_sink_init()
{
    esp_a2d_register_callback(&a2dp_event_handler);            // Register event handler
    esp_a2d_sink_register_data_callback(&audio_data_callback); // Register audio handler
    esp_a2d_sink_init();                                       // Enable A2DP Sink
    ESP_LOGI(TAG, "A2DP Sink Initialized");
}

// 🔹 🎉 NEW FUNCTION: Start Everything with One Call!
void bt_a2dp_sink_start()
{
    bluetooth_init(); // Step 1: Initialize Bluetooth
    a2dp_sink_init(); // Step 2: Set up A2DP Sink
    ESP_LOGI(TAG, "Bluetooth A2DP Sink Started Successfully!");
}
