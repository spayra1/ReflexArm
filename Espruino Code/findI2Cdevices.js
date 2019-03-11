function go() {
  function scanI2c( i2c, first, last ) {
   if (typeof first === "undefined") {
        first = 0x03;
    }
    if (typeof (last) === "undefined") {
        last = 0x77;
    }
    print( "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f" );
    for (var upper = 0; upper < 8; ++upper) {
        var line = upper + "0: ";
        for (var lower = 0; lower < 16; ++lower) {
            var address = (upper << 4) + lower;
            // Skip unwanted addresses
            if ((address < first) || (address > last)) {
                line += "   ";
                continue;
            }
            try {
                i2c.readFrom( address, 1 );
                line += (address + 0x100).toString( 16 ).substr( -2 );
                line += " ";
            } catch (err) {
                line += "-- ";
            }
        }
        print( line );
    }
}
// Usage
I2C1.setup( {scl: B6, sda: B7} );
scanI2c( I2C1, 0x45, 0x55 );
}

setTimeout(go, 100);
