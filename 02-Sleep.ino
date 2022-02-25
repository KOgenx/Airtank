void sleepScreen(long now) {

  if ((now - sleeplastUpdate) > sleepUpdateFrequency && (overwrite == "No")) {
    timeClient.update();
    int ntpHour = timeClient.getHours();
    
    if (ntpHour == switchOff) {
       display.displayOff();
       digitalWrite(led, LOW);
    } else if (ntpHour == switchOn) {
       display.displayOn();
    }
    sleeplastUpdate = millis();
  }
}
