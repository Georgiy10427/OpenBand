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
class Menu
{
public:
    bool isTreeActive = true;
    int HideParagraph(int index)
    {
        if (Screens[index].find("$") > Screens[index].length() 
            && !isFind(Screens[index], "Home") 
            && !isFind(Screens[index], "Settings"))
        {
            Screens[index] = Screens[index] + std::string("$");
            return 0;
        }
        else
        {
            return -1;
        }
    }
    void ShowParagraph(int index)
    {
        if (Screens[index].find("$") > Screens[index].length())
        {
        }
        else
        {
            std::string* str = &Screens[index];
            str->erase(std::remove(str->begin(), str->end(), '$'), str->end());
            Screens[index] = *str;
        }
    }
    int SetActiveParagraph(int index)
    {
        for (int i = 0;  arraySize(Screens) > i; i++) 
        {
            if (isFind(Screens[i], "!"))
            {
                std::string* str = &Screens[i];
                str->erase(std::remove(str->begin(), str->end(), '!'), str->end());
                Screens[i] = *str;
            }
        }
        if (isFind(Screens[index], "$") == false) 
        {
           Screens[index] = std::string("!") + Screens[index];
           return 0;
        }
        else
        {
            return -1;
        }
        
    }
    int GetActiveParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "!") && !isFind(Screens[i], "$"))
            {
                return i;
            }
        }
        return -1;
    }
    int GetIndex(std::string str) 
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], str))
            {
                return i;
            }
        }
    }
    std::string GetTextParagrath(int index)
    {
        return Screens[index];
    }
    int GetHomeParagrath() 
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Home"))
            {
                return i;
            }
        }
    }
    int GetStepsParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Steps"))
            {
                return i;
            }
        }
    }
    int GetPulseParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Pulse"))
            {
                return i;
            }
        }
    }
    int GetSettingsParagrath()
    {
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "Settings"))
            {
                return i;
            }
        }
    }
    int* GetAllIndexes()
    {
        int IndexesSize = 0;
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "$") == false)
            {
                IndexesSize++;
            }
        }
        int* indexes = new int[IndexesSize];
        for (int i = 0; arraySize(Screens) > i; i++)
        {
            if (isFind(Screens[i], "$") == false)
            {
                indexes[i] = i;
            }
        }
        return indexes;
    }
    int GetNextIndex(int index)
    {
      for (int i = index; i < arraySize(Screens); i++)
      {
            if (isFind(Screens[i], "$") == false)
            {
                return i;
            }
       }
       
    }
    int GetQuantityParagraths()
    {
      return arraySize(Screens);
    }
private:
    std::string Screens[4] = {
                        "Home",
                        "Steps",
                        "Pulse",
                        "Settings"
    };

    template<typename T, size_t n>
    inline size_t arraySize(const T(&arr)[n])
    {
        return n;
    }

    bool isFind(std::string str, std::string subString)
    {
        if (str.find(subString) > str.length())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

template<typename T, size_t n>
inline size_t arraySize(const T (&arr)[n])
{
    return n;
}

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

void HomeShow()
{
  screen.fillScreen(TFT_BLACK);

}
void StepsShow()
{
  screen.fillScreen(TFT_WHITE);
}
void PulseShow()
{
  screen.fillScreen(TFT_RED);
}
void SettingsShow()
{
  screen.fillScreen(TFT_BLUE);
}

void NextActive(int index)
{
  for(int i = index; i < MainMenu.GetQuantityParagraths(); i++){
    int r = MainMenu.SetActiveParagraph(i);
    if(r == 0)
    {
      if(i == MainMenu.GetHomeParagrath())
      {
        HomeShow();
        break;
      }
      else if(i == MainMenu.GetPulseParagrath())
      {
        PulseShow();
        break;
      }
      else if (i == MainMenu.GetStepsParagrath())
      {
        StepsShow();
        break;
      }
      else if(i == MainMenu.GetSettingsParagrath())
      {
        SettingsShow();
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
        screen.fillScreen(TFT_WHITE);
      }
      else
      {
        NextActive(MainMenu.GetStepsParagrath());
      }
    }
    else if (MainMenu.GetActiveParagrath() == MainMenu.GetStepsParagrath())
    {
      int r = MainMenu.SetActiveParagraph(MainMenu.GetPulseParagrath());
      if(r == 0)
      {
        PulseShow();
      }
      else
      {
        NextActive(MainMenu.GetPulseParagrath());
      }
      
    }
    else if (MainMenu.GetActiveParagrath() == MainMenu.GetPulseParagrath())
    {
      int r = MainMenu.SetActiveParagraph(MainMenu.GetSettingsParagrath());
      if(r == 0)
      {
        SettingsShow();
      }
      else
      {
        NextActive(MainMenu.GetSettingsParagrath());
      }
      
    }
    else if(MainMenu.GetActiveParagrath() == MainMenu.GetSettingsParagrath())
    {
      MainMenu.SetActiveParagraph(MainMenu.GetHomeParagrath());
      HomeShow();
    }  
  }
}