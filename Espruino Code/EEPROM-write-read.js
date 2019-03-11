I2C1.setup( {scl: B6, sda: B7} );

var eeprom=require("AT24").connect(I2C1,64,128,0x50); //I2C2, 64 byte pages, 128 (16 kbyte) kbit capacity, with the A1 pin high, and A0 and A2 low. 

//console.log(E.toString(eeprom.read(0,25))); //Read the beginning of the EEPROM. 

eeprom.write(0x0400,[12,23,240,192,245]);

// then type in interface:
/**
	console.log(eeprom.read(0x0400,16))
**/