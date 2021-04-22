#### Πείραμα εκφόρτησης μπαταρίας - 22-04-2021
Με το πείραμα αυτό θέλουμε να δούμε τον τρόπο εκφόρτησης της μπαταριας και να υπολογίσουμε τον μέσο όρο κατανάλωσης του ESP32 όταν αποστέλει ανα 3 δευτερόλεπτα με ανοιχτό το WIFI δεδομένα.

#### Περιβάλον εκτέλεσης
Μπαταρία: ANR26650M1-B χωρητικότητας 2500mAh
Κώδικας όπως είναι στις 20-4-2021

#### Αποτελέσματα
Ξεκινήσαμε το πείραμα στις 20-04-2021 στις 15:54 και λάβαμε την τελευταία μέτρηση στις 22-04-2021 και ώρα 07:04

Συνολική διάρκεια: 1 ημέρα, 15 ώρες και 10 λεπτά, δηλαδή **περίπου 39 ώρες.**

#### Γραφική παράσταση μπαταρίας
![](https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_adc_cal_characterize_multi_pin/experiment_22-4-2021-battery-chart.png)

#### Συμπεράσματα
1. Με βαση τα παραπάνω η κατανάλωση του ESP32 με το WIFI ανοιχτό και συνεχή αποστολή δεδομένων ανά 3 δευτερόλεπτα είναι:
2500mAh (χωρητικότητα μπαταρίας) / 39 ώρες = 64,20mA (max)
2. Το ESP32 απενεργοποιήθηκε περίπου στα 3v

#### Δεδομένα
Μέσος όρος μετρήσεων ανά ώρα: [https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_adc_cal_characterize_multi_pin/avg_battery_voltage_experiment_22-4-2021.json](https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_adc_cal_characterize_multi_pin/avg_battery_voltage_experiment_22-4-2021.json)
