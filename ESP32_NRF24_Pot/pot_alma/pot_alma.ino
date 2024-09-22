#include <RF24.h>
#include <Servo_ESP32.h>

// Define NRF24L01 pins
#define CE_PIN 4
#define CSN_PIN 5

// Servo motor pin
static const int  SERVO_PIN = 14;  // GPIO2

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Adres tanımlama

Servo_ESP32 myServo;  // Servo motor nesnesi

void setup() {
  Serial.begin(115200);
  
  // Initialize NRF24L01
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  myServo.attach(SERVO_PIN);  // Servo motoru bağla
  
  Serial.println("Alıcı ESP32 hazır");
}

void loop() {
  if (radio.available()) {
    int receivedValue;
    radio.read(&receivedValue, sizeof(receivedValue));  // Mesajı al

    // Potansiyometre değerini 0-180 aralığına çevir
    int servoValue = map(receivedValue, 0, 4095, 0, 180);
    myServo.write(servoValue);  // Servo motorun açı değerini ayarla
    
    Serial.print("Gelen Değer: ");
    Serial.print(receivedValue);
    Serial.print(" - Servo Açısı: ");
    Serial.println(servoValue);
  }
}