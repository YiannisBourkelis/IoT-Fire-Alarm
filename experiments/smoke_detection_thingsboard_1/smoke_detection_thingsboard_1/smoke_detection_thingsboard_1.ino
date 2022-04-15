//#include "Arduino.h"
//#include "ESP32AnalogRead.h"
#include "WiFi.h"
#include <HTTPClient.h>
//#include "uptime_formatter.h"
//#include "secrets.h"
#include <esp_wifi.h> //for esp_wifi_stop()
#include "VoltageCalculator.h"


VoltageCalculator voltage_calc;

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 10 /* Time ESP32 will go to sleep (in seconds) */

const int SMOKE_THRESHOLD = 300;

esp_sleep_wakeup_cause_t wakeup_reason; //o logos pou egine wakeup apo deep sleep to esp32
RTC_DATA_ATTR int bootCount = 0; //metrisis tou plithous deep sleep - awake

const char* ssid = "Hot";
const char* password =  "paswworddd";

#define ADC_input_infrared_sensor 32 // diavazoume ton sensora
const int act_sensor = 27; //dinei revma ston sensora
const int IO_BEEPER = 14; //dinei revma ston sensora


bool connect_to_wifi()
{
  delay(500);
  Serial.println("Attempt WiFi connection...");
  //syndesi sto wifi
  //WiFi.mode(WIFI_STA);
  Serial.println("CALL WiFi.begin...");
  WiFi.begin(ssid, password);
  Serial.println("CHECK WiFi.status()...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }

  bool connected_to_wifi = (WiFi.status() == WL_CONNECTED);
  if(connected_to_wifi){
    Serial.println("Connected to the WiFi network");
  } else {
    Serial.println("Could not connect to the WiFi network");
  }

  return connected_to_wifi;
}

void print_wakeup_reason(){
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
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

void setup() {
  delay(500);
  Serial.begin(115200);
  pinMode(act_sensor, OUTPUT);
  pinMode(IO_BEEPER, OUTPUT);
  Serial.println("Entering SETUP");
  print_wakeup_reason();

  
  Serial.println("Set esp_sleep_enable_timer_wakeup to: " + (String)TIME_TO_SLEEP + " seconds");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  voltage_calc.Setup();
}

void send_data_to_iot_server(int smoke_value, uint32_t real_voltage_value)
{
  HTTPClient http;
  //http.begin("http://iot.techthrace.com:8080/api/v1/wGNzhlUkS6EFpW41FcuZ/telemetry");
  //http.begin("http://iot.techthrace.com:8080/api/v1/hVDr5uCDchaFcnojdMzH/telemetry");

  String telemetryPort   = "443";
  String telemetry_url   = "iot.steth.gr";
  String telemetry_token = "FMI0a5Yh6YhqKRIiHFXn";

  
  
  String url = "https://" + telemetry_url + ":" + telemetryPort + "/api/v1/" + telemetry_token + "/telemetry";
  Serial.println(url);
  http.begin(url);
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  http.addHeader("Host", telemetry_url); //Specify content-type header


  String json;
  json += "{";
  json += "\"smoke_value\":\""                   + ((String)smoke_value)                   + "\"";
  json += ",\"real_voltage_value\":\""           + ((String)real_voltage_value)                   + "\"";
  json += "}";

 

  int httpResponseCode = http.POST(json); //Send the actual POST request

  if(httpResponseCode > 0){
    String response = http.getString();  //Get the response to the request
    Serial.println("HTTP POST response code: " + (String)httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST (thingsboard): ");
    Serial.println(httpResponseCode);
  }

  //If the http post response is not 200
  //then report the error to the watchdog
  if(httpResponseCode == 200){
    //FailureWatchdog::reportSuccess();
  } else {
    //FailureWatchdog::reportError();
  }
  
  http.end();  //Free resources
}

float take_smoke_measurement()
{
  /*
   * lipsi metrisis gia anixnefsi kapnou.
   * Thetei to pin high gia na trofodotisei to led kai ton aisthitira yperithris
   * perimenei 1sec gia na trofodotithei swsta kai sti synexeia lamvanei tin metrisi.
   * Telos, thetei to pin low gia na min katanalwnei energeia.
   */
  digitalWrite(act_sensor, HIGH);
  delay(1000);
  float result_normalized = AnalogReadNormalized(ADC_input_infrared_sensor, 10, 10);
  Serial.print("HIGH result_normalized: ");
  Serial.println(result_normalized);
  Serial.println("Deactivate Sircuit");
  digitalWrite(act_sensor, LOW); 

  return result_normalized;
}


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop Start");
  Serial.println("Activate Sircuit");

  //take measurement or ULP
  float result_normalized = take_smoke_measurement();

  auto real_voltage_value = voltage_calc.GetVoltage();
  Serial.print("real voltage:");
  Serial.print(real_voltage_value);
  Serial.println(" mV");

  //smoke detected?
  if (result_normalized > SMOKE_THRESHOLD) {
    Serial.println(">>>> SMOKE DETECTED");
    //yparxei kapnos
    digitalWrite(IO_BEEPER, HIGH);
    
    //syndesi sto wifi kai apostoli dedomenwn
    bool connected_to_wifi = connect_to_wifi();
    if (connected_to_wifi){
      Serial.println("will send_data_to_iot_server");
      send_data_to_iot_server(result_normalized, real_voltage_value);
      WiFi.disconnect();
    }

    delay(20000);
  }
  else {
    //den yparxei kapnos
    
    //deactivate beeper
    digitalWrite(IO_BEEPER, LOW);

    //syndesi sto wifi kai apostoli dedomenwn
    bool connected_to_wifi = connect_to_wifi();
    if (connected_to_wifi){
      Serial.println("will send_data_to_iot_server");
      send_data_to_iot_server(result_normalized, real_voltage_value);
      WiFi.disconnect();
    }
    
    //kleinoume to wifi opws proteinetai kai sto documentation
    Serial.println("Stopping WiFi...");
    esp_wifi_stop(); 
    
    delay(500);
    Serial.println("Calling esp_deep_sleep_start...");
    esp_deep_sleep_start();
  }
  
}
