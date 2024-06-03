#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <TimeLib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231 rtc(SDA, SCL);

const int AlarmProperties = 8;
const int AlarmEditor = 9;

void setup() {
  pinMode(AlarmProperties, INPUT); 
  pinMode(AlarmEditor, INPUT); 

  lcd.init();
  lcd.backlight();

  rtc.begin();
  //rtc.setTime(10, 55, 00);
  //rtc.setDate(30, 5, 2024);

  Serial.begin(9600);
}

int AlarmMod = 0;

int isAlarmOn = 0;
int AlarmHr = 0;
int AlarmMinute = 0;
int AlarmSecond = 0;

int AlarmOnMod = 0;
int AlarmHrMod = 0;
int AlarmMinMod = 0;
int AlarmSecondMod = 0;

void loop() 
{
  //Time, AlarmOnOff, AlarmHr, AlarmMin, AlarmSecond

  if (digitalRead(AlarmProperties) == HIGH)
  {
    if (AlarmMod % 2 == 0)
      AlarmMod = (AlarmMod + 1) % 10;
  }
  else
  {
    if (AlarmMod % 2)
    {
      AlarmMod = (AlarmMod + 1) % 10;
      lcd.clear();
    }

    {
      if (AlarmMod == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print(rtc.getTimeStr());
        lcd.setCursor(0, 1);
        lcd.print(rtc.getDateStr());
      }
      else if (AlarmMod == 2)
      {
        lcd.setCursor(0, 0);
        lcd.print("Alarm On?");

        if (digitalRead(AlarmEditor) == HIGH)
        {
          if (AlarmOnMod % 2 == 0)
            AlarmOnMod = (AlarmOnMod + 1) % 4;
        }
        else
        {
          if (AlarmOnMod % 2)
          {
            AlarmOnMod = (AlarmOnMod + 1) % 4;
            isAlarmOn = 1 - isAlarmOn;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print(isAlarmOn? "ON " : "OFF");
      }
      else if (AlarmMod == 4)
      {
        lcd.setCursor(0, 0);
        lcd.print("Alarm Hour?");

        if (digitalRead(AlarmEditor) == HIGH)
        {
          if (AlarmHrMod % 2 == 0)
            AlarmHrMod = (AlarmHrMod + 1) % 48;
        }
        else
        {
          if (AlarmHrMod % 2)
          {
            AlarmHrMod = (AlarmHrMod + 1) % 48;
            AlarmHr = (AlarmHr + 1) % 24;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print(AlarmHr);
        lcd.print(" ");
        //lcd.setCursor(0, 1);
        //lcd.print(rtc.getDateStr());
      }
      else if (AlarmMod == 6)
      {
        lcd.setCursor(0, 0);
        lcd.print("Alarm Minute?");

        if (digitalRead(AlarmEditor) == HIGH)
        {
          if (AlarmMinMod % 2 == 0)
            AlarmMinMod = (AlarmMinMod + 1) % 120;
        }
        else
        {
          if (AlarmMinMod % 2)
          {
            AlarmMinMod = (AlarmMinMod + 1) % 120;
            AlarmMinute = (AlarmMinute + 1) % 60;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print(AlarmMinute);
        lcd.print(" ");
      }
      else if (AlarmMod == 8)
      {
        lcd.setCursor(0, 0);
        lcd.print("Alarm Second?");

        if (digitalRead(AlarmEditor) == HIGH)
        {
          if (AlarmSecondMod % 2 == 0)
            AlarmSecondMod = (AlarmSecondMod + 1) % 120;
        }
        else
        {
          if (AlarmSecondMod % 2)
          {
            AlarmSecondMod = (AlarmSecondMod + 1) % 120;
            AlarmSecond = (AlarmSecond + 1) % 60;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print(AlarmSecond);
        lcd.print(" ");
      }
    }
  }
  String s = rtc.getTimeStr();
  //delay(1000);
  String AlarmHrToString = (AlarmHr < 10)? ("0" + String(AlarmHr)) : String(AlarmHr);
  String AlarmMinuteToString = (AlarmMinute < 10)? ("0" + String(AlarmMinute)) : String(AlarmMinute);
  String AlarmSecondToString = (AlarmSecond < 10)? ("0" + String(AlarmSecond)) : String(AlarmSecond);
  String AlarmTime = AlarmHrToString + ":" + AlarmMinuteToString + ":" + AlarmSecondToString;
  //Serial.println(AlarmTime);
  if ((AlarmTime == s) && (isAlarmOn))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARM! To stop,");
    lcd.setCursor(0, 1);
    lcd.print("right button");

    int StopStatus = 0;
    for (int i = 1; i <= 60; i++)
    {
      lcd.backlight();
      tone(10, 784, 1/500);
      delay(500);
      lcd.noBacklight();
      noTone(10);
      delay(500);
      if (digitalRead(AlarmEditor) == HIGH)
      {
        if (StopStatus == 0) StopStatus++;
      }
      else 
        if (StopStatus) 
        {
          isAlarmOn = 1 - isAlarmOn;
          AlarmOnMod = (AlarmOnMod + 2) % 4;
          break;
        }
    }
    lcd.clear();
    lcd.backlight();
  }
}