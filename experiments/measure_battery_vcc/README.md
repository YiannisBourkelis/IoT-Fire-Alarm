# Μέτρηση Volt της μπαταρίας τροφοδοσίας
Σε αυτό το πείραμα θα προσπαθήσουμε να βρούμε ένα ακριβή τρόπο μέτρησης της μπαταρίας LiFePO4 με την οποία τροφοδοτείται το ESP32

# Χρησιμότητα
Πρέπει να ξέρουμε τα volt της μπαταρίας για να μπορούμε να απεικονίζουμε το ποσοστό φόρτισης της μπαταρίας, ώστε να γνωρίζουμε έγκαιρα αν το device μας χρειάζει φόρτιση

# Το πρόβλημα
Το naked ESP32 δεν έχει voltage regulator που να δίνει μία σταθερή τάση τροφοδοσίας 3.3v ώστε να μπορούμε να χρησιμοποιήσουμε την μέθοδο ανάγνωσης της τάσης χρησιμοποιώντας τα volt εισόδου ως μίας σταθερά.

      float VBat = float(analogRead(ADC_input_battery_pin)) / 4096.0f * 3.3;  
Ο παραπάνω κώδικας δεν μπορεί να δουλέψει στην περίπτωσή μας.

Πρέπει οπότε να βρούμε ένα άλλο τρόπο μέτρησης που να μήν εξαρτάται από την σταθερά 3.3v

# Σχετική επίσημη τεκμηρίωση
- [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html "https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html")

# Δοκιμή 1
esp_adc_cal_get_voltage

# Δοκιμή 2
    adc1_config_width(ADC_WIDTH_BIT_11);
    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    float battery_read()
    {
        //read battery voltage per %
        long sum = 0;                  // sum of samples taken
        float voltage = 0.0;           // calculated voltage
        float output = 0.0;            //output value
        const float battery_max = 4.2; //maximum voltage of battery
        const float battery_min = 3.0; //minimum voltage of battery before shutdown
    
        float R1 = 100000.0; // resistance of R1 (100K)
        float R2 = 27000.0;  // resistance of R2 (10K)
    
        for (int i = 0; i < 100; i++)
        {
            sum += adc1_get_voltage(ADC1_CHANNEL_0);
            delayMicroseconds(1000);
        }
        // calculate the voltage*/
        
        voltage = sum / (float)100;
        Serial.println(voltage);
        voltage = (voltage *1.1) / 2047.0; //for internal 1.1v reference
        // use if added divider circuit
        voltage = voltage / (R2/(R1+R2));
    }
[https://github.com/espressif/arduino-esp32/issues/2957](https://github.com/espressif/arduino-esp32/issues/2957 "https://github.com/espressif/arduino-esp32/issues/2957")
