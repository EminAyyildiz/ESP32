#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "Your wifi name";
const char* password = "Your wifi password";

WebServer server(80);

const int ledPin1 = 2;  // İlk LED'in bağlı olduğu pin
const int ledPin2 = 4;  // İkinci LED'in bağlı olduğu pin

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "  <title>ESP32 Web Server Control 2 LEDs Written By EMIN AYYILDIZ</title>"
                "  <style>"
                "    body {"
                "      text-align: center;"
                "      font-family: Arial, sans-serif;"
                "    }"
                "    .button-container {"
                "      display: flex;"
                "      justify-content: center;"
                "      margin-bottom: 20px;"
                "    }"
                "    .button-container button {"
                "      margin: 0 5px;"
                "      width: 100px;"
                "      height: 40px;"
                "      font-size: 16px;"
                "      border-radius: 5px;"
                "    }"
                "    .button-container button.on {"
                "      background-color: green;"
                "      color: white;"
                "    }"
                "    .button-container button.off {"
                "      background-color: red;"
                "      color: white;"
                "    }"
                "  </style>"
                "</head>"
                "<body>"
                "  <h1>ESP32 Web Server Control 2 LEDs Written By EMIN AYYILDIZ</h1>"
                "  <div class=\"button-container\">"
                "    <button class=\"on\" onclick=\"led1On()\">LED 1 ON</button>"
                "    <button class=\"off\" onclick=\"led1Off()\">LED 1 OFF</button>"
                "  </div>"
                "  <div class=\"button-container\">"
                "    <button class=\"on\" onclick=\"led2On()\">LED 2 ON</button>"
                "    <button class=\"off\" onclick=\"led2Off()\">LED 2 OFF</button>"
                "  </div>"
                "  <script>"
                "    function led1On() {"
                "      fetch('/led1on');"
                "    }"
                "    function led1Off() {"
                "      fetch('/led1off');"
                "    }"
                "    function led2On() {"
                "      fetch('/led2on');"
                "    }"
                "    function led2Off() {"
                "      fetch('/led2off');"
                "    }"
                "  </script>"
                "</body>"
                "</html>"
               );
  });

  server.on("/led1on", HTTP_GET, []() {
    digitalWrite(ledPin1, HIGH);
    server.send(200, "text/plain", "LED 1 is ON");
  });

  server.on("/led1off", HTTP_GET, []() {
    digitalWrite(ledPin1, LOW);
    server.send(200, "text/plain", "LED 1 is OFF");
  });

  server.on("/led2on", HTTP_GET, []() {
    digitalWrite(ledPin2, HIGH);
    server.send(200, "text/plain", "LED 2 is ON");
  });

  server.on("/led2off", HTTP_GET, []() {
    digitalWrite(ledPin2, LOW);
    server.send(200, "text/plain", "LED 2 is OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
