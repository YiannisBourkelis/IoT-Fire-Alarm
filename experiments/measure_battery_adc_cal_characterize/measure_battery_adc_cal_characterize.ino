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
* In this experiment we are trying to measure the battery voltage of the battery
* that powers the esp32
* 
*/

#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate

static esp_adc_cal_characteristics_t *adc_chars;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  delay(200);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_6);

  adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));

  //Characterize ADC at particular atten
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:

  uint32_t reading =  adc1_get_raw(ADC1_CHANNEL_4);
  uint32_t voltage =  esp_adc_cal_raw_to_voltage(reading, adc_chars);

  Serial.print("Reading:");
  Serial.println(reading);
  Serial.print("Voltage:");
  Serial.println(voltage);
  Serial.println("");
  

  //int measurements=0;
  //for (int i=0; i<100; i++){
  //  int val = adc1_get_raw(ADC1_CHANNEL_4);
  //}
  //int val = adc1_get_raw(ADC1_CHANNEL_4);
 // Serial.print("Raw ADC:");
  //Serial.println(val);

  //Serial.print("Volt:");
  //float volts = (val * 1.35f) / 4095;
  //Serial.println(volts);
  
  delay(500);        // delay in between reads for stability
}
