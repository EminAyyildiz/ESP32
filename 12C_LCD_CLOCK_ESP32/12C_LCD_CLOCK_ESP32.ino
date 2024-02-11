#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid       = "Your wifi name";
const char* password   = "your password";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 10800;

const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  
LiquidCrystal_I2C lcd(0x27, 16, 2);
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(&timeinfo,"%B %d %Y");
  lcd.setCursor(0,1);
  lcd.print(&timeinfo,"%a %H:%M:%S");
}


void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);


  sntp_set_time_sync_notification_cb( timeavailable );

  lcd.init();
  lcd.backlight();
 
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);


  Serial.printf("Connecting to %s ", ssid);
  lcd.clear();
 lcd.setCursor(0, 0);
lcd.print("Connecting to ");
 lcd.setCursor(0, 1);
lcd.print(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED ");
  lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("CONNECTED");
  

}

void loop()
{
  delay(1000);
  printLocalTime(); 
}
