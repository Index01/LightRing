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
//
//const char* registerName[][30] = 
//  {
//    "MODE1 ", "MODE2 ", "LEDPWM0 ", "LEDPWM1 ", "LEDPWM2 ", 
//    "LEDPWM3 ", "LEDPWM4 ", "LEDPWM5 ", "LEDPWM6 ", "LEDPWM7 ", 
//    "LEDPWM8 ", "LEDPWM9 ", "LEDPWM10 ", "LEDPWM11 ", "LEDPWM12 ", 
//    "LEDPWM13 ", "LEDPWM14 ", "LEDPWM15 ", "GRPPWM ", "GRPFREQ ", 
//    "LEDOUT0 ", "LEDOUT1", "SUBADDR1 ", "SUBADDR2 ", "SUBADDR3 ", 
//    "ALLCALLADR ", "IREF ", "EFLAG1 ", "EFLAG2 "
//  };
//
//for(int registerArrayIndex = 0; registerArrayIndex < sizeof(registerName)/sizeof(registerName[0]); registerArrayIndex++){
//  Serial.println(registerArrayIndex);
//  const char** arrayValue = registerName[registerArrayIndex];
//  for(int arrayPointerRef=0; arrayValue[arrayPointerRef]; arrayPointerRef++){
//    if(arrayValue[arrayPointerRef]== NULL){
//      break; 
//    }
//    else {
//      Serial.println(arrayValue[arrayPointerRef]);
//      
//    }
//  }
//}

// read setup registers
delay(1000);
readAll(ADDRESS + 0b1);
delay(2000);

}
// START CONDITION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startCondition(byte addy) {
  byte retVal= 0;
  String outVal= "1";
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
  byte retVal= 0;
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

// READ FUNCTION ////////////////////////////////////////////////////////////
void readAll(byte reqAddy) {
  Serial.print("Read address sent: ");
  Serial.println(ADDRESS, BIN);  // request ,x bits from slave device #2

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

    Wire.requestFrom(ADDRESS, 64);

    for(int registerArrayIndex = 0; registerArrayIndex < sizeof(registerName)/sizeof(registerName[0]); registerArrayIndex++){
      Serial.println(registerArrayIndex);
      const char** arrayValue = registerName[registerArrayIndex];
      for(int arrayPointerRef=0; arrayValue[arrayPointerRef]; arrayPointerRef++){
        if(arrayValue[arrayPointerRef]== NULL){
          break; 
          
        }
        else {
//            while(Wire.available())    // slave may send less than requested
//              { 

                byte c = Wire.read();    // receive a byte 
                String hexStrOut = String(regNum, HEX);
//              String binStrOut = String(c, BIN);

                Serial.print("Reg num: ");                
                Serial.print(arrayValue[arrayPointerRef]);      
                Serial.println(c, BIN);    // print the character        
            
                regNum++;
   
//              }
        }
      }
    }
  
    
//    switch(int(regNum)){
////    if(hexStrOut=="1b"){
//      case int(0x0):
//        Serial.print("MODE1: ");
//        Serial.println(c, BIN);    // print the character        
//        break;
//        
//      case int(0x1):
//        Serial.print("MODE2: ");
//        Serial.println(c, BIN);    // print the character
//        break;
//
//      case int(0x18):
//        Serial.print("SUBADR1: ");
//        Serial.println(c, BIN);    // print the character        
//        break;
//        
//      case int(0x19):
//        Serial.print("SUBADR2: ");
//        Serial.println(c, BIN);    // print the character
//        break;
//
//      case int(0x1A):
//        Serial.print("SUBADR3: ");
//        Serial.println(c, BIN);    // print the character  
//        break;
//
//      case int(0x1B):
//        Serial.print("ALLCALLADR: ");
//        Serial.println(c, BIN);    // print the character
//        break;
//
//    
//      default:
//        Serial.print("0x");
//        Serial.print(regNum, HEX);
//        Serial.print(" ReadVal: ");
//        Serial.println(c, BIN);    // print the character
//    }
    

  delay(100);
}

// WRITE FUNCTION ///////////////////////////////////////////////////////////////
void writeAll(byte setVal) {
  byte inVal = byte(setVal);   
  byte retVal= 0;
  String outVal= "";
  
  if (retVal==0){
    outVal=String("ACK");
  }
  else {
   outVal=String("NACK"); 
  }

// try to set mode1/2
//  byte mode1= AUTO_INCREMENT_ALL_REGISTERS;
//  Wire.write(mode1);
//  Serial.print("mode1: ");
//  Serial.println(mode1);

  byte mode2= 0b10000001;
  Wire.write(mode2);
  Serial.print("mode2: ");
  Serial.println(+mode2);
  
  byte mode3= 0b10000001;
  Wire.write(mode3);
  Serial.print("mode3: ");
  Serial.println(+mode3);
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
//
//    byte ALLCALL= 0xFF;
//    Wire.write(ALLCALL);
//    Serial.print("ALLCALL: ");
//    Serial.println(ALLCALL);
//    
    byte IREF= 0xFF;
    Wire.write(IREF);
    Serial.print("IREF: ");
    Serial.println(IREF);

  
    Wire.endTransmission(); 
//    delay(100);

}

// MAIN LOOP /////////////////////////////////////////////////////////////////
void loop(){
  
  for(int Cycle=0b00000000; Cycle<0b11111111; Cycle++){
    startCondition(ALLCALL + 0B0);
 //   controlConfig(AUTO_INCREMENT_ALL_REGISTERS);    
    writeAll(Cycle);  
   
//   delay(100);
   
    startCondition(ADDRESS + 0B0);
    controlConfig(READ_CONTROL);
    Wire.endTransmission();
    startCondition(ADDRESS + 0B1);
    Wire.endTransmission();
    readAll(ADDRESS); 
//    Wire.endTransmission();
  
  delay(100);
  }
  
  for(int c=0; c<10; c++){
    delay(10);
  }
}
