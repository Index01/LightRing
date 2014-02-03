#include <Wire.h>

// I2C bus addresses (excludes the R/W bit)
#define ADDRESS 0b11000000
#define ALLCALL 0b1101000
#define RESET 0b1101011

// I2C R/W
#define READ 1
#define WRITE 0

// Control register (three MSB control auto-increment)
#define NO_AUTO_INCREMENT 0b00000000
#define AUTO_INCREMENT_ALL_REGISTERS 0b10100010
#define AUTO_INCREMENT_BRIGHTNESS 0b10100000
#define AUTO_INCREMENT_CONTROL 0b11000000
#define AUTO_INCREMENT_BRIGHT_CONTROL 0b11100000


void setup()
{
Wire.begin();
Serial.begin(9600);
delay(2000);

// Transmit to the TLC59116
Wire.beginTransmission(ALLCALL);
// Send the control register. All registers will be written to, starting at register 0
Wire.write(byte(AUTO_INCREMENT_ALL_REGISTERS));
// Set MODE1: no sub-addressing
Wire.write(byte(0));
// Set MODE2: dimming
Wire.write(byte(0));
// Set individual brightness control to maximum
for (int i=0; i< 16; i++)
Wire.write(byte(0x7F));
// Set GRPPWM: Full brightness
Wire.write(byte(0xFF));
// Set GRPFREQ: Not blinking, must be 0
Wire.write(byte(0b0));
// Set LEDs off for now
for (int i=0; i< 4; i++)
Wire.write(byte(0xFF));
// Set the I2C all-call and sub addresses (if needed)
delay(2000);
Serial.print("Setup: ");
Serial.println(Wire.endTransmission());

}

void readAll() {
  
  Serial.print("reade dis shit: ");
  Serial.println(Wire.requestFrom(ALLCALL, 8));    // request 6 bytes from slave device #2
  
  while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read();    // receive a byte as character
    Serial.print("readVal: ");
    Serial.println(c);         // print the character
  }
  delay(500);
}


void writeAll() {

  byte retVal= 0;
  String outVal= "";

  if (retVal==0){
    outVal=String("ACK");
  }
  else {
   outVal=String("NACK"); 
  }

//write to the driver  
  Serial.print("Driver address sent: ");
  Serial.println(byte(ALLCALL));
  Wire.beginTransmission(ALLCALL);
//  retVal=Wire.endTransmission();
  Serial.println(outVal);
  
//control register
  Wire.write(byte(AUTO_INCREMENT_ALL_REGISTERS));
//  retVal=Wire.endTransmission();
  Serial.print("Control sent: ");
  Serial.println(AUTO_INCREMENT_ALL_REGISTERS);
  Serial.println(outVal);
  
//write all register birghtness values  
  for (int i=0; i< 16; i++){
    Wire.write(byte(0x7F));
    Serial.print("LED#: ");
    Serial.println(i);
//    retVal=Wire.endTransmission();
//    Serial.println(outVal);
  delay(200);

  }    
  delay(200);
  Wire.endTransmission(); 
}

void loop(){
  for(int Cycle=0; Cycle<2; Cycle++){
    writeAll();  
  }
  delay(100);
  readAll(); 
}
