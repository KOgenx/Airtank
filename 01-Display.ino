// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Display Functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
