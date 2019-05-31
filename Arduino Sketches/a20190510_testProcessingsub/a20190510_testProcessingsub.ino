#include <Wire.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

/* BEGIN TEMP DEVICE DEFINITIONS */

// ID and address

#define DEV_TYPE   0x90 >> 1                    // shift required by wire.h
                                                // TWI takes in a 7-bit address and
                                                // ORs a R/W bit after left-shifting
#define TEMP_DEV_ADDR   0x48                    // address
#define SLAVE_ID   TEMP_DEV_ADDR

// Registers & Commands

#define TEMP    0x00                         
#define CONFIG  0x01                         // configuration settings
#define T_HYST  0x02                         // temperature hysteresis (less than or equal to T_OS)
#define T_OS    0x03                         // temperature threshold

// MAX31875 configuration bits

#define OVER                    B1000000000000000     // (READ-ONLY) controlled by Comp/Int - overtemperature value exceeded
#define FAULT_QUEUE_1           B0001000000000000     // consecutive overtemperature fault threshold (left bit)
#define FAULT_QUEUE_2           B0000100000000000     // consecutive overtemperature fault threshold (right bit)
#define COMP_INT                B0000001000000000     // 0 = comparator mode; 1 = interrupt mode
#define SHUTDOWN                B0000000100000000     // 1 = enter shutdown mode; 0 = continuous conversion
#define DATA_FORMAT             B0000000010000000     // 1 = extended temperature format
#define RESOLUTION_1            B0000000001000000     // conversion resolution (left bit)
#define RESOLUTION_2            B0000000000100000     // conversion resolution (right bit)
#define TIME_OUT                B0000000000010000     // 1 = disable bus timeout; 0 = enable bus timeout
#define PEC                     B0000000000001000     // packet error checking (PEC byte appended if enabled)
#define CONVERSION_RATE_1       B0000000000000100     // conversion rate (left bit)
#define CONVERSATION_RATE_2     B0000000000000010     // conversion rate (right bit)
#define ONE_SHOT                B0000000000000001     // 1 = one conversion; 0 = continuous conversion

/* BEGIN EEPROM DEVICE DEFINITIONS */

#define MEM_DEV_ADDR  0x52                      // address is 000

long EEPROMAddress;
int dataSize;

/* data holder for smoothing temperature data */
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

/* data holders for derivative calculation */
int previous = 0;
int current;
int change;

// Arduino pinouts
int flagpin = 2;
int TX = 2;
int RX = 3;
unsigned long currentTime;
unsigned long previousTime = 0;
int motionInterval = 2000;

SoftwareSerial SSC(RX, TX);

void setup() {
 Wire.begin();                                 // connect I2C
 Serial.begin(115200);
 Wire.setClock(400000);
 SSC.begin(9600);
 SSC.listen();
 configTemp();
 Serial.println("I2C Temperature Sensor (MAX31875)");
 Serial.println("I2C Temperature Sensor with External EEPROM Memory");
 EEPROMAddress = 0;
 dataSize = 2;                                // integer - 2 bytes
 pinMode(flagpin, OUTPUT);
}

