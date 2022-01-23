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