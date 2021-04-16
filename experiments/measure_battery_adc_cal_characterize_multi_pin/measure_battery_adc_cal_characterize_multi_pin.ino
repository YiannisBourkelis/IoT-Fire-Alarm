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

ESP32AnalogRead adc;

const char* ssid = "steth";
const char* password =  "ilovecomputers";
  
// ADC1_CHANNEL_0     ADC1 channel 0 is GPIO36
// ADC1_CHANNEL_1     ADC1 channel 1 is GPIO37
// ADC1_CHANNEL_2     ADC1 channel 2 is GPIO38
// ADC1_CHANNEL_3     ADC1 channel 3 is GPIO39
// ADC1_CHANNEL_4     ADC1 channel 4 is GPIO32
// ADC1_CHANNEL_5     ADC1 channel 5 is GPIO33 ok
// ADC1_CHANNEL_6     ADC1 channel 6 is GPIO34
// ADC1_CHANNEL_7     ADC1 channel 7 is GPIO35

void setup() {
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



float voltage_divider_read()
{
    float sum = 0;               // sum of samples taken
    float voltage = 0;           // calculated voltage

    for (int i = 0; i < 500; i++)
    {
        sum += adc.readVoltage();
        delayMicroseconds(1000);
    }
    // calculate the voltage
    voltage = sum / 500;
    Serial.print("Voltage: ");
    Serial.println(voltage);
    Serial.println(adc.readVoltage());
    return voltage;
}

float to_battery_volts(float voltage_divider_volts){
  const float voltage_calibration = 0.197; //lowering this value increase return value
  return voltage_divider_volts / voltage_calibration;
}


void loop() {
  //battery_read();
  Serial.println("Loop Start");
  delay(500);

  HTTPClient https;
  https.begin("https://iot.filoxeni.com/api/user/device/measurement");

  String uptime = uptime_formatter::getUptime();
  Serial.println(uptime);

  String battery_voltage = (String)to_battery_volts(voltage_divider_read());

  https.addHeader("Content-Type", "application/json"); //Specify content-type header
  https.addHeader("Host", "iot.filoxeni.com"); //Specify content-type header
  https.addHeader("Authorization", "Bearer DCRkyCeUXE1u44z6rBLnWJ6STjRSzpgiRbTYYOa9"); //Specify content-type header
  String json;
  json += "{";
  json += "\"team_id\":\""           + ((String)2)                   + "\"";
  json += ",\"photoresistor\":\"" + ((String)10)    + "\"";
  json += ",\"battery_voltage\":\"" + battery_voltage    + "\"";
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
}
