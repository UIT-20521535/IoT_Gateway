#include <WiFi.h>
#include <HTTPClient.h>
#include <dht11.h>
DHT11 DHT;
#define DHT_PIN 5


const char* ssid = "ESP32_Gateway";
const char* password = "123456789";
const char* gatewayAddress = "192.168.4.1"; // Địa chỉ IP của Gateway
const int gatewayPort = 80; // Cổng kết nối của Gateway

WiFiClient wifiClient;

void setup() {
  Serial.begin(9600);

  // Kết nối đến WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {

  // Đọc giá trị từ cảm biến
  DHT.read(DHT_PIN);
  float temp = DHT.temperature;
  
  // Gửi giá trị đến Gateway qua đường WiFi
  if (wifiClient.connect(gatewayAddress, gatewayPort)) {
    String url = "/sensorData?temperature=" + String(temp);
    HTTPClient http;
    http.begin(wifiClient, gatewayAddress, gatewayPort, url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      Serial.print("Sensor data sent to Gateway ");
      Serial.print(temp);
      Serial.println("*C");
    } else {
      Serial.println("Failed to send sensor data to Gateway");
    }
    http.end();
  } else {
    Serial.println("Failed to connect to Gateway");
  }

  delay(5000);
}
