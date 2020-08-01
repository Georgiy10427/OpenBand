#include <Arduino.h>
#include <TFT_eSPI.h>
#include <string.h>
#include <map>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include "GyverButton.h"
#include "WiFiSettings.h"
#include "ArduinoJson.h"
#include "Menu.h"
#include "slots.h"
#include "homeScreen.h"
#include "stepsScreen.h"
#include "settingsScreen.h"
#include "pulseScreen.h"

const int TP_PWR_PIN = 25;
const int ChargePin = 32;
const int TouchButton = 33;
const int Motor = 14;

TFT_eSPI screen = TFT_eSPI(); 
GButton touch(TouchButton);
int orientationScreen = 1;

//Menu
/*
-Home
-Steps
-Pulse
-Settings
 -Brightness
 -Time and Date
 -Alarm
 ...
*/

Menu MainMenu;

void setup() {
  touch.setType(LOW_PULL);
  pinMode(TP_PWR_PIN, PULLUP);
  pinMode(TouchButton, INPUT);
  pinMode(ChargePin, INPUT_PULLUP);
  pinMode(Motor, OUTPUT);
  MainMenu.SetActiveParagraph(MainMenu.GetHomeParagrath());
  MainMenu.HideParagraph(MainMenu.GetPulseParagrath());
  screen.init();
  screen.setRotation(orientationScreen);
  screen.setSwapBytes(true);
  screen.fillScreen(TFT_BLACK);
}

void NextActive(int slot, int index)
{
  for(int i = index; i < MainMenu.GetQuantityParagraths(); i++){
    int r = MainMenu.SetActiveParagraph(i);
    if(r == 0)
    {
      if(i == MainMenu.GetHomeParagrath())
      {
        HomeShow(slot);
        break;
      }
      else if(i == MainMenu.GetPulseParagrath())
      {
        PulseShow(slot);
        break;
      }
      else if (i == MainMenu.GetStepsParagrath())
      {
        StepsShow(slot);
        break;
      }
      else if(i == MainMenu.GetSettingsParagrath())
      {
        SettingsShow(slot);
        break;
      }
    }
  }
}

void loop() {
  touch.tick();
  if(touch.isPress()){
    if(MainMenu.GetActiveParagrath() == MainMenu.GetHomeParagrath())
    {
      int r = MainMenu.SetActiveParagraph(MainMenu.GetStepsParagrath());
      if(r == 0){
        PulseShow(SlotPress);
      }
      else
      {
        NextActive(SlotPress, MainMenu.GetStepsParagrath());
      }
    }
    else if (MainMenu.GetActiveParagrath() == MainMenu.GetStepsParagrath())
    {
      int r = MainMenu.SetActiveParagraph(MainMenu.GetPulseParagrath());
      if(r == 0)
      {
        PulseShow(SlotPress);
      }
      else
      {
        NextActive(SlotPress, MainMenu.GetPulseParagrath());
      }
      
    }
    else if (MainMenu.GetActiveParagrath() == MainMenu.GetPulseParagrath())
    {
      int r = MainMenu.SetActiveParagraph(MainMenu.GetSettingsParagrath());
      if(r == 0)
      {
        SettingsShow(SlotPress);
      }
      else
      {
        NextActive(SlotPress, MainMenu.GetSettingsParagrath());
      }
      
    }
    else if(MainMenu.GetActiveParagrath() == MainMenu.GetSettingsParagrath())
    {
      MainMenu.SetActiveParagraph(MainMenu.GetHomeParagrath());
      HomeShow(SlotPress);
    }  
  }
}