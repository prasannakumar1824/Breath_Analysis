#include <Arduino.h>
#include <driver/i2s.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define I2S_WS 15   // LRCL (Word Select) pin
#define I2S_SD 32   // DOUT (Data Out) pin
#define I2S_SCK 14  // BCLK (Bit Clock) pin

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 1024

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* server_url = "http://your-server-address/receive_data"; // Replace with your server endpoint

int16_t samples[BUFFER_SIZE];

void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = BUFFER_SIZE,
        .use_apll = false
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD
    };
    
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void recordAudio() {
    size_t bytes_read;
    i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytes_read, portMAX_DELAY);
}

String diagnoseBreathing(int breath_rate) {
    if (breath_rate < 12) {
        return "Possible Bradypnea (Slow Breathing)";
    } else if (breath_rate > 20) {
        return "Possible Tachypnea (Fast Breathing)";
    } else {
        return "Normal Breathing Rate";
    }
}

int detectBreathRate() {
    int peak_count = 0;
    int threshold = 5000;
    bool peak_detected = false;
    
    for (int i = 1; i < BUFFER_SIZE; i++) {
        if (samples[i] > threshold && !peak_detected) {
            peak_detected = true;
            peak_count++;
        } else if (samples[i] < threshold) {
            peak_detected = false;
        }
    }
    
    return peak_count * (60 / (BUFFER_SIZE / SAMPLE_RATE));
}

void sendDataToMobile(int breath_rate, String diagnosis) {
    HTTPClient http;
    http.begin(server_url);
    http.addHeader("Content-Type", "application/json");
    
    String jsonPayload = "{\"breath_rate\": " + String(breath_rate) + ", \"diagnosis\": \"" + diagnosis + "\"}";
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        Serial.println("Data sent successfully");
    } else {
        Serial.print("Error sending data: ");
        Serial.println(httpResponseCode);
    }
    
    http.end();
}

void setup() {
    Serial.begin(115200);
    setupI2S();
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println(" Connected to WiFi");
}

void loop() {
    recordAudio();
    int breath_rate = detectBreathRate();
    String diagnosis = diagnoseBreathing(breath_rate);
    
    Serial.print("Estimated Breath Rate: ");
    Serial.print(breath_rate);
    Serial.println(" breaths per minute");
    Serial.print("Diagnosis: ");
    Serial.println(diagnosis);
    
    sendDataToMobile(breath_rate, diagnosis);
    
    delay(5000);
}
