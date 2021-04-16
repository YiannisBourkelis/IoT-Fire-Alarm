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

//https://github.com/espressif/esp-idf/blob/cf457d412a7748139b77c94e7debe72ee86199af/examples/peripherals/adc/single_read/adc/main/adc1_example_main.c

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
  adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);

  adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));

  //Characterize ADC at particular atten
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

  print_char_val_type(val_type);
}

void print_char_val_type(esp_adc_cal_value_t _val_type)
{
    if (_val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (_val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}


void check_efuse(void)
{
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}

// the loop routine runs over and over again forever:
void loop() {

  //Check if Two Point or Vref are burned into eFuse
  check_efuse();
    
  // read the input on analog pin 0:

  int NO_OF_SAMPLES = 64;
  uint32_t adc_reading = 0;
  //Multisampling
  for (int i = 0; i < NO_OF_SAMPLES; i++) {
    adc_reading += adc1_get_raw(ADC1_CHANNEL_4);
  }
  adc_reading /= NO_OF_SAMPLES;
 //Convert adc_reading to voltage in mV
 uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
  //uint32_t reading =  adc1_get_raw(ADC1_CHANNEL_4);
  //uint32_t voltage =  esp_adc_cal_raw_to_voltage(reading, adc_chars);
  

  Serial.print("Reading:");
  Serial.println(adc_reading);
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
