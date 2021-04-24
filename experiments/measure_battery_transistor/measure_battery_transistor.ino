/* ************************************************************************
 * IoT Protect
 * (C) 2021 by Yiannis Bourkelis (https://github.com/YiannisBourkelis/)
 * (C) 2021 by Yiannis Grigoriadis
 * (C) 2021 by Kostas Laftsis
 *
 * This file is part of IoT Protect
 *
 * IoT Protect is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IoT Protect is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IoT Protect.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#include "Arduino.h"
#include "ESP32AnalogRead.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "uptime_formatter.h"
#include "secrets.h"

ESP32AnalogRead adc;
#define TRANSISTOR_PIN 25

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 60 /* Time ESP32 will go to sleep (in seconds) */

const char* ssid = SECRET_WIFI_SSID;
const char* password =  SECRET_WIFI_PASS;
  
// ADC1_CHANNEL_0     ADC1 channel 0 is GPIO36
// ADC1_CHANNEL_1     ADC1 channel 1 is GPIO37
// ADC1_CHANNEL_2     ADC1 channel 2 is GPIO38
// ADC1_CHANNEL_3     ADC1 channel 3 is GPIO39
// ADC1_CHANNEL_4     ADC1 channel 4 is GPIO32
// ADC1_CHANNEL_5     ADC1 channel 5 is GPIO33 ok
// ADC1_CHANNEL_6     ADC1 channel 6 is GPIO34
// ADC1_CHANNEL_7     ADC1 channel 7 is GPIO35

void setup() {
  Serial.println("Entering SETUP");
  
  //gpio 25 pin gia na oplizei to transistor metrisis tis batarias
  pinMode(TRANSISTOR_PIN, OUTPUT);
  
  adc.attach(33);
  Serial.begin(115200);
  //edo epilegeis to gpio exo valei to GPIO33
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  
}



uint32_t voltage_divider_read()
{
    uint32_t sum = 0;               // sum of samples taken
    uint32_t voltage_divider_milivolts = 0; // calculated voltage
    const int samples = 32;

    for (int i = 0; i < samples; i++)
    {
        sum += adc.readMiliVolts();
        delayMicroseconds(1000);
    }
    Serial.print("Sum milivolts: ");
    Serial.println(sum);
    
    // calculate the voltage
    voltage_divider_milivolts = sum / samples;
    Serial.print("Voltage Divider milivolts: ");
    Serial.println(voltage_divider_milivolts);
    Serial.println(adc.readMiliVolts());
    return voltage_divider_milivolts;
}

uint32_t to_battery_milivolts(uint32_t voltage_divider_milivolts){
  const float voltage_calibration = 6.04; //lowering this value increase return value
  return (float)voltage_divider_milivolts * voltage_calibration;
}


void loop() {
  Serial.println("Loop Start");

  Serial.println("Open transistor");
  digitalWrite(TRANSISTOR_PIN, HIGH); 
  //delay(3000);

  HTTPClient https;
  https.begin("https://iot.filoxeni.com/api/user/device/measurement");

  String uptime = uptime_formatter::getUptime();
  Serial.println(uptime);

  uint32_t battery_milivolts = to_battery_milivolts(voltage_divider_read());
  String str_battery_milivolts = (String)battery_milivolts;
  Serial.print("battery_voltage converted as string: ");
  Serial.println(str_battery_milivolts);

  //close transistor
  Serial.println("Close transistor");
  digitalWrite(TRANSISTOR_PIN, LOW); 

  https.addHeader("Content-Type", "application/json"); //Specify content-type header
  https.addHeader("Host", "iot.filoxeni.com"); //Specify content-type header
  https.addHeader("Authorization", "Bearer DCRkyCeUXE1u44z6rBLnWJ6STjRSzpgiRbTYYOa9"); //Specify content-type header
  String json;
  json += "{";
  json += "\"team_id\":\""           + ((String)2)                   + "\"";
  json += ",\"photoresistor\":\"" + ((String)10)    + "\"";
  json += ",\"battery_voltage\":\"" + str_battery_milivolts    + "\"";
  json += ",\"uptime\":\"" + uptime    + "\"";
  json += "}";
   
  int httpResponseCode = https.POST(json); //Send the actual POST request

  if(httpResponseCode > 0){
    Serial.print("POST (filoxeni): ");
    Serial.println(httpResponseCode);
    } else {
    Serial.print("Error on sending POST (filoxeni): ");
    Serial.println(httpResponseCode);
  }

 Serial.println("Calling esp_deep_sleep_start...");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();

}
