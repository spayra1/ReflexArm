// modification of randommotion
// last edited 4/15/19

// the purpose of this script is to test out different positions to establish limits for the randommotion script

import processing.serial.*;

Serial SSC; // serial port for the SSC-32U

void setup() {
  // Print a list of the serial ports, for debugging purposes:
  printArray(Serial.list());
  String portnumber = Serial.list()[0];
  SSC = new Serial(this, portnumber, 9600);
  delay(1000);
}

// codes to refer to the servos
// "#<ID>" refers to servo <ID>, "P" tells it to go to position <following int>
String baserotation = "#0P";
String shoulder = "#1P";
String elbow = "#2P";
String wrist = "#3P";
String wristrotation = "#4P";
String gripper = "#5P";


//0 positions:
//baserotation: servo left (head-on right)
//shoulder: full forward
//elbow: full extension

//90 positions:
//baserotation: straight forward
//shoulder: vertically up
//elbow: 90 from shoulder

//180 positions:
//baserotation: servo right (head-on left)
//shoulder: full backward
//elbow: full flexion (curled against lower arm)


void draw() {
  // take note that because of the sensor on the wrist, wrist must stay locked at 90
  String[] leftretracted = {baserotation, servomap(160), shoulder, servomap(160), elbow, servomap(160)};
  String[] leftquarter = {baserotation, servomap(120), shoulder, servomap(90), elbow, servomap(90)};
  String[] midfullextend = {baserotation, servomap(90), shoulder, servomap(20), elbow, servomap(10)};
  String[] rightquarter = {baserotation, servomap(50), shoulder, servomap(90), elbow, servomap(90)};
  String[] rightretracted = {baserotation, servomap(20), shoulder, servomap(160), elbow, servomap(160)};
  
  // array of the arrays above
  String[][] commandgroup = {leftretracted, leftquarter, midfullextend, rightquarter, rightretracted, rightquarter, midfullextend, leftquarter};
  
  // iterates through the array array and executes each one at a time, pausing 2 seconds in between
  for(String[] command : commandgroup) {
    execute(command);
    delay(2000);
  }

}

// position reference
String left = "500";
String center = "1500";
String right = "2500";
String servomap(int degree) {
  return str(int(map(degree, 0, 180, 500, 2500)));
}

String output;
String commandConcatenate(String[] commandgroup) {
  output = "";
  for(String command : commandgroup){
    output += command;
  }
  output += "#3P1500#4P1500";
  output += "T2000";
  output += "\r";
  return output;
}

void execute(String[] commandgroup) {
  SSC.write(commandConcatenate(commandgroup));
}
