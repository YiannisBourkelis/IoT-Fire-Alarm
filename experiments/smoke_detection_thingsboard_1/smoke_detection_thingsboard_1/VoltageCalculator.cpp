#include "VoltageCalculator.h"

//static esp_adc_cal_characteristics_t* VoltageCalculator::adc_chars;

VoltageCalculator::VoltageCalculator()                                  
{
}

void VoltageCalculator::Setup() 
{
  delay(200);

  adc1_config_width(ADC_WIDTH_BIT_12);
  //ADC1_CHANNEL_4 ADC1 channel 4 is GPIO32
  //ADC1_CHANNEL_5 ADC1 channel 5 is GPIO33
  adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);

  adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));

  //Characterize ADC at particular atten
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

  //print_char_val_type(val_type);
}

uint32_t VoltageCalculator::GetVoltage()
{
  //check_efuse();
    
  // read the input on analog pin 0:

  int NO_OF_SAMPLES = 64;
  uint32_t adc_reading = 0;
  //Multisampling
  for (int i = 0; i < NO_OF_SAMPLES; i++) {
    //ADC1_CHANNEL_5 ADC1 channel 5 is GPIO33
    adc_reading += adc1_get_raw(ADC1_CHANNEL_5);
  }
  adc_reading /= NO_OF_SAMPLES;
 //Convert adc_reading to voltage in mV
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
  return voltage;
}
