// modification of SerialTest_smoothpullback2
// last edited 3/13/19


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
String gripper = "#5P";





void draw() {
  delay(8000);
    // take note that because of the sensor on the wrist, wrist must stay locked at 90
    String[] commandgroup1 = {baserotation, randPos(10,170), shoulder, randPos(90,170), elbow, randPos(90,170), wrist, servomap(90), wristrotation, servomap(90)};
    execute(commandgroup1);
    delay(1000);
}

String randPos(int low, int high) {
  return servomap(int(random(low, high)));
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
