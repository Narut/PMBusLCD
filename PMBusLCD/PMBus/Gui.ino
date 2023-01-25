extern uint8_t BigFont[];

void setpicture()
 {
  myGLCD.clrScr();
  myGLCD.fillScr(200,200,150);
  myGLCD.setColor(0,0,0); 
   /*myGLCD.drawLine(0,150,479, 150);
  myGLCD.drawLine(0,200,479, 200);
  myGLCD.drawLine(240,200,240, 319);*/
  myGLCD.setBackColor(200,200,150);
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);
  /*
  myGLCD.print("test",40,170);
  myGLCD.print("temperature",10,210);
  myGLCD.print("Humunity",250,210);
  delay(500);*/
 }
