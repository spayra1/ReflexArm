/** 
 * Serial Call-Response 
 * by Tom Igoe. 
 * 
 * Sends a byte out the serial port, and reads 3 bytes in. 
 * Sets foregound color, xpos, and ypos of a circle onstage
 * using the values returned from the serial port. 
 * Thanks to Daniel Shiffman  and Greg Shakar for the improvements.
 * 
 * Note: This sketch assumes that the device on the other end of the serial
 * port is going to send a single byte of value 65 (ASCII A) on startup.
 * The sketch waits for that byte, then sends an ASCII A whenever
 * it wants more data. 
 */
 

import processing.serial.*;

Serial SSC;                       // The serial port for the motor controller

void setup() {
  // Print a list of the serial ports, for debugging purposes:
  printArray(Serial.list());

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portnumber = Serial.list()[0];
  SSC = new Serial(this, portnumber, 9600);
}

// servos
String rotatebase = "#0P";
String shoulder = "#1P";
String elbow = "#2P";
String wrist = "#3P";
String wristrotate = "#4P";

// positions
String left = "500";
String center = "1500";
String right = "2500";

String movecommand;
String[] commandgroup = {elbow, "2000", shoulder, "2000"};
int i = 0;
void draw() {
  delay(3000);
  if(i<1){
    String command = commandConcatenate();
    SSC.write(command);
    System.out.println(command);
    i++;
  }
}

String output;
String commandConcatenate() {
  output = "";
  for(int i = 0; i<commandgroup.length; i++){
    output += commandgroup[i];
  }
  output += "T1000\r";
  return output;
}
