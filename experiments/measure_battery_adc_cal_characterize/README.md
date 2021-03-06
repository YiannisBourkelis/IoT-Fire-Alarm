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
[Analog to Digital Converter](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html "Analog to Digital Converter")

# Επίσημη υλοποίηση βιβλιοθήκης
[esp_adc_cal_raw_to_voltage]( https://github.com/espressif/esp-idf/blob/master/examples/peripherals/adc/adc/main/adc1_example_main.c "esp_adc_cal_raw_to_voltage")

# Εξωτερική τεκμηρίωση

# Αποτελέσματα πειράματος
Η μέτρηση με αυτό τον τρόπο έγινε με επιτυχία. Στον πίνακα είναι οι μετρήσεις που πήραμε.
|   | Vin 3,60v  | Vin 3,00v  | Vin 2,80v  | Vin 2,60v   |
| ------------ | ------------ | ------------ | ------------ | ------------ |
| Tester 0,10v |0,12   |0,11   |0,11   |0,11   |
| Tester 0,30v |0,32   |0,31   |0,31   |0,31   |
| Tester 0,50v |0,52   |0,51   |0,51   |0,51   |
| Tester 0,90v  |0,91   |0,91   |0,91   |0,91   |
| Tester 1,00v  |1,01   |1,00   |1,01   |1,01   |
| Tester 1,20v  |1,21   |1,22   |1,21   |1,21   |
| Tester 1,35v  |1,37   |1,37   |1,37   |1,37   |
