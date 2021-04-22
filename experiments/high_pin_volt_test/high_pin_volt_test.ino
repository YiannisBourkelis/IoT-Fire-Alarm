/* ************************************************************************
 * Environmental Monitoring Station
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

/*
* Experiment details
* =====================
* 
*/
#include "Arduino.h"

#define ESP_32_PIN 25

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);  

  // initialize digital pin LED_BUILTIN as an output.
  delay(2000); 
  pinMode(ESP_32_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(ESP_32_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGH");
  delay(3000);                       // wait for a second
  digitalWrite(ESP_32_PIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("LOW");
  delay(3000);                       // wait for a second
}
