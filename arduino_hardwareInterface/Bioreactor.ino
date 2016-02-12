#include <OneWire.h>

int temperatureSensor1Pin = 7;

int muxSelectorPin0 = 8;
int muxSelectorPin1 = 9;
int muxSelectorPin2 = 10;
int muxSelectorPin3 = 11;

int muxSignalPin = A0;

int powerRelay1Pin = 12;
int powerRelay2Pin = 13;
boolean powerRelay1On = false;
boolean powerRelay2On = false;
int lightSensorReadings[10] = {};
float temperatureReadings[10] = {};
byte currentTempSensorAddr[8];



OneWire ds(temperatureSensor1Pin); 







void setup(){  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);   
  pinMode(temperatureSensor1Pin, INPUT);
  pinMode(muxSignalPin, INPUT);
  
  pinMode(muxSelectorPin0, OUTPUT);
  pinMode(muxSelectorPin1, OUTPUT);
  pinMode(muxSelectorPin2, OUTPUT);
  pinMode(muxSelectorPin3, OUTPUT);
  pinMode(powerRelay1Pin, OUTPUT);
  pinMode(powerRelay2Pin, OUTPUT);
  
  
  digitalWrite(muxSelectorPin0, LOW);
  digitalWrite(muxSelectorPin1, LOW);
  digitalWrite(muxSelectorPin2, LOW);
  digitalWrite(muxSelectorPin3, LOW);
  
}





void loop(){
  
  updateAllTemperatureReadings();
  
  updateAllLightSensorReadings();

  respondToCommandsFromSerialInput();
  
  writeAllReadingsToSerialOutput();
      
}











void updateAllLightSensorReadings(){
  for(int i = 0; i < 10; i ++){
    lightSensorReadings[i] = readMux(i);
  }
}



int readMux(int channel){
  int controlPin[] = {muxSelectorPin0, muxSelectorPin1, muxSelectorPin2, muxSelectorPin3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(muxSignalPin);
  return val;
}



void updateAllTemperatureReadings(){  
  float currentTempReading = 0;
  String currentROM = "";
  
  for(int i = 0; i < 10; i ++){
    currentTempReading = getTemp();
  
    if(currentTempReading != -1000){
      currentROM = "";
      for(int j = 0; j < 8; j++) {
        currentROM += String(currentTempSensorAddr[j], HEX);
      }
                                    
      if(currentROM == "2815cab5400bf"){
          temperatureReadings[0] = currentTempReading;
      }else if(currentROM == "281336b54008a"){
          temperatureReadings[1] = currentTempReading;
      }else{
          Serial.print("ALERT - New ROM found: ");
          Serial.println(currentROM);
      }         
    }else{
      break;
    }
  }  
}




float getTemp(){
  byte type_s;
  byte data[12];
  float celsius, fahrenheit;
  
  if ( !ds.search(currentTempSensorAddr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    delay(250);
    return -1000;
  }
    
  if ( OneWire::crc8( currentTempSensorAddr, 7) != currentTempSensorAddr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }
  
  if ( currentTempSensorAddr[0] != 0x10 && currentTempSensorAddr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }
  
  ds.reset();
  ds.select(currentTempSensorAddr);
  ds.write(0x44,1); 
  
  delay(1000);
  
  byte present = ds.reset();
  ds.select(currentTempSensorAddr); 
  ds.write(0xBE); 
  
  
  for (int i = 0; i < 9; i++) { 
    data[i] = ds.read();
  }  
  
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  
  return celsius;
}














void respondToCommandsFromSerialInput(){  
  if(Serial.available() > 0){
    String currentCommand = Serial.readString();
    
    Serial.print("Recieved command from Raspi =");
    Serial.println(currentCommand);
    
    if(currentCommand.indexOf("PR0:") > -1){
      int powerRelay1CommandIndex = currentCommand.indexOf("PR0:");
      powerRelay1On = (currentCommand.charAt(powerRelay1CommandIndex+4) == '1');
      digitalWrite(powerRelay1Pin, ((powerRelay1On) ? HIGH : LOW));
    }
    
    if(currentCommand.indexOf("PR1:") > -1){ 
      int powerRelay2CommandIndex = currentCommand.indexOf("PR1:");
      powerRelay2On = (currentCommand.charAt(powerRelay2CommandIndex+4) == '1');
      digitalWrite(powerRelay2Pin, ((powerRelay2On) ? HIGH : LOW));
    }    
  }  
}




















void writeAllReadingsToSerialOutput(){
  int i = 0;
  for(i = 0; i < 10; i++){
   Serial.print("LS");
   Serial.print(i);
   Serial.print(":");
   //Serial.print(map(lightSensorReadings[i], 0, 1023, 0, 255));
   Serial.print(lightSensorReadings[i]);
   Serial.print(",");    
  }
  
  for(i = 0; i < 2; i++){
   Serial.print("TS");
   Serial.print(i);
   Serial.print(":");
   Serial.print(temperatureReadings[i]);
   Serial.print(",");    
  }
  
  Serial.print("PR0:");
  Serial.print(powerRelay1On);
  Serial.print(",PR1:");
  Serial.println(powerRelay2On);  
}





