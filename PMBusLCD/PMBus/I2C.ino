
/************** Include Zone ***************/
#include <Wire.h>
/************** Extern Zone ***************/

/************** Variable Zone ***************/ 

/************** Function Zone ***************/
void I2C_Init()
{  
    
    Wire.begin();
    
}
/*
int I2C_WriteByte(byte devAddr,byte address,byte data)
{
  byte hiByte = ((byte)((address & 0xFF00) >> 8));
  byte loByte = ((byte)(address & 0xFF));   

  Wire.beginTransmission(devAddr);                  
  Wire.write(address);    
  Wire.write(data);   
  int error = Wire.endTransmission();
  if(error != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] ");     
  }
  delay(5);  
  return error;
}

int I2C_Write(byte devAddr,byte address,byte data[],int sizeByte)
{
  //byte hiByte = ((byte)((address & 0xFF00) >> 8));
  //byte loByte = ((byte)(address & 0xFF));   

  Wire.beginTransmission(devAddr);                  
  Wire.write(address);    
  for(int i = 0 ; i  <  sizeByte ; i++)                    
  { 
    byte dataWrite = data[i];
    Wire.write(dataWrite);      
  }   
  int error = Wire.endTransmission();
  if(error != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] ");     
  }
  delay(4);  
  return error;
}  */

double I2C_Read_LinearFormat(byte devAddr,byte address)
{
  //byte hiByte = ((byte)((address & 0xFF00) >> 8));
  //byte loByte = ((byte)(address & 0xFF));  
  Serial.print(" WR "); 
  Serial.print(devAddr,HEX); 
  Wire.beginTransmission(devAddr);   
  Serial.print(" WR ");   
  Serial.print(address,HEX);              
  Wire.write(address);            
  int error = Wire.endTransmission();               
  if(error != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] "); 
    return -999;
  }
    Wire.requestFrom(devAddr,2);  
    byte r1 = Wire.read(); 
    byte r2 = Wire.read(); 
    Serial.print(" RD ");   
    Serial.print(r1,HEX); 
    Serial.print(" ");   
    Serial.print(r2,HEX); 
    Serial.print(" ");   
    int error2 = Wire.endTransmission();               
    if(error2 != 0 )
    {
      Serial.print(" [Error:"); 
      Serial.print(error,HEX); 
      Serial.print("] "); 
      return -998;
    }
    double r = linear11(r2,r1);
    return r;
}
byte I2C_ReadVoutMode(byte devAddr)
{ 
  Wire.beginTransmission(devAddr);         
  Wire.write(0x20);        
  int error = Wire.endTransmission();   
  if(error != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] "); 
    return 0xFF;
  }   
  Wire.requestFrom(devAddr,1); 
  byte r1 = Wire.read();  
  int error2 = Wire.endTransmission();               
  if(error2 != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] "); 
    return 0xFF;
  }
  
  Serial.print(" VoutMode=0x" + String(r1,HEX)); 
  return r1;
}
double I2C_Read_LinearMode(byte devAddr,byte address)
{
   byte VoutMode = I2C_ReadVoutMode(devAddr);
   if(0xFF == VoutMode)
      return -999;
  //byte hiByte = ((byte)((address & 0xFF00) >> 8));
  //byte loByte = ((byte)(address & 0xFF));  
  Serial.print(" WR "); 
  Serial.print(devAddr,HEX); 
  Wire.beginTransmission(devAddr);   
  Serial.print(" WR ");   
  Serial.print(address,HEX);              
  Wire.write(address);            
  int error = Wire.endTransmission();               
  if(error != 0 )
  {
    Serial.print(" [Error:"); 
    Serial.print(error,HEX); 
    Serial.print("] "); 
    return -998;
  }
    Wire.requestFrom(devAddr,2);  
    byte r1 = Wire.read(); 
    byte r2 = Wire.read(); 
    Serial.print(" RD ");   
    Serial.print(r1,HEX); 
    Serial.print(" ");   
    Serial.print(r2,HEX); 
    Serial.print(" ");   
    int error2 = Wire.endTransmission();               
    if(error2 != 0 )
    {
      Serial.print(" [Error:"); 
      Serial.print(error,HEX); 
      Serial.print("] "); 
      return -997;
    }
    double r = linear16(r2,r1,VoutMode);
    return r;
}

/********************************
 * 
 *    Linear Format
 * 
 ***********************************/
double linear11(byte msb, byte lsb)
{
  int data = (int)((msb << 8) | lsb);
     int tmp;
            int exp = 0;
            tmp = (int)(data & 0x07FF);
            double Val = tmp;
            if ((data & 0x8000) != 0)
            {
                exp = data & 0x7FFF;
                exp >>= 11;
                exp -= 1;
                exp = (~exp) & 0x0F;
                exp = 0 - exp;
                if ((tmp & 0x400) != 0)
                {
                    tmp -= 1;
                    tmp = (int)~tmp;
                    tmp &= 0x3FF;
                    Val = tmp * -1;
                }
                Val = Val * pow(2, exp);

            }
            else
            {
                exp = data & 0x7FFF;
                exp >>= 11;
                if ((tmp & 0x400) != 0) //check bit 11 
                {
                    tmp -= 1;
                    tmp = (int)~tmp;
                    tmp &= 0x3FF;
                    Val = tmp * -1;
                }
                Val = Val * pow(2, exp);
            }
            return Val;
}

/***********************************
 * 
 *    Linear Mode
 * 
 ***********************************/
double linear16(byte msb, byte lsb , byte VoutMode)
{
      int data = (int)(((int)msb << 8) | (int)lsb);
      byte rawData = (byte)(VoutMode & 0x1F);
      byte minus_pow = (byte)(rawData & 0x10);
      int exponent_N = 0;
      if (minus_pow == 0x10)
          exponent_N = (byte)(((byte)(~rawData) + 0x01) & 0xF) * (-1);
      else
          exponent_N = rawData;

      int mantissa_V = data;
      return (mantissa_V * pow(2, exponent_N));
}
