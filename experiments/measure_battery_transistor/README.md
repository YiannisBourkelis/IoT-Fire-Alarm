#### Πείραμα μέτρησης μπαταρίας με χρήση transistor - 26-04-2021
Σε αυτό το πείραμα μετράμε τα milivolt της μπαταρίας, ενεργοποιώντας το κύκλωμα του διαιρέτη με την βοήθεια ενός transistor.

#### Περιβάλον εκτέλεσης
Μπαταρία: ANR26650M1-B χωρητικότητας 2500mAh
Κώδικας όπως είναι στις 26-4-2021

#### Αποτελέσματα
Ξεκινήσαμε το πείραμα στις 26-04-2021 στις 03:19 και λάβαμε την τελευταία μέτρηση στις 29-04-2021 και ώρα 12:30

Συνολική διάρκεια: 3 ημέρες, 9 ώρες και 11 λεπτά

#### Γραφική παράσταση μπαταρίας
[![](https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_transistor/transistor-experiment-graph.png)](https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_transistor/transistor-experiment-graph.png)

#### Συμπεράσματα
1. Για να γίνεται μέτρηση της τάσης με ακρίβεια θα πρέπει να είναι απενεργοποιημένο το WiFi
2. Με χρήση WiFi η συσκευή σταματάει να στέλνει δεδομένα όταν η μπαταρία πέσει κάτω από τα 3.02v. Το ίδιο είδαμε και στο προηγούμενο πείραμα.
3. Για οικονομία στην κατανάλωση το WiFi θα πρέπει να ενεργοποιείται ακριβώς πριν γίνει η αποστολή του json και να απενεργοποιείται αμέσως μετά.
4. Για διαγνωστικούς λόγους, μπορεί να αποστέλεται και το boot_count, ο αριθμός δηλαδή του πλήθουε των wake απο sleep καθώς και το wakeup_reason που μας δίχνει τον λόγο που ξεκίνησε το esp32 (απο deep sleep, reset κλπ). Ετσι μπορούμε να βλέπουμε αν κάτι δεν πάει καλά κατα το ξεκίνημα της συσκευής.

#### Δεδομένα
Μέσος όρος μετρήσεων ανά ώρα: [https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_transistor/avg_battery_voltage_transistor.json](https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_transistor/avg_battery_voltage_transistor.json "https://github.com/YiannisBourkelis/IoT-Fire-Alarm/blob/master/experiments/measure_battery_transistor/avg_battery_voltage_transistor.json")
