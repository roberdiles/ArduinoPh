#include <SPI.h>
#include <SD.h>

File dataFile;

float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;


const int chipSelect = 10;


void setup() 
{
 Serial.begin(9600);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) 
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  delay(2000);
}

uint16_t line = 1;

void loop() {
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
 delay(1000);

 dataFile = SD.open("pH.txt", FILE_WRITE);
 
 if (dataFile) {  
  //Serial.print(line);
  Serial.print("pH = ");
  Serial.println(ph_act);

  //dataFile.print(line++);
  dataFile.print("pH = ");
  dataFile.println(ph_act);
  dataFile.close();
  }
  else
    Serial.println("error opening pH.txt");
}
