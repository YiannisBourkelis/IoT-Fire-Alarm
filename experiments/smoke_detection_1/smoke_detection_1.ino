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
* 1. Oi metriseis eginan me antistasi pou se iremia to ADC epestrefe 0.
* 2. Eleksame tin evesthisia anixnefsis kapnou me diafores taseis
* 3. Eleksame an metavaletai i katastasi iremias otan dioxetevoume zesto aera
* 4. Eleksame an to epirreazei to fws
* 
* Symperasmata
* ===============
* 1. theloyme se iremia (xwris parousia kapnou) na lamvanoume mia statheri metrisi p.x. gyrw sto 100
* 2. leitourgei ok
* 3. xreiazetai na vroume mia antistasi opou se iremia na lamvanei p.x. 100. Etsi tha mporesoume na katalavenoume kai:
*   3.1 An yparxei kapoia vlavi ston aisthitira (p.x. lipsi metrisis ektos evrous iremias p.x. 100 +/- 10
*   3.2 An i tasi leitourgias 3.1 - 3.6 epireazei tis metriseis 
* 4. me parousia kapnou (i atmou) lamvaname amesws metriseis panw apo to 0 mexri kai panw apo 3000
* 5. Dioxetefsame zesto aera kai paratirisame oti den epirreazontousan oi metriseis. Thelei kai alles dokimes otan vroume kaliteri antistasi alla kai kathws yparxei kapnos
* 6. Dokimasame diafores taseis apo 3.1 ews 3.6 kai den paratirisame metavoli stin katastasi iremias. Thelei kai allo elegxo me alli antistasi kai kathws ginetai metrisi kapnou
* 7. Me anoixto to kapaki tou smoke detection chamber epireazontai oi metriseis otan p.x. anavoume ton anaptira konta. Me lefko fws epirreazetai ligo.
*   Symperasma sxetika me smoke detection chamber: Tha prepei na einai kataskevasmeno wste na epitrepei na pernaei kai na eglovizetai o kapnos me afkolia alla OXI to fws.
*/


//the board should support an ADC resolution of 12bits
//TODO: check if there is a constant to get the ADC resolution of the board at compile time
//static const unsigned int ADC_RESOLUTION = 4096;

#define ADC_input_infrared_sensor 32
const int act_sensor = 27;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(act_sensor, OUTPUT);
}

//TODO version 2.
float AnalogReadNormalized(int adc_pin, unsigned int samples, int sample_delay_ms)
{
  unsigned long measurements_total = 0;
  int min_measurement = INT_MAX;
  int max_measurement = 0;

  unsigned int samples_taken_counter = 0;
  
  while (true) {
    int measurement = analogRead(adc_pin);
    min_measurement = min(min_measurement, measurement);
    max_measurement = max(max_measurement, measurement);
    measurements_total += measurement;
    samples_taken_counter++;
    if (samples_taken_counter < samples){
      delay(sample_delay_ms);
    } 
    else {
      break;
    }
  }

  /*
   * if there are 4 or more samples, remove minimum and maximum measurement
   * from the measurements_total to improve ta accurancy
   */
  if (samples > 3){
    measurements_total -= (min_measurement + max_measurement);
    samples -= 2;
  }

  return (float)measurements_total / (float)samples;
}


float smooth(int adc_pin, int repetitions, int delay_ms)
{
  int total_measurements = 0;

  //TODO: na prosthesw logiki ekseresis mikroteris kai megaliteris metrisis otan repetitions > 3 xrisimopoiwntas min/max functions
  for (int i = 0; i < repetitions; i++) {
    //Serial.println(analogRead(adc_pin));
    total_measurements += analogRead(adc_pin);
    delay(delay_ms); //TODO na to valw prin apo to total_measurements += analogRead(adc_pin);
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
  Serial.println("Activate Sircuit");
  digitalWrite(act_sensor, HIGH);
   delay(1000);
   
  //lipsi normilized metrisis
  float result = analogRead(ADC_input_infrared_sensor); //smooth(ADC_input_infrared_sensor, 10, 50); 
  Serial.println(result); 

  float result_normalized = AnalogReadNormalized(ADC_input_infrared_sensor, 10, 10);
  Serial.print("HIGH result_normalized: ");
  Serial.println(result_normalized);
  Serial.println("Deactivate Sircuit");
  digitalWrite(act_sensor, LOW);

  delay(2000);
  
  result_normalized = AnalogReadNormalized(ADC_input_infrared_sensor, 10, 10);
  Serial.print("LOW result_normalized: ");
  Serial.println(result_normalized);
  Serial.println("\n\n\n");

  delay(2000);
}
