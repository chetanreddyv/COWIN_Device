

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <SoftwareSerial.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Adafruit_SH1106 display(-1);

int motor = 4;
int IR = 2;
float roomTemp; // ambient temperature
float objectTemp, objectTemp1, objectTemp2, objectTemp3;  // object temperature

int readcount = 0;
float threshold= 8.8 ;


void setup() {

  pinMode(IR, INPUT);
  pinMode(motor, OUTPUT);
  Serial.begin(9600);
  // setting up the OLED display
  display.begin(SH1106_SWITCHCAPVCC,0x3C) ;
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop()

{
  digitalWrite(motor, HIGH);
  int IRval = digitalRead(IR);
  Serial.println(IRval);
  if (IRval == 0)
  {
    pump();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Scanning..");
  display.display(); 
  delay(500);
  objectTemp1 = threshold + mlx.readObjectTempF() ;  
  delay(500);
  objectTemp2 = threshold + mlx.readObjectTempF() ;  
  delay(500);
  objectTemp3 = threshold + mlx.readObjectTempF() ;  
  
  objectTemp = (objectTemp1 + objectTemp2 + objectTemp3)/3;
  //roomTemp = mlx.readAmbientTempF() ;  


  // print to Serial port
 Serial.println(objectTemp);

 
  // display on OLED
  
   display.clearDisplay();
  display.setTextSize(2);
     display.setCursor(0,0);
      display.print("your temp:");
      display.setTextSize(2);
      display.setCursor(60,18);
      display.print(String(objectTemp).substring(0, 4) + "F");
     display.display();
     delay(4000);
      
      if (objectTemp >= 99.5) 
      {
        sendsms();
       delay(100);
        play_alert();
     delay(100);
      } 
      else 
      {
       display.clearDisplay();

     display.setTextSize(2);

     display.setCursor(0,18);
     
     display.print(" thankyou ");
    
     display.display();
    
    delay(1000); 
      }
  }
  else
  {
     display.clearDisplay();

     display.setTextSize(2);

     display.setCursor(0,18);
     
     display.print(" Welcome ");
    
     display.display();
    
    delay(1000); 
  }
 
 
 }



void play_alert() { // beep 3x when object temperature is >= 37.5C
  tone(3, 2000, 1000);
  delay(1000);
  tone(3, 3000, 1000);
  delay(1000);
  tone(3, 4000, 1000);
  delay(1000);
  noTone(3);
}

void sendsms()
{
  SoftwareSerial mySerial(9, 10);
  mySerial.begin(9600);
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);  // Delay of 1 second
     mySerial.println("AT+CMGS=\"+919100855854\"\r"); // Replace x with mobile number
     delay(1000);
     mySerial.println("alert fever");// The SMS text you want to send
     delay(100);
     mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
      delay(1000);
}

void pump()
{ 
  digitalWrite(motor, HIGH);
  delay(500);
  digitalWrite(motor, LOW);
  delay(1000);
  digitalWrite(motor, HIGH);
}
