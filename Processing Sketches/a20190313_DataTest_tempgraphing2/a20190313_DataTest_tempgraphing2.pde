// on the basis of serial inputs that contain the analog value of the temperature and end in a \n

 import processing.serial.*;
 PFont f;
 PFont F;
 Serial myPort;        // The serial port
 int xPos = 40;         // horizontal position of the graph
 
 void setup () {
 // set the window size:  and Font size
 f = createFont("Arial",12,true);
 F = createFont("Arial",24,true);
 size(700, 600);        
 
 // List all the available serial ports
 println(Serial.list());
 myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n');
 // set inital background:
 background(70);
 }
 void draw () 
 {
   // everything happens in the serialEvent()
 }
 
 void serialEvent (Serial myPort) {
 // get the ASCII string:
 String inString = myPort.readStringUntil('\n');
 
 if (inString != null) {
 // trim off any whitespace:
 
 print("Temperature: ");
 print(inString);
 println("Degree Celcius");
 inString = trim(inString);
 
 // convert to an int and map to the screen height:
 float inByte = float(inString+(char)9); 
 inByte = map(inByte, 0,117, 0, height);
 
 println(inByte);
 
  stroke(175);                       // temperature line
  line(40,height-40,40,0);
  
  stroke(175);                          // Time line
  line(40,height-40,width,height-40);
  
  stroke(100,100,255);                          // 30 degree line
  line(40,height-194,width,height-194);
 
 stroke(100,100,255);                          // 60 degree line
  line(40,height-344,width,height-344);
 
  textFont(F);       
  fill(255);
 
 textAlign(RIGHT);
  text("Temperature Plot Using Proccessing",680,40); 
  
  textAlign(RIGHT);
  text("And Arduino By Saddam Khan",653,70); 
  
  textAlign(RIGHT);
  text("Engineers Garage",580,100); 
 
   textAlign(RIGHT);
  text("TEMP",70,40);                         
  
  textAlign(RIGHT);
  text("TIME --->",650,580);    
  
  
  
   textAlign(RIGHT);
  text(inString,500,200);
 
   textAlign(RIGHT);
  text(" Degree Celsuis",560,230);
  
  
  fill(0);
 // int j;
  stroke(255);   
  for(int j=500;j>430;j--)
  {
  line(j,height-398,j,height-425);
  }
  stroke(0,0,0);
  textAlign(RIGHT);
  text(inString,495,200); 
 
  
 fill(240);
 textFont(f); 
 
  textAlign(RIGHT);
  text("(In Degree)",140,40); 
  
  textAlign(RIGHT);                 // 100 degree
  text("100 -",40,60);
  
   textAlign(RIGHT);                // 90 degree
  text("90 -",40,110);
  
   textAlign(RIGHT);                // 80 degree
  text("80 -",40,160);
  
   textAlign(RIGHT);                 // 70 degree
  text("70 -",40,210);
  
   textAlign(RIGHT);                // 60 degree
  text("60 -",40,260);
  
   textAlign(RIGHT);               // 50 degree
  text("50 -",40,310);
  
   textAlign(RIGHT);                 // 40 degree
  text("40 -",40,360);
  
   textAlign(RIGHT);
  text("30 -",40,410);
  
   textAlign(RIGHT);
  text("20 -",40,460);
  
   textAlign(RIGHT);
  text("10 -",40,510);
  
  textAlign(RIGHT);
  text("0 -",40,560);
 
 /*---- scale between 30 degree to 40 degree------*/
  
  textAlign(RIGHT);
  text("   -",40,370);
  
  textAlign(RIGHT);
  text("   -",40,380);
  
  textAlign(RIGHT);
  text("   -",40,390);
  
  textAlign(RIGHT);
  text("   -",40,400);
  
 // textAlign(RIGHT);
 // text("0 -",40,360);
 
 // draw the line:
 int shift=40;            // set trace origin
 stroke(255,0,0);              // trace colour
 for(int i=0;i<2;i++)
 {
// line(xPos, height-inByte-1, xPos, height - inByte);
   line(xPos, height-inByte-(shift+2), xPos, height-inByte-shift);
   xPos++;
 }
 if (xPos >= width)         //  go back to begining
 {
 xPos = 40;
 background(100); 
 } 
 }
 }
