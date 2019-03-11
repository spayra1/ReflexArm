I2C1.setup( {scl: B6, sda: B7} );

var eeprom=require("AT24").connect(I2C1,64,128,0x50); //I2C2, 64 byte pages, 128 (16 kbyte) kbit capacity, with the A1 pin high, and A0 and A2 low. 


var a;
var l=eeprom.write(0x64,"digitalWrite(LED1,!a);a=!a;digitalWrite(LED2,!a);"); //write a bit of JS to the EEPROM at position 100

eval(E.toString(eeprom.read(0x64,l))); //read it back and run the code

var interval=setInterval("eval(E.toString(eeprom.read(0x64,l)));",333); 
