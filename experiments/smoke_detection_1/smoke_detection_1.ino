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
* Dokimi
* ===============
* 1. theloyme se iremia (xwris parousia kapnou) na lamvanoume mia statheri metrisi p.x. gyrw sto 100
* 2. sto peirama pou kaname eixame ipsili antistasi opote se iremia lamvaname panta 0
* 3. me parousia kapnou (i atmou) lamvaname amesws metriseis panw apo to 0 mexri kai panw apo 3000
* 
* Symperasmata
* ===============
* 1. leitourgei ok
* 2. xreiazetai na vroume mia antistasi opou se iremia na lamvanei p.x. 100. Etsi tha mporesoume na katalavenoume kai:
*   2.1 An yparxei kapoia vlavi ston aisthitira (p.x. lipsi metrisis ektos evrous iremias p.x. 100 +/- 10
*   2.2 An i tasi leitourgias 3.1 - 3.6 epireazei tis metriseis 
* 
*/

#define ADC_input_infrared_sensor 32

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}


float smooth(int adc_pin, int repetitions, int delay_ms)
{
  int total_measurements = 0;
  
  for (int i = 0; i < repetitions; i++) {
    //Serial.println(analogRead(adc_pin));
    total_measurements += analogRead(adc_pin);
    delay(delay_ms);
  }

  return total_measurements / repetitions;
}


// the loop routine runs over and over again forever:
void loop() {
/*
  int result = analogRead(ADC_input_infrared_sensor);
  Serial.println(result);
  delay(200);
  */

  //lipsi normilized metrisis
  float result = smooth(ADC_input_infrared_sensor, 10, 50);
  Serial.println(result); 
}
