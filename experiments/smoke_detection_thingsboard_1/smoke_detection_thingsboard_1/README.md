![alternative text](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/YiannisBourkelis/IoT-Fire-Alarm/master/experiments/smoke_detection_thingsboard_1/smoke_detection_thingsboard_1/activity_diagram.txt)


https://www.planttext.com


```
@startuml

start

repeat
  :take measurement or ULP;

  if (moke detected?) then (yes)
    :activate beeper;
    :connect to internet;
    :send alarm json;
    :sleep alarm_sleep_seconds_20;
  else (no)
  	:deactivate beeper;
  
    :connect to wifi;
    :send data;
    :close wifi;
    :sleep 30sec enter ULP mode;
    
  endif
repeat while (more data?) is (yes)
->no;
stop

@enduml
```
