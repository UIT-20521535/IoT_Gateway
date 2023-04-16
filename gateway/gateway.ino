#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_Gateway";
const char* password = "123456789";
int port = 80; // Cổng kết nối

// Khởi tạo đối tượng WebServer
WebServer server(port);

void setup() {
  Serial.begin(9600);

  // Thiết lập chế độ AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("");
  Serial.println("AP started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Đăng ký đường dẫn /sensorData
  server.on("/sensorData", handleSensorData);

  // Khởi động WebServer
  server.begin();
}

void loop() {
  // Xử lý các yêu cầu HTTP đến từ node cảm biến
  server.handleClient();

  //Đẩy dữ liệu cảm biến lên máy tính bằng cách sử dụng Serial Monitor.
  while (Serial.available()) {
    char c = Serial.read();
    Serial.write(c);
  }
}

// Hàm xử lý yêu cầu HTTP từ node cảm biến
void handleSensorData() {
  float temperature = server.arg("temperature").toFloat();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("*C\n");

  server.send(200, "text/plain", "OK");
}
