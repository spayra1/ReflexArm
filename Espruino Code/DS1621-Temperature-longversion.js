DS1621 (Large sensor) Temperature sensor: (Longer version). This sensor is a test sensor and not the one incorporated in the fiber.
(Reading low resolution temperature)

I2C1.setup( {scl: B6, sda: B7} );


var C = {
    I2CADDR_BASE: 0x4f,
    REG_CONFIG: 0xAC, 
    RD_TEMP:0xAA,
    REG_T_UPPER: 0xA1, 
    REG_T_LOWER: 0xA2,
    REG_TEMPERATURE: 0xAA,
};


function DS1621( i2c, a2a1a0Pins ) {
    this.i2c = i2c;
    this.i2cAddress = 0x4f ;
    console.log(0x4f);
}

//****SETUP***//

//Stop conversion

function stopconver(){
    this.i2c.writeTo( this.i2cAddress, 0x22 );
}
  
//Initialized config
function config(){
    write8(0xAC,0x02);
}

//startconfiguration
  
function startconver(){
    this.i2c.writeTo( this.i2cAddress, 0xEE );
}
  
//***END of SETUP//



//***RETURN TEMPERATURE in °C ***//
var x=1;
function getTemperature(value) {
  if (x==1){
  this.i2c.writeTo( this.i2cAddress, 0xEE );//activate temperature sensor by uncommenting this statement
  }
  x=0;
  console.log("The temp register is"+" "+ value);
  return read8(value);
  
}



//** Conversion

function rawToCelsius ( raw ) {
    return raw;
  //& 0x1000 ? (raw & 0xefff) / 16.0 - 256 : raw / 16.0;
}




//**Write and read functions

// Read 1 byte from register reg
function read8 (reg) {
    this.i2c.writeTo( this.i2cAddress, reg);
    return this.i2c.readFrom( this.i2cAddress, 1 )[0];
}

// Read two  bytes from register reg and combine to unsigned integer
function read16 (reg) {
    this.i2c.writeTo( this.i2cAddress, reg );
    var array = this.i2c.readFrom( this.i2cAddress, 2 );
    console.log(array);
    return (array[0] << 8)| array[1];
}

// Write one byte (value) to register reg
function write8 ( reg, value ) {
    this.i2c.writeTo( this.i2cAddress, [reg, value] );
}

// Write an unsigned integer value (two bytes) to register reg
function write16 ( reg, value ) {
    this.i2c.writeTo( this.i2cAddress, [reg, value >> 8, value & 0xff] );
}

DS1621(I2C1, 0x4f); 

function tempread(){
  var Temperature= getTemperature(0xAA);
  console.log("Temperature: " + Temperature + " C");
}  

setInterval(tempread, 500);
