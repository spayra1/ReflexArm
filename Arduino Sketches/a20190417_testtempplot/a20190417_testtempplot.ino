#include <Wire.h>
#include <EEPROM.h>

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

void setup() {
 Wire.begin();                                 // connect I2C
 Serial.begin(115200);
 Wire.setClock(400000);
 configTemp();
 Serial.println("I2C Temperature Sensor (MAX31875)");
 Serial.println("I2C Temperature Sensor with External EEPROM Memory");
 EEPROMAddress = 0;
 dataSize = 2;                                // integer - 2 bytes
 
}





void loop() {
  int temperature_raw;
  float temperature_C1;
  
  /* UNCOMMENT BELOW AND COMMENT OUT NEXT BLOCK TO WRITE */
  temperature_raw = readTemp();
  //Serial.print("Temperature in raw: ");
  //Serial.println(temperature_raw);
  
  if(temperature_raw != 0) {
    temperature_C1 = temperature_raw/256.0;
    int temperature_C=round(temperature_C1*100);
    //Serial.print("Temperature in Celsius: ");
    Serial.println(temperature_C);
    convertAndWrite(temperature_C);//
  }
  else {
    Serial.println("Temp Read Error ");
  }
  /* END WRITE BLOCK */
  

   /* UNCOMMENT BELOW AND COMMENT OUT ABOVE BLOCK TO READ */
  /*Serial.print("Read from address ");
  Serial.print(EEPROMAddress);
  Serial.print(": ");
  Serial.println(readFromEEPROM(EEPROMAddress));
  if (EEPROMAddress == EEPROM.length()) {
    Serial.println("WRAP");
    EEPROMAddress = 0;
  }
  /* END READ BLOCK */
  
  
  EEPROMAddress += dataSize;
  delay(20);  // 1000 milliseconds delay
}








void configTemp()
{
  // creates an array to store the values to be written to the configuration register
  // values chosen will program the MAX31875 for 8 conversions/second and 12-bit resolution (.0625?)
  Wire.beginTransmission(SLAVE_ID);
  Wire.write(CONFIG);
  Wire.write((int)(102 >> 8));
  Wire.write((int)(102 & 0xFF));
  Wire.endTransmission();
  delay(20);
}





int readTemp()
{
  int temp_raw = 0;
  
  // the first I2C command sets the MAX31875 to point to the temperature register
  Wire.beginTransmission(SLAVE_ID);
  Wire.write(TEMP);
//  Wire.write(0);
  Wire.endTransmission();
  
  //wait for temperature readings to happen
  delay(10);
  
  // request temperature reading from sensor
  Wire.requestFrom(SLAVE_ID, 2);
  
  // receive temperature reading from sensor
  if (Wire.available() >= 2) {
    byte first_byte = Wire.read();
    byte second_byte = Wire.read();
    //Serial.print("First byte: ");
    //Serial.println(first_byte);
    //Serial.print("Second byte: ");
    //Serial.println(second_byte);
    temp_raw = word(first_byte, second_byte);
  }

  return temp_raw;
}


void convertAndWrite(int temp) {
  writeToEEPROM(temp); // fTemp for fahrenheit, temp for celsius
//  Serial.print(temp);
//  Serial.print(" / ");
//  Serial.println(fTemp);
}



void writeToEEPROM(int value) {
  Wire.beginTransmission(MEM_DEV_ADDR);
  // Write address to start at
  Wire.write((int)(EEPROMAddress >> 8)); // MSB
  Wire.write((int)(EEPROMAddress & 0xFF)); // LSB
  
  Wire.write(highByte(value));
  Wire.write(lowByte(value));
  Wire.endTransmission();
  
  //Serial.print("Value ");
  //Serial.print(value);
  //Serial.print(" written to Arduino EEPROM at address ");
  //Serial.println(EEPROMAddress);
}





int readFromEEPROM(int EEPROMAddress) {
  Wire.beginTransmission(MEM_DEV_ADDR);
  // Write address to start at
  Wire.write((int)(EEPROMAddress >> 8)); // MSB
  Wire.write((int)(EEPROMAddress & 0xFF)); // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(MEM_DEV_ADDR, dataSize); // Request 2 bytes
  byte a = Wire.read();
  byte b = Wire.read();
  int temp = word(a, b);  // Converts 2 bytes into int
  return temp;
}

 
