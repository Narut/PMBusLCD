#include <Wire.h>
#include <UTFT.h>
//#include <avr/wdt.h>

/* Address */
byte DevAddr     = 0x58;/* 0x40 => 0x80 */

/* Command */
byte VIN_CMD        = 0x88;
byte IIN_CMD        = 0x89;
byte PIN_CMD        = 0x97;
byte VOUT_CMD       = 0x8B;
byte IOUT_CMD       = 0x8C;
byte POUT_CMD       = 0x96;
byte FAN_CMD       = 0x90;

/* Register values */
double Vi        = 0;
double Ii        = 0;
double Pi        = 0;
double Vo        = 0;
double Io        = 0;
double Po        = 0;

double rpm       = 0;
double eff       = 0;
String text = "";
UTFT myGLCD(CTE32HR,38,39,40,41);

void setup() {
  Serial.begin(9600);
  I2C_Init(); 
  myGLCD.InitLCD();
  setpicture();

   //wdt_enable(WDTO_2S);// 1sec watch dog
}


void loop() {
  //wdt_reset();//Feed the dog
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  /*** Vin ***/
  Vi = I2C_Read_LinearFormat(DevAddr,VIN_CMD);
  text = "Addr=0x" + String(DevAddr<<1,HEX);
  myGLCD.print(text,300,10);
  if(Vi == -999)
  {
      DevAddr = 0x5F; //BE
      text = "Addr=0x" + String(DevAddr<<1,HEX);
      myGLCD.print(text,300,10);
      Vi = I2C_Read_LinearFormat(DevAddr,VIN_CMD);
      if(Vi == -999)
      {  
          DevAddr = 0x58; //B0
          text = "Addr=0x" + String(DevAddr<<1,HEX);
          myGLCD.print(text,300,10);
          Vi = I2C_Read_LinearFormat(DevAddr,VIN_CMD); 
          if(Vi == -999)
          {  
              DevAddr = 0x47; //8E
              text = "Addr=0x" + String(DevAddr<<1,HEX);
              myGLCD.print(text,300,10);
              Vi = I2C_Read_LinearFormat(DevAddr,VIN_CMD); 
              if(Vi == -999)
              {  
                  DevAddr = 0x40; //80
                  text = "Addr=0x" + String(DevAddr<<1,HEX);
                  myGLCD.print(text,300,10);
                  Vi = I2C_Read_LinearFormat(DevAddr,VIN_CMD); 
              }
          }
      }     
  }
  Serial.print("Vin = ");
  Serial.print(Vi);  
  Serial.println(" V");  
  text= "Vin=" + String(Vi, DEC).substring(0,6) + " V ";
  myGLCD.print(text,40,10);

  /*** Iin ***/
  Ii = I2C_Read_LinearFormat(DevAddr,IIN_CMD);
  Serial.print("Iin = ");
  Serial.print(Ii);
  Serial.println(" A");
   text= "Iin=" + String(Ii, DEC).substring(0,5) + " A ";
  myGLCD.print(text,40,40);

  /*** Pin ***/
  Pi = I2C_Read_LinearFormat(DevAddr,PIN_CMD);
  Serial.print("Pin = ");
  Serial.print(Pi);
  Serial.println("  W");
   text= "Pin=" + String(Pi, DEC).substring(0,5) + " W ";
  myGLCD.print(text,40,70);
  
  /*** Vout ***/
  Vo = I2C_Read_LinearMode(DevAddr,VOUT_CMD);
  Serial.print("Vout = ");
  Serial.print(Vo);
  Serial.println(" V");
   text= "Vout=" + String(Vo, DEC).substring(0,5) + " V ";
  myGLCD.print(text,40,130);
  
  /*** Iout ***/
  Io = I2C_Read_LinearFormat(DevAddr,IOUT_CMD);
  Serial.print("Iout = ");
  Serial.print(Io);
  Serial.println(" A");
   text= "Iout=" + String(Io, DEC).substring(0,5) + " A ";
  myGLCD.print(text,40,160);

  /*** Pout ***/
  Po = I2C_Read_LinearFormat(DevAddr,POUT_CMD);
  Serial.print("Pout = ");
  Serial.print(Po);
  Serial.println("  W");
   text= "Pout=" + String(Po, DEC).substring(0,5) + " W ";
  myGLCD.print(text,40,190);
  
  /*** Fan ***/
  rpm = I2C_Read_LinearFormat(DevAddr,FAN_CMD);
  Serial.print("Fan Speed = ");
  Serial.print(rpm);
  Serial.println(" rpm");
   text= "Fan=" + String(rpm, DEC).substring(0,7) + " RPM ";
  myGLCD.print(text,40,240);

  /*** EFF ***/
  if(Pi == 0)
  {
    text = "EFF=INF%        ";    
  }
  else
  {
    eff = ((double)Po*100.0)/(double)Pi;
    text = "EFF=" + String(eff, DEC).substring(0,5) + "% ";    
  }
  myGLCD.print(text,40,290);
  
  delay(1);
}

 
