function AT24(i2c, pgsz, cap, i2ca) {
  this.i2c = i2c;
  this.i2ca = (i2ca===undefined) ? 0x50 : 0x50|(i2ca&0x07);
  this.pgsz=pgsz?pgsz:0;
  this.cap=cap<<7;
  this.ca=0;
}

AT24.prototype.a=function(b) {return (this.cap>2048)?E.toString([b>>8,b]):E.toString(b);}; //

AT24.prototype.i=function(a) {
  return this.i2ca|(a>>((this.cap>2048)?16:8));
};

AT24.prototype.read= function(add,bytes) {
	if (add!==undefined) {
		this.i2c.writeTo(this.i(add),this.a(add));
	}
	var ov=this.i2c.readFrom(this.i(add),bytes);
	return ov;
	
};
