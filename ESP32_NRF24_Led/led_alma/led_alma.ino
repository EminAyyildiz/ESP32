#include <RF24.h>

// Define NRF24L01 pins
#define CE_PIN 4
#define CSN_PIN 5
#define LED_PIN 2 // LED GPIO 2 üzerinde (düzeltme)

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Adres tanımlama

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize NRF24L01
  if (!radio.begin()) {
    Serial.println("NRF24L01 başlatılamadı!");
    while (1);  // Hata varsa sonsuz döngüye gir
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  Serial.println("Alıcı ESP32 hazır");
}

void loop() {
  if (radio.available()) {
    bool receivedMessage;
    radio.read(&receivedMessage, sizeof(receivedMessage));  // Mesajı al
    
    // Debugging: Gelen veriyi yazdır
    Serial.print("Gelen veri: ");
    Serial.println(receivedMessage);

    // LED'i kontrol et
    if (receivedMessage == 1) {
      digitalWrite(LED_PIN, HIGH);  // LED yanar
      Serial.println("1 Alındı: LED Yandı");
    } else if (receivedMessage == 0) {
      digitalWrite(LED_PIN, LOW);  // LED söner
      Serial.println("0 Alındı: LED Söndü");
    } else {
      Serial.println("Bilinmeyen mesaj alındı!");
    }
  } else {
    // Radyo bağlantısında herhangi bir mesaj alınmadığında hata ayıklama için bilgi yazdırın
    Serial.println("Mesaj alınamadı, bekleniyor...");
    delay(1000);  // 1 saniye bekleyin
  }
}