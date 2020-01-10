#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WifiDetails.h>

// const char *ssid = ENV_SSID;
// const char *pass = ENV_PASS;

const long utcOffsetInSeconds = 0; // afaik UTC is GMT

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  Serial.begin(115200);
  delay(3000);
  Serial.println("Okay");
  Serial.println(SSID);
  // Serial.println(ssid);
  // WiFi.begin(ssid, pass);
  // while (WiFi.status() != WL_CONNECTED)  {
  //   delay(500);
  //   Serial.print(".");
  // }
  // timeClient.begin();
}

void loop()
{
  // timeClient.update();
  // Serial.print(daysOfTheWeek[timeClient.getDay()]);
  // Serial.print(", ");
  // Serial.print(timeClient.getHours());
  // Serial.print(":");
  // Serial.print(timeClient.getMinutes());
  // Serial.print(":");
  // Serial.println(timeClient.getSeconds());
  // //Serial.println(timeClient.getFormattedTime());
  // delay(1000);
}