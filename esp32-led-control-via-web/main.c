#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
//#define WIFI_SSID "Pixel"
//#define WIFI_PASS "1234567890"
#define WIFI_SSID "Pixel"
#define WIFI_PASS "1234567890"
#define LED_GPIO GPIO_NUM_4
static const char *TAG = "LED_WIFI";

// HTML webpage

static const char *html_page =
"<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"
"<style>"
"body { font-family: Arial; background-image: url('https://images.unsplash.com/photo-1524253482453-3fed8d2fe12b'); background-size: cover; color: white; text-align: center; padding-top: 60px; }"
"h2 { color: #fff; margin-bottom: 30px; text-shadow: 2px 2px 5px #000; }"
"button { padding: 15px 35px; font-size: 20px; margin: 10px; border: none; border-radius: 12px; cursor: pointer; box-shadow: 2px 2px 5px #333; }"
".on { background-color: #4CAF50; color: white; }"
".off { background-color: #f44336; color: white; }"
"</style></head><body>"
"<h2>ESP32 LED CONTROL</h2>"
"<button class='on' onclick=\"fetch('/on')\">Turn ON</button>"
"<button class='off' onclick=\"fetch('/off')\">Turn OFF</button>"
"</body></html>";



// HTTP Handlers
esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t led_on_handler(httpd_req_t *req) {
    gpio_set_level(LED_GPIO, 1);
    httpd_resp_send(req, "LED ON", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t led_off_handler(httpd_req_t *req) {
    gpio_set_level(LED_GPIO, 0);
    httpd_resp_send(req, "LED OFF", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Web server
httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler
        };
        httpd_register_uri_handler(server, &root_uri);

        httpd_uri_t on_uri = {
            .uri = "/on",
            .method = HTTP_GET,
            .handler = led_on_handler
        };
        httpd_register_uri_handler(server, &on_uri);

        httpd_uri_t off_uri = {
            .uri = "/off",
            .method = HTTP_GET,
            .handler = led_off_handler
        };
        httpd_register_uri_handler(server, &off_uri);
    }

    return server;
}

// Event handlers
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected. Reconnecting...");
        esp_wifi_connect();
    }
}

static void got_ip_handler(void *arg, esp_event_base_t event_base,
                           int32_t event_id, void *event_data) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(TAG, "Connected with IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
    start_webserver();
}

// Wi-Fi
void wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAG, "Connecting to Wi-Fi: %s", wifi_config.sta.ssid);
}
uint8_t mac[6];


// Main
void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);

    wifi_init_sta();
    esp_wifi_get_mac(WIFI_IF_STA, mac);
        ESP_LOGI(TAG, "MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

