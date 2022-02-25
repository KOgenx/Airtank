// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Webserver Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void handleLED() {
  //Toggle LED
  int currState = digitalRead(led);
  if (currState == LOW) {
    digitalWrite(led, HIGH);
    server->send(200, "text/html", StatePage("LED is on"));
  } else {
    digitalWrite(led, LOW);
    server->send(200, "text/html", StatePage("LED is off"));
  }
}

void handleReboot() {
  server->send(200, "text/html", StatePage("Rebooting!"));
  delay(1000);
  ESP.restart();
}

void handleDisplayoff() {
  display.displayOff();
  server->send(200, "text/html", StatePage("Display is off!"));
}
void handleDisplayon() {
  display.displayOn();
  server->send(200, "text/html", StatePage("Display is on!"));
}

String StatePage(String msg) {
  String s = STATE_page;
  s.replace("@@txtState@@", msg);
  s.replace("@@ehostename@@", String(HOST_NAME));
  return String(s);
}

void handleRoot() { 
  String s = MAIN_page;
  TMP_RH stat = ag.periodicFetchData();
  espUpTime = uptime_formatter::getUptime();
  htmlCO2 = ag.getCO2_Raw();
  
  int LEDread = digitalRead(led);
  String LEDState = "unset";
  if (LEDread == 1) {LEDState = "is on";} else {LEDState = "is off";}
  String LEDUrl = "http://" + WiFi.localIP().toString() + "/led";

  if (overwrite == "Yes") {
    s.replace("@@overwriteNo@@", " ");
    s.replace("@@overwriteYes@@", "selected");
  } else {
    s.replace("@@overwriteNo@@", "selected");
    s.replace("@@overwriteYes@@", " ");
  }

  s.replace("@@C02@@", String(htmlCO2));
  s.replace("@@PM01@@", String(htmlPM01));
  s.replace("@@PM02@@", String(htmlPM02));
  s.replace("@@PM10@@", String(htmlPM10));
  s.replace("@@tmp@@", temperatureOffset(stat.t));
  s.replace("@@hum@@", String(stat.rh));
  
  s.replace("@@espRestart@@", String(espRestart));
  s.replace("@@espChipId@@", String(espChipId, HEX));
  s.replace("@@espCoreV@@", String(espCoreV));
  s.replace("@@espMhz@@", String(espMhz));
  s.replace("@@espSketch@@", String(espSketch));
  s.replace("@@espJson@@", String(espJson));
  s.replace("@@ehostename@@", String(HOST_NAME));
  s.replace("@@espUptime@@", String(espUpTime));

  s.replace("@@switchOff@@", String(switchOff));
  s.replace("@@switchOn@@", String(switchOn));
  s.replace("@@tempOffset@@", String(tempOffset));

  s.replace("@@ledState@@", String(LEDState));
  s.replace("@@ledUrl@@", String(LEDUrl));

  server->send(200, "text/html", s); 
}

void handleSettings() {
  String s = STATE_page;
  uint8_t counterARG = 0;
  for (uint8_t i = 0; i < server->args(); i++) {
    if (server->argName(i) == "SwitchOFF")
    {
      switchOff = server->arg(i).toInt();
      counterARG++;
    } 
    else if (server->argName(i) == "SwitchON")
    {
      switchOn = server->arg(i).toInt();
      counterARG++;
    }
    else if (server->argName(i) == "tempOffset")
    {
      tempOffset = server->arg(i).toFloat();
      counterARG++;
    }
    else if (server->argName(i) == "overWrite")
    {
      overwrite = server->arg(i);
      counterARG++;
    }
  }
  server->send(200, "text/html", StatePage("Settings saved!"));
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";
  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }
  server->send(404, "text/plain", message);
}

void handleJson() { server->send(200, "text/plain", GenerateJson()); }

String GenerateJson() {
  String message = "";
  String idString = "\"id\":\"" + String(HOST_NAME) + "\",\"mac\":\"" + WiFi.macAddress().c_str() + "\"";

  message += "{";
  message += idString;
  message += ",";

  if (hasPM) {
    int stat = ag.getPM1_Raw();
    message += "\"pm01\":\"";
    message += String(stat);
    message += "\",";
    delay(100);
    stat = ag.getPM2_Raw();
    message += "\"pm02\":\"";
    message += String(stat);
    message += "\",";
    delay(100);
    stat = ag.getPM10_Raw();
    message += "\"pm10\":\"";
    message += String(stat);
    message += "\",";
  }

  if (hasCO2) {
    int stat = ag.getCO2_Raw();
    message += "\"rco2\":\"";
    message += String(stat);
    message += "\",";
  }

  if (hasSHT) {
    TMP_RH stat = ag.periodicFetchData();

    message += "\"atmp\":\"";
    message += temperatureOffset(stat.t);
    message += "\",";

    message += "\"rhum\":\"";
    message += String(stat.rh);
    message += "\"";
  }
  message += "}";
  return message;
}