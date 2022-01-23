#include "Arduino.h"
//#include "ESP32AnalogRead.h"
#include "WiFi.h"
#include <HTTPClient.h>
//#include "uptime_formatter.h"
//#include "secrets.h"
#include <esp_wifi.h> //for esp_wifi_stop()

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 5 /* Time ESP32 will go to sleep (in seconds) */


esp_sleep_wakeup_cause_t wakeup_reason; //o logos pou egine wakeup apo deep sleep to esp32
RTC_DATA_ATTR int bootCount = 0; //metrisis tou plithous deep sleep - awake

const char* ssid = "B10_120";
const char* password =  "yiannpass";

#define ADC_input_infrared_sensor 32 // diavazoume ton sensora
const int act_sensor = 27; //dinei revma ston sensora


void connect_to_wifi()
{
  delay(500);
  Serial.println("Attempt WiFi connection...");
  //syndesi sto wifi
  //WiFi.mode(WIFI_STA);
  Serial.println("CALL WiFi.begin...");
  WiFi.begin(ssid, password);
  Serial.println("CHECK WiFi.status()...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
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
  Serial.println("Entering SETUP");
  print_wakeup_reason();

  
  Serial.println("Set esp_sleep_enable_timer_wakeup to: " + (String)TIME_TO_SLEEP + " seconds");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

void send_data_to_iot_server(int smoke_value)
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
  json += "\"smoke_value\":\""           + ((String)smoke_value)                   + "\"";
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


void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop Start");
  Serial.println("Activate Sircuit");
  digitalWrite(act_sensor, HIGH);
  delay(1000);
  
  float result_normalized = AnalogReadNormalized(ADC_input_infrared_sensor, 10, 10);
  Serial.print("HIGH result_normalized: ");
  Serial.println(result_normalized);
  Serial.println("Deactivate Sircuit");
  digitalWrite(act_sensor, LOW);

  //if result_normalized
  if (bootCount*TIME_TO_SLEEP >= 250 || true) {
    bootCount = 0;
    //Anigo to wifi
    connect_to_wifi();

    send_data_to_iot_server(result_normalized);

    //kleinoume to wifi opws proteinetai kai sto documentation
    Serial.println("Stopping WiFi...");
    WiFi.disconnect();
    esp_wifi_stop();
    }

  Serial.println("Calling esp_deep_sleep_start...");
  esp_deep_sleep_start();
  
  delay(3000);
}
