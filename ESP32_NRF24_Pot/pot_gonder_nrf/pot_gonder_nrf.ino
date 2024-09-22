#include <RF24.h>

// Define NRF24L01 pins
#define CE_PIN 4
#define CSN_PIN 5

// Potansiyometre pin
#define POT_PIN 34  // GPIO34 (analog giriş)

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Adres tanımlama

void setup() {
  Serial.begin(115200);
  pinMode(POT_PIN, INPUT);
  
  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  
  Serial.println("Gönderici ESP32 hazır");
}

void loop() {
  int potValue = analogRead(POT_PIN);  // Potansiyometreden değer oku
  radio.write(&potValue, sizeof(potValue));  // Değeri gönder
  
  Serial.print("Potansiyometre Değeri Gönderildi: ");
  Serial.println(potValue);
  
  delay(100);  // Küçük bir gecikme (100 ms)
}