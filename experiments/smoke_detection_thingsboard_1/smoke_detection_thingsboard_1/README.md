![alternative text](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/YiannisBourkelis/IoT-Fire-Alarm/master/experiments/smoke_detection_thingsboard_1/smoke_detection_thingsboard_1/activity_diagram.txt)


https://www.planttext.com


```
@startuml

start

repeat
  :take measurement;

  if (moke detected?) then (yes)
    :activate beeper;
    :connect to internet;
    :send alarm json;
  else (no)
  
    if (should send measurement after 5min?) then (yes)
      :connect to wifi;
      :send data;
      :close wifi;
      :reset counter;
    else (no)
      :counter++;
    endif
    
    :sleep 30sec;
    
  endif
repeat while (more data?) is (yes)
->no;
stop

@enduml
```
