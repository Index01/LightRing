#include <Wire.h>

// I2C bus addresses (excludes the R/W bit)
#define ADDRESS 0b1100000
#define ALLCALL 0b1101000
#define RESET 0b1101011

// I2C R/W
#define READ 1
#define WRITE 0

// Control register (three MSB control auto-increment)
#define NO_AUTO_INCREMENT 0b00000000
#define AUTO_INCREMENT_ALL_REGISTERS 0b10000000
#define AUTO_INCREMENT_BRIGHTNESS 0b10100000
#define AUTO_INCREMENT_CONTROL 0b11000000
#define AUTO_INCREMENT_BRIGHT_CONTROL 0b11100000
#define READ_CONTROL 0b10000000
#define AUTO_INCREMENT_MULTIWRITE 0b10100010


// SETUP CONFIGURATION //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
Wire.begin();
Serial.begin(9600);
delay(2000);

// read setup registers
delay(1000);
readAll(ADDRESS + 0b1);
delay(2000);

}

// START CONDITION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startCondition(byte addy) {
  byte retVal= 1;
  String outVal= "";
  if (retVal==0){
    outVal=String("ACK");
  }
  else {
   outVal=String("NACK"); 
  }

//write to the driver  
  Serial.print("Driver address sent: ");
  Serial.println(byte(addy));
  Wire.beginTransmission(addy);
//  retVal=Wire.endTransmission();
  Serial.println(outVal);   
}  

// CONTROL CONDITION REGISTER ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void controlConfig(byte controlVal) {
  byte retVal= 1;
  String outVal= "";

  if (retVal==0){
    outVal=String("ACK");
  }
  else {
   outVal=String("NACK"); 
  }

//control register
  Wire.write(byte(controlVal));
  retVal=Wire.endTransmission();
  Serial.print("Control sent: ");
  Serial.println(controlVal);
  Serial.println(outVal);
   
}

// READ FUNCTION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void readAll(byte reqAddy) {
  byte inAddress = byte(reqAddy);
  int regNum= 0;  
    
  const char* registerName[][30] = 
    {
    "MODE1 ", "MODE2 ", "LEDPWM0 ", "LEDPWM1 ", "LEDPWM2 ", 
    "LEDPWM3 ", "LEDPWM4 ", "LEDPWM5 ", "LEDPWM6 ", "LEDPWM7 ", 
    "LEDPWM8 ", "LEDPWM9 ", "LEDPWM10 ", "LEDPWM11 ", "LEDPWM12 ", 
    "LEDPWM13 ", "LEDPWM14 ", "LEDPWM15 ", "GRPPWM ", "GRPFREQ ", 
    "LEDOUT0 ", "LEDOUT1", "SUBADDR1 ", "SUBADDR2 ", "SUBADDR3 ", 
    "ALLCALLADR ", "IREF ", "EFLAG1 ", "EFLAG2 "
    };

  Serial.print("Read address sent: ");
  Serial.println(inAddress, BIN); 

    Wire.requestFrom(ADDRESS, 32);                         // request ,x bits from driver 

    for(int registerArrayIndex = 0; registerArrayIndex < sizeof(registerName)/sizeof(registerName[0]); registerArrayIndex++){
      Serial.println(registerArrayIndex);
      const char** arrayValue = registerName[registerArrayIndex];
      for(int arrayPointerRef=0; arrayValue[arrayPointerRef]; arrayPointerRef++) {
        if(arrayValue[arrayPointerRef]== NULL) {
          break;
        }
        else {
          byte c = Wire.read();                           // receive a byte 
          String hexStrOut = String(regNum, HEX);
          Serial.print("Reg num: ");                
          Serial.print(arrayValue[arrayPointerRef]);       
          Serial.println(c, BIN);                      
            
          regNum++;
        }
      }
    }
    delay(100);
}


// WRITE FUNCTION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: make write function more reliable, get some pointer action going. 
void writeAll(byte setPWMVal) {
  byte inVal = byte(setPWMVal);   
  byte retVal= 0;
  String outVal= "";
  
  if (retVal==0){
    outVal=String("ACK");
  }
  else {
   outVal=String("NACK"); 
  }

// set mode1/2
  byte mode1= 0b10000001;
  Wire.write(mode1);
  Serial.print("mode1: ");
  Serial.println(mode1);

  byte mode2= 0b10000001;
  Wire.write(mode2);
  Serial.print("mode2: ");
  Serial.println(+mode2);
  
//  byte mode3= 0b10000001;
//  Wire.write(mode3);
//  Serial.print("mode3: ");
//  Serial.println(+mode3);
//  
//  byte mode4= 0b00000000;
//  Wire.write(mode4);
//  Serial.print("mode4: ");
//  Serial.println(mode4);

    for (int regNum=0; regNum< 16; regNum++){
      Wire.write(inVal);
      Serial.print("LED#: ");
      Serial.print(regNum);
      Serial.print(" ");
      Serial.println(inVal);
//      delay(100);

    }  

    byte GRPPWN= 0xFF;
    Wire.write(GRPPWN);
    Serial.print("GRPPWN: ");
    Serial.println(GRPPWN);
    
    byte GRPFREQ= 0xFF;
    Wire.write(GRPFREQ);
    Serial.print("GRPFREQ: ");
    Serial.println(GRPFREQ);

    byte LEDOUT0= 0xFF;
    Wire.write(LEDOUT0);
    Serial.print("LEDOUT0: ");
    Serial.println(LEDOUT0);
    
    byte LEDOUT1= 0xFF;
    Wire.write(LEDOUT1);
    Serial.print("LEDOUT1: ");
    Serial.println(LEDOUT1);

    byte SUBADDR1= 0xFF;
    Wire.write(SUBADDR1);
    Serial.print("SUBADDR1: ");
    Serial.println(SUBADDR1);

    byte SUBADDR2= 0xFF;
    Wire.write(SUBADDR2);
    Serial.print("SUBADDR2: ");
    Serial.println(SUBADDR2);
    
    byte SUBADDR3= 0xFF;
    Wire.write(SUBADDR3);
    Serial.print("SUBADDR3: ");
    Serial.println(SUBADDR3);

    byte ALLCALLreg= 0xFF;
    Wire.write(ALLCALLreg);
    Serial.print("ALLCALL: ");
    Serial.println(ALLCALLreg);
    
    byte IREF= 0xFF;
    Wire.write(IREF);
    Serial.print("IREF: ");
    Serial.println(IREF);

  
    Wire.endTransmission(); 

}

// MAIN LOOP //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

//Write registers  
  for(int PWMRegistersValues=0b00000000; PWMRegistersValues<0b11111111; PWMRegistersValues++){
    startCondition(ALLCALL + WRITE);
    controlConfig(AUTO_INCREMENT_ALL_REGISTERS);    
    writeAll(PWMRegistersValues);  
   
   
//Read back registers
    startCondition(ADDRESS + WRITE);
    controlConfig(READ_CONTROL);
    startCondition(ADDRESS + READ);
    readAll(ADDRESS); 

    delay(100);
  }
  
  for(int c=0; c<10; c++){
    delay(10);
  }
}
