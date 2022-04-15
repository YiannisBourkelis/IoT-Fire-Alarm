#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "Arduino.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate 

class VoltageCalculator
{
  public:
    VoltageCalculator();
    void Setup();
    uint32_t GetVoltage();

    esp_adc_cal_characteristics_t *adc_chars;
};
