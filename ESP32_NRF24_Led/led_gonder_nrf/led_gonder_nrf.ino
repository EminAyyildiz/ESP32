#include <RF24.h>

// Define NRF24L01 pins
#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Adres tanımlama

unsigned long previousMillis = 0;   // Zamanlayıcı için önceki zaman
const long interval = 1000;         // 5 saniye (5000 milisaniye)
bool message = 1;                   // Gönderilecek veri (1 veya 0)

void setup() {
  Serial.begin(115200);
  
  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  
  Serial.println("Gönderici ESP32 hazır");
}

void loop() {
  unsigned long currentMillis = millis();

  // Her 5 saniyede bir veri gönder
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Zamanlayıcıyı sıfırla
    
    // Mesajı sırayla 1 ve 0 olarak değiştir ve gönder
    radio.write(&message, sizeof(message));
    
    Serial.print(message == 1 ? "1 Gönderildi: LED Yansın" : "0 Gönderildi: LED Sönsün");
    Serial.println();

    // Mesajı sıradaki değere değiştir (1 ise 0 yap, 0 ise 1 yap)
    message = !message;
  }
}