#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid       = "FiberHGW_2.4GHz";
const char* password   = "kggyyPFzsN";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 10800;
const int   daylightOffset_sec = 10800;

const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)
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

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );

  lcd.init();
  lcd.backlight();
 

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  sntp_servermode_dhcp(1);    // (optional)

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagicaly.
   */
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
   * A more convenient approach to handle TimeZones with daylightOffset 
   * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  //configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
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
  printLocalTime();     // it will take some time to sync time :)
}
