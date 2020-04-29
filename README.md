#### Αυτοματισμός έγκαιρης ενημέρωσης πυρκαιάς (IoT Fire Protection)

Ο αυτοματισμός έγκαιρης ενημέρωσης πυρκαιάς είναι μία κατασκευή που βασίζεται στο Arduino και έχει σκοπό την ανίχνευση μίας πυρκαιάς σε αρχικό στάδιο και την έγκαιρη ειδοποίηση του ενδιαφερόμενου. 


**Πίνακας περιεχομένων**

[TOCM]

[TOC]

###Συντομογραφίες
- **IoTFP**:  αυτοματισμός έγκαιρης ενημέρωσης πυρκαιάς (IoT Fire Protection)
- **IoTServer**: Η εφαρμογή που αναλαμβάνει να συγκεντρώνει τις μετρήσεις από όλους τους IoTFP και που αποστέλει ειδοποιήσεις όποτε χρειάζεται.

###Υλικό
Ο IoTFP αποτελείται από ένα **ESP32** στο οποίο είναι συνδεδεμένοι ένας **αισθητήρας μονοξειδίου του άνθρακα**  ένας **αισθητήρας θερμοκρασίας** και ένα δυνατό **buzzer**

###Αρχές λειτουργείας
Ο IoTFP μπορεί να χρησιμοποηθεί με δύο τρόπους: είτε αυτόνομα σε κάποιο χώρο, είτε συνδεδεμένος στο Internet.
####Αυτόνομη χρήση
Στην αυτόνομη χρήση μόλις ανιχνευθεί μονοξείδιο του άνθρακα πάνω από 10ppm ή θερμοκρασία πάνω από 45°C θα ενεργοποιείται το buzzer ώστε σε περίπτωση που κάποιος βρίσκεται κοντά να το ακούσει και να ελέγξει τι συμβαίνει.
####Χρήση στο Internet (Cloud)
Για τη χρηση στο Internet λαμβάνει μέρος και ένα ακόμη σύστημα που συγκεντώνει τα δεδομένα από τα IoTFP.
Στη χρήση με σύνδεση στο Internet, θα ισχύει ότι και στην αυτόνομη χρήση και επιπλέον:
- Κάθε 60 δευτερόλεπτα ο IoTFP αποστέλει στον IoTServer τις τιμές του μονοξειδίου του άνθρακα και την θερμοκρασία.
	- Σε περίπτωση που ληφθεί τιμή πάνω από ένα προκαθορισμένο όριο ή
	- Σε περίπτωση που δεν ληφθεί τιμή για πάνω από 60 δευτερόλεπτα
Ενημερώνεται ο ενδιαφερόμενος μέ ειδοποήσηση στο κινητό του τηλέφωνο. Για να συμβεί αυτό θα πρέπει να έχει εγκατεστημένη και την εφαρμογή στο κινητό του.
	- Επίσης σε περίπτωση που ληφθεί τιμή μόνο για ένα ή κανένα από τους δύο αισθητήρες πάλι ενημερώνεται ο ενδιαφερόμενος.