void configTemp()
{
  // creates an array to store the values to be written to the configuration register
  // values chosen will program the MAX31875 for 8 conversions/second and 12-bit resolution (.0625?)
  Wire.beginTransmission(SLAVE_ID); Wire.write(CONFIG); Wire.write((int)(102 >> 8)); Wire.write((int)(102 & 0xFF)); Wire.endTransmission();
  delay(20);
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


String leftretracted[] = {baserotation, servomap(160), shoulder, servomap(160), elbow, servomap(160)};
String leftquarter[] = {baserotation, servomap(120), shoulder, servomap(90), elbow, servomap(90)};
String midfullextend[] = {baserotation, servomap(90), shoulder, servomap(20), elbow, servomap(10)};
String rightquarter[] = {baserotation, servomap(50), shoulder, servomap(90), elbow, servomap(90)};
String rightretracted[] = {baserotation, servomap(20), shoulder, servomap(160), elbow, servomap(160)};
  


// position reference
String left = "500";
String center = "1500";
String right = "2500";

String servomap(int degree) {
  return String(int(map(degree, 0, 180, 500, 2500)));
}




int readTemp()
{
  int temp_raw = 0;
  float temperature_C1;
  
  // the first I2C command sets the MAX31875 to point to the temperature register
  Wire.beginTransmission(SLAVE_ID); Wire.write(TEMP); Wire.endTransmission();
  //wait for temperature readings to happen
  delay(10);
  // request temperature reading from sensor
  Wire.requestFrom(SLAVE_ID, 2);
  // receive temperature reading from sensor
  if (Wire.available() >= 2) {byte first_byte = Wire.read(); byte second_byte = Wire.read(); temp_raw = word(first_byte, second_byte);}
  
  if(temp_raw !=0) {
    temperature_C1 = temp_raw/256.0;
    int temperature_C=round(temperature_C1*100);
    convertAndWrite(temperature_C);
    // Temperature in Celsius:
    current = temperature_C;
    change = current - previous;
    // Serial.println(smoothvalues(change));

    return(smoothvalues(temperature_C));    
  }
}


void convertAndWrite(int temp) {
  writeToEEPROM(temp); // fTemp for fahrenheit, temp for celsius
}



void writeToEEPROM(int value) {
  Wire.beginTransmission(MEM_DEV_ADDR);
  // Write address to start at
  Wire.write((int)(EEPROMAddress >> 8)); // MSB
  Wire.write((int)(EEPROMAddress & 0xFF)); // LSB
  
  Wire.write(highByte(value));
  Wire.write(lowByte(value));
  Wire.endTransmission();
}



int readFromEEPROM(int EEPROMAddress) {
  Wire.beginTransmission(MEM_DEV_ADDR);
  // Write address to start at
  Wire.write((int)(EEPROMAddress >> 8)); // MSB
  Wire.write((int)(EEPROMAddress & 0xFF)); // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(MEM_DEV_ADDR, dataSize); // Request 2 bytes
  byte a = Wire.read(); byte b = Wire.read();
  int temp = word(a, b);  // Converts 2 bytes into int
  return temp;
}

int smoothvalues(int newesttemp) {
  // subtract the last reading:
  total = total - readings[readIndex];
  
  // read from the sensor:
  readings[readIndex] = newesttemp;
  
  // add the reading to the total:
  total = total + readings[readIndex];
  
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate and return the average:
  average = total / numReadings;
  return(average);
}





char commandConcatenate(String commandgroup[]) {
  String output = "";
  for(int i=0; i<6; i++){
    output += commandgroup[i];
  }
  output += "#3P1500#4P1500";
  output += "T2000";
  output += "\r";

  char charconversion[output.length()];
  output.toCharArray(charconversion, output.length());
  Serial.println(charconversion);
  return charconversion;
}




void loop() {
  // check temperature condition
  int temperature;
  temperature = readTemp();
  Serial.print("temp is ");
  Serial.print(temperature);
  if (temperature <= 2700) {
    Serial.println();
    digitalWrite(flagpin, LOW);

    // stay extended for now
    SSC.write(commandConcatenate(midfullextend));

    // do normal motion patterns here
//    String commandgroup[8][6] = {leftretracted, leftquarter, midfullextend, rightquarter, rightretracted, rightquarter, midfullextend, leftquarter};
//    // iterates through the array array and executes each one at a time, pausing 2 seconds in between
//    for(int i=0; i<8; i++) {
//      
//      //Serial.println("2 seconds passed, now moving");
//      SSC.write(commandConcatenate(commandgroup[i]));
//      //delay(2000);

        currentTime = millis();
        if(currentTime-previousTime>motionInterval){
            //do motion
            previousTime = currentTime;
        }
  } else {
    Serial.println(", above 27C, writing HIGH");
    digitalWrite(flagpin, HIGH);

    // do pullback motion here
    String midretracted[] = {baserotation, servomap(90), shoulder, servomap(160), elbow, servomap(160)};
    SSC.write(commandConcatenate(midretracted));
  }
  
  EEPROMAddress += dataSize;
  delay(20);  // 1000 milliseconds delay
}
