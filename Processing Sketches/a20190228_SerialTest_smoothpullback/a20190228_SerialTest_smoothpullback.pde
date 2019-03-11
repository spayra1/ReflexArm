// modification of SerialTest_SSC
// last edited 2/28/19

// DEBUG:
/***
see if this is able to implement smooth pullback
*///

import processing.serial.*;

Serial SSC; // serial port for the SSC-32U

void setup() {
  // Print a list of the serial ports, for debugging purposes:
  printArray(Serial.list());
  String portnumber = Serial.list()[0];
  SSC = new Serial(this, portnumber, 9600);
}

// codes to refer to the servos
// "#<ID>" refers to servo <ID>, "P" tells it to go to position <following int>
String baserotation = "#0P";
String shoulder = "#1P";
String elbow = "#2P";
String wrist = "#3P";
String wristrotation = "#4P";
String all = "all";

// position reference
String left = "500";
String center = "1500";
String right = "2500";

String movecommand;

int i = 0;


void draw() {
  delay(1000);
  if(i<1){ i++;
    // in this loop, put the commands to execute once 
    
    String[] commandgroup1 = {baserotation, servomap(90), shoulder, servomap(140), elbow, servomap(140), wrist, servomap(90), wristrotation, servomap(90)};
    String[] commandgroup2 = {shoulder, servomap(135)};
    execute(commandgroup1);
    delay(500);
    //execute(commandgroup2);
    
}}

String output;
String commandConcatenate(String[] commandgroup) {
  output = "";
  for(int i = 0; i<commandgroup.length; i++){
    output += commandgroup[i];
  }
  //output += "T1000";
  output += "\r";
  return output;
}

void execute(String[] commandgroup) {
  SSC.write(commandConcatenate(commandgroup));
}

String servomap(int degree) {
  return str(int(map(degree, 0, 180, 500, 2500)));
}
