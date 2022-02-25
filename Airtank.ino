#include <AirGradient.h>
#include <ESP8266WiFi.h>
#include <qrcode.h>
#include "SSD1306Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "uptime_formatter.h"
#include <ArduinoOTA.h>

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
float espSketch;

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
  espSketch = (ESP.getSketchSize() / 1024);
  espJson = ("http://" + WiFi.localIP().toString() + "/json");

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Main Loop
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
  long t = millis();
  server->handleClient();
  updateScreen(t);
  sleepScreen(t);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Temp Offset Function
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String temperatureOffset(float originalTemp){
  float tempsum = originalTemp * tempOffset;
  return String(tempsum);
}
