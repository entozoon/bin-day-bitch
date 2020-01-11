#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WifiDetails.h>

const long utcOffsetInSeconds = 0; // AFAIK UTC = GMT

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);

//   _______
//  /       \
// /         \
// |         |
// \---------/
//   | | | |
//   | | | |
//     | | |
//     | |
//     |
//   R + G B
//

const int redPin = D1;
const int greenPin = D2;
const int bluePin = D3;

const char *wl_status_to_string(wl_status_t status)
{
  switch (status)
  {
  case WL_NO_SHIELD:
    return "WL_NO_SHIELD";
  case WL_IDLE_STATUS:
    return "WL_IDLE_STATUS";
  case WL_NO_SSID_AVAIL:
    return "WL_NO_SSID_AVAIL";
  case WL_SCAN_COMPLETED:
    return "WL_SCAN_COMPLETED";
  case WL_CONNECTED:
    return "WL_CONNECTED";
  case WL_CONNECT_FAILED:
    return "WL_CONNECT_FAILED";
  case WL_CONNECTION_LOST:
    return "WL_CONNECTION_LOST";
  case WL_DISCONNECTED:
    return "WL_DISCONNECTED";
  }
}
const int yearFromEpoch(int epoch)
{
  time_t rawtime = epoch;
  struct tm *ti;
  ti = localtime(&rawtime);
  int year = ti->tm_year + 1900;

  return year;
}
const int daysSince6Jan2020(int epoch)
{
  // Tuesday morning, 7th jan was definitely a bottle bin day (so put out notification day before)
  // And I preeeetty much  reckon that it's alternate weeks thereafter, give or take bank holidays and junk
  int secondsSinceMorning6Jan2020 = epoch - 1578268800; // 0:00am 6th jan
  int days = floor(secondsSinceMorning6Jan2020 / 60 / 60 / 24);
  return days;
}
const int weeksSince6Jan2020(int epoch)
{
  return floor(daysSince6Jan2020(epoch) / 7);
}
char *whichBinWeek(int weeks)
{
  if (weeks % 2 == 0)
  {
    return "bottle";
  }
  else
  {
    return "black";
  };
}
void ledOff()
{
  analogWrite(redPin, 1023);
  analogWrite(greenPin, 1023);
  analogWrite(bluePin, 1023);
}
void indicateBinWeek(char *week)
{
  if (week == "bottle")
  {
    // Bergundy  .. bottles, paper and also green bin (recycling in general)
    analogWrite(redPin, 400);
    analogWrite(greenPin, 1000);
    analogWrite(bluePin, 800);
  }
  else if (week == "black")
  {
    // white, I guess?
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  // LED test (red, green, blue)
  analogWrite(redPin, 0);
  analogWrite(greenPin, 1023);
  analogWrite(bluePin, 1023);
  delay(500);
  analogWrite(redPin, 1023);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 1023);
  delay(500);
  analogWrite(redPin, 1023);
  analogWrite(greenPin, 1023);
  analogWrite(bluePin, 0);
  delay(500);
  ledOff();
  delay(1500);
  Serial.println("\nConnecting to WIFI");
  Serial.println(SSID);
  Serial.println(PASS);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
    Serial.println(wl_status_to_string(WiFi.status()));
  }
  timeClient.begin();
}
void loop()
{
  timeClient.update();
  char *binWeek = whichBinWeek(weeksSince6Jan2020(timeClient.getEpochTime()));
  // Serial.print(" Day: ");
  // Serial.print(timeClient.getDay());
  // Serial.print(" Hours: ");
  // Serial.print(timeClient.getHours());
  // Serial.print(" Mins: ");
  // Serial.print(timeClient.getMinutes());
  Serial.print("( daysSince6Jan2020: ");
  Serial.print(daysSince6Jan2020(timeClient.getEpochTime()));
  Serial.print("   weeksSince6Jan2020: ");
  Serial.print(weeksSince6Jan2020(timeClient.getEpochTime()));
  Serial.print(" )");

  // Mondays are bin night
  if (timeClient.getDay() == 1)
  {
    Serial.println("\nBin night! For bin: ");
    indicateBinWeek(binWeek);
  }
  else
  {
    Serial.print("\nNot bin night, but next will be: ");
    ledOff();
  }
  Serial.println(binWeek);

  delay(3600000); // hourly check
}