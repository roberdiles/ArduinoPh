// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

File dataFile;

float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;


const int chipSelect = 10;

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

void setup () {
  Serial.begin(9600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
   // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
   //rtc.adjust(DateTime(2023, 1, 26, 11, 50, 0));
   
   Serial.print("Initializing SD card...");
  if (!SD.begin(10)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  delay(2000);
}

void loop () {
    DateTime now = rtc.now();
    for(int i=0;i<10;i++) 
   { 
   buffer_arr[i]=analogRead(A0);
   delay(30);
   }
   for(int i=0;i<9;i++)
   {
   for(int j=i+1;j<10;j++)
   {
   if(buffer_arr[i]>buffer_arr[j])
   {
   temp=buffer_arr[i];
   buffer_arr[i]=buffer_arr[j];
   buffer_arr[j]=temp;
   }
   }
   }
   avgval=0;
   for(int i=2;i<8;i++)
   avgval+=buffer_arr[i];
   float volt=(float)avgval*5.0/1024/6;
   float ph_act = -5.70 * volt + calibration_value;
   //Serial.print(ph_act);
   //Serial.print('\n');
   
   dataFile = SD.open("pH.txt", FILE_WRITE);
   
   if (dataFile) {  
   
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(5000);

    Serial.print("pH: ");
    Serial.println(ph_act);
  
    //dataFile.print(line++);
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(" (");
    dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
    dataFile.print(") ");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.println();
    dataFile.print("pH = ");
    dataFile.println(ph_act);
    delay(5000);
    dataFile.close();
    }
    else
    Serial.println("error opening pH.txt");
}
