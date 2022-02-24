#include <AirGradient.h>
#include <ESP8266WiFi.h>
#include <qrcode.h>
#include "SSD1306Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "uptime_formatter.h"

// Needed for Wifimanager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"        

std::unique_ptr<ESP8266WebServer> server;

// Display Fonts
#include "_fonts/opensans-Extrabold-22.h"
#include "_fonts/opensans-light-12.h"
#include "_fonts/opensans-regular-22.h"

// Webinterface
#include "_pages/index.h"
#include "_pages/state.h"

const char *AP_NAME= "AirTank_Config"; //#EDIT AP Name during config
const char *HOST_NAME= "AirTank"; //#EDIT ME Hostname

const bool hasPM = true;
const bool hasCO2 = true;
const bool hasSHT = true;

const int updateFrequency = 5000; //#EDIT Display update 
long lastUpdate;
int counter = 0;

String espRestart;
String espCoreV;
String espJson;
String espUpTime;

int espChipId;
int espMhz;
int espSketch;

int htmlCO2;
int htmlPM01;
int htmlPM02;
int htmlPM10;

int led = 15;

float tempOffset  = 1; //#EDIT Temperature Offset (0.9 = 90%)
int switchOff = 23; //#EDIT Display off time (23:00)
int switchOn  = 7; //#EDIT Display on time (7:00)
const long utcOffsetInSeconds = 3600; // Timezone (Berlin)
String overwrite = "No"; // Sleepmode overwrite

const int sleepUpdateFrequency = 900000; //Display off check (15min)
long sleeplastUpdate;

AirGradient ag = AirGradient();
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48);
QRcode qrcode(&display);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.europe.pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(115200);
  
  // Light Pin
  pinMode(led, OUTPUT);
  
  //Display initialization
  display.init();
  //display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  //QRCODE initialization
  qrcode.init();  
  
  //Boot Message
  displayFontSmall("Booting...","","");

  //Wifi initialization
  WiFiManager wifiManager;
  //Configuration Portal Timeout
  wifiManager.setConfigPortalTimeout(180);
  bool wifi_state;
  //wifiManager.resetSettings(); for debugging
  wifi_state = wifiManager.autoConnect(AP_NAME);
  WiFi.hostname(HOST_NAME);
  
  // Airgradient Sensor initialization
  if (hasPM)
    ag.PMS_Init();
  if (hasCO2)
    ag.CO2_Init();
  if (hasSHT)
    ag.TMP_RH_Init(0x44);

  if(!wifi_state) {
    displayFontSmall("WIFI","CONFIG","MODE");
    delay(2000);   
  } 
  else {
    displayFontSmall("Local Config", WiFi.localIP().toString(), HOST_NAME );
    delay(5000);
    qrcode.create("http://" + WiFi.localIP().toString());
    delay(5000);
  }

  //NTP initialization
  timeClient.begin();

  //Webserver initialization
  server.reset(new ESP8266WebServer(WiFi.localIP(), 80));
  server->on("/", handleRoot);
  server->on("/json", handleJson);
  server->on("/led", handleLED);
  server->on("/settings", handleSettings);
  server->on("/reboot", handleReboot);
  server->on("/displayoff", handleDisplayoff);
  server->on("/displayon", handleDisplayon);
  server->onNotFound(handleNotFound);
  server->begin();

  //Fill Systeminfo Variables
  espRestart = ESP.getResetReason();
  espChipId = ESP.getChipId();
  espCoreV = ESP.getCoreVersion();
  espMhz = ESP.getCpuFreqMHz();
  espSketch = ESP.getSketchSize();
  espJson = ("http://" + WiFi.localIP().toString() + "/json");

}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Main Loop
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
  long t = millis();
  server->handleClient();
  updateScreen(t);
  if (overwrite == "No") {sleepScreen(t);}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Display Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void sleepScreen(long now) {
  //Serial.println("Active");
  if ((now - sleeplastUpdate) > sleepUpdateFrequency) {
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

void updateScreen(long now) {
  if ((now - lastUpdate) > updateFrequency) {
    switch (counter) {
      case 0:
        if (hasCO2) {
          int stat = ag.getCO2_Raw();
          displayFontBig("CO2", String(stat));
        }
        break;
      case 1:
        if (hasSHT) {
          TMP_RH stat = ag.periodicFetchData();
          displayFontBig("HUM", String(stat.rh) + "%");
        }
        break;
      case 2:
        if (hasSHT) {
          TMP_RH stat = ag.periodicFetchData();
          String tmpstat = temperatureOffset(stat.t);
          tmpstat = tmpstat.substring(0,4);
          displayFontBig("TMP", tmpstat + "C");
        }
        break;
      case 3:
        if (hasPM) {
          int stat = ag.getPM1_Raw();
          htmlPM01 = stat;
          displayFontBig("PM01", String(stat));
        }
        break;
      case 4:
        if (hasPM) {
          int stat = ag.getPM2_Raw();
          htmlPM02 = stat;
          displayFontBig("PM02", String(stat));
        }
        break;
      case 5:
        if (hasPM) {
          int stat = ag.getPM10_Raw();
          htmlPM10 = stat;
          displayFontBig("PM10", String(stat));
        }
        break;
    }
    counter++;
    if (counter > 5)
      counter = 0;
      lastUpdate = millis();
  }
}

void displayFontBig(String ln1, String ln2) {
  display.clear();
  display.setFont(Open_Sans_ExtraBold_22);
  display.drawString(0, -8, ln1);
  display.setFont(Open_Sans_Regular_22);
  display.drawString(0, 16, ln2);
  display.fillRect(2, 19, 58, 2);
  display.display();
}

void displayFontSmall(String ln1, String ln2, String ln3) {
  display.clear();
  display.setFont(Open_Sans_Condensed_Light_12);
  display.drawString(0, 0, ln1);
  display.drawString(0, 12, ln2);
  display.drawString(0, 24, ln3);
  display.display();
}

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


String temperatureOffset(float originalTemp){
  float tempsum = originalTemp * tempOffset;
  return String(tempsum);
}
