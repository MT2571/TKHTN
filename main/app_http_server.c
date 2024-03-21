#include "app_http_server.h"
#include "esp_log.h"
static const char *TAG = "app_http_server ";
httpd_handle_t server;

extern const uint8_t web_start[] asm("_binary_web_html_start");
extern const uint8_t web_end[] asm("_binary_web_html_end");
/* An HTTP GET handler */
static esp_err_t hello_get_handler(httpd_req_t *req)
{
    char*  buf = "My ESP32";
    size_t buf_len = strlen(buf);

    //httpd_resp_set_type(req, "html/web.html");
    httpd_resp_send(req, (const char*)web_start, (const char*)(web_end - web_start));

    /* After sending the HTTP response the old HTTP request
     * headers are lost. Check if HTTP request headers can be read now. */
    if (httpd_req_get_hdr_value_len(req, "Host") == 0) {
        ESP_LOGI(TAG, "Request headers lost");
    }
    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/hello",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = "Hello World!"
};

void start_webserver(void)
{
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &hello);
        
    }
    else
        ESP_LOGI(TAG, "Error starting server!");
    
}

void stop_webserver()
{
    // Stop the httpd server
    httpd_stop(server);
}