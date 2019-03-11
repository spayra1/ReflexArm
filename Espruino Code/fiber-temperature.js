I2C1.setup( {scl: B6, sda: B7} );
 this.i2c = I2C1;
 this.i2cAddress = 0x48 ;

//****SETUP***//

//Stop conversion

  
//Initialized config
function config(){
    write16(0x01,102);
}

function write8 ( reg, value ) {
    this.i2c.writeTo( this.i2cAddress, [reg, value] );
}


function write16 ( reg, value ) {
    this.i2c.writeTo( this.i2cAddress, [reg, value >> 8, value & 0xff] );
    console.log ([reg, value >> 8, value & 0xff]);
}

  
//***END of SETUP//

//***RETURN TEMPERATURE in °C ***//
var x=1;
function getTemperature(value) {
  console.log("The temp register is"+" "+ value);
  return read16(value);
}



//**Write and read functions

// Read 1 byte from register reg
function read8 (reg) {
    this.i2c.writeTo( this.i2cAddress, reg);
    return this.i2c.readFrom( this.i2cAddress, 1 )[0];
}

function read16 (reg) {
    this.i2c.writeTo( this.i2cAddress, reg );
    var array = this.i2c.readFrom( this.i2cAddress, 2 );
    console.log(array);
    return (array[0] << 8)| array[1];
}


config();

function tempread(){
  var Temperature= getTemperature(0x00)/256.00;
  console.log("Temperature: " + Temperature + " C");
}  

setInterval(tempread, 500);


