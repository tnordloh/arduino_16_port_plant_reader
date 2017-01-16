#include <SoftwareSerial.h>

int secondcount = 0;

// Attach the serial display's RX line to digital pin 2
SoftwareSerial mySerialData(A4,13); // RX,TX (unused)

int sensorValue[16] = {0,0,0,0,
                       0,0,0,0,
                       0,0,0,0,
                       0,0,0,0
                     };
int sensorpower = A0;

//Mux control pins
int s0 = 2;
int s1 = 3;
int s2 = 4;
int s3 = 5;

int s4 = 6;
int s5 = 7;
int s6 = 8;
int s7 = 9;
//int muxstate = 6;
//Mux in "SIG" pin
int SIG_pin = A1;


//set up shift register pins
int SER_Pin   = 10; //pin 14 on the 75HC595
int RCLK_Pin  = 11; //pin 12 on the 75HC595
int SRCLK_Pin = 12; //pin 11 on the 75HC595

//Number of daisy-chained shift registers
#define number_of_74hc595s 2 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8
boolean registers[numOfRegisterPins];

void setup() {

  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);


  pinMode(SIG_pin, INPUT);
  pinMode(sensorpower, OUTPUT);
  digitalWrite(sensorpower, LOW);

  //set Mux initial values
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 
  pinMode(s4, OUTPUT); 
  pinMode(s5, OUTPUT); 
  pinMode(s6, OUTPUT); 
  pinMode(s7, OUTPUT); 
  //pinMode(muxstate, OUTPUT);

  //initialize mux to C0
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  digitalWrite(s4, LOW);
  digitalWrite(s5, LOW);
  digitalWrite(s6, LOW);
  digitalWrite(s7, LOW);

  //start mux in "off" staate
  //digitalWrite(muxstate, LOW);
  mySerialData.begin(9600); // set up serial port for 9600 baud
  mySerialData.write(0x12);
  mySerialData.write("2");
  delay(500); // wait for display to boot up



  //Serial.begin(9600); 
  delay(500);

}

void loop() {

  secondcount++;
  if(secondcount % 6 == 0 ) {
    for(int x=0;x<16;++x) {
      checkSensors(x);
    }
  }
  for(int x=0;x<16;++x) {
    setLights(x);
  }
  writeRegisters();
  lcdStatus();
  delay(500);  
}

//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    registers[i] = LOW;
  }
}
/**************************************************************
 * Begin register-specific code
 *************************************************************/
//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);
    //Serial.print(registers[i]);
    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  //Serial.println();
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

/*****************************************************
 * end register-specific functions
 **************************************************/
/**************************************************
 * begin mux-specific functions
 **************************************************/


int readSensor(int channel){
  //digitalWrite(muxstate,LOW);
  int controlPin[] = { s0, s1, s2, s3 };

  int muxChannel[16][4]={
    { 0,0,0,0 }, //channel 0
    { 1,0,0,0 }, //channel 1
    { 0,1,0,0 }, //channel 2
    { 1,1,0,0 }, //channel 3
    { 0,0,1,0 }, //channel 4
    { 1,0,1,0 }, //channel 5
    { 0,1,1,0 }, //channel 6
    { 1,1,1,0 }, //channel 7
    { 0,0,0,1 }, //channel 8
    { 1,0,0,1 }, //channel 9
    { 0,1,0,1 }, //channel 10
    { 1,1,0,1 }, //channel 11
    { 0,0,1,1 }, //channel 12
    { 1,0,1,1 }, //channel 13
    { 0,1,1,1 }, //channel 14
    { 1,1,1,1 }  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    Serial.print("control pin:");
    Serial.print(i);
    Serial.print(" muxchannel:");
    Serial.println(muxChannel[channel][i]);
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  //return the value at the SIG pin
  return analogRead(SIG_pin);
}


void EnableSensor(int channel){
  //digitalWrite(muxstate,LOW);
  int controlPin[] = { s4, s5, s6, s7 };

  int muxChannel[16][4]={
    { 0,0,0,0 }, //channel 0
    { 1,0,0,0 }, //channel 1
    { 0,1,0,0 }, //channel 2
    { 1,1,0,0 }, //channel 3
    { 0,0,1,0 }, //channel 4
    { 1,0,1,0 }, //channel 5
    { 0,1,1,0 }, //channel 6
    { 1,1,1,0 }, //channel 7
    { 0,0,0,1 }, //channel 8
    { 1,0,0,1 }, //channel 9
    { 0,1,0,1 }, //channel 10
    { 1,1,0,1 }, //channel 11
    { 0,0,1,1 }, //channel 12
    { 1,0,1,1 }, //channel 13
    { 0,1,1,1 }, //channel 14
    { 1,1,1,1 }  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    //Serial.print("control pin:");
    //Serial.print(i);
    //Serial.print(" muxchannel:");
    //Serial.println(muxChannel[channel][i]);
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  //read the value at the SIG pin
  //int val = analogRead(SIG_pin);
  //digitalWrite(muxstate,HIGH);
  //return the value
  //return val;
}

/**************************************************
 * end mux-specific functions
 **************************************************/

void checkSensors(int value) {
  //Serial.print("value:");
  //Serial.println(value);
  EnableSensor(value);
  digitalWrite(sensorpower,HIGH);
  //give power 10 ms to stabilize
  delay(10);
  //change to the correct sensor, then delay another 10 ms
  readSensor(value);
  delay(10);
  //do a final read
  sensorValue[value] = readSensor(value);
  //Serial.println(sensorValue[value]);  
  digitalWrite(sensorpower,LOW);
}

void setLights(int light) {
  if((secondcount % 4 == 0 || secondcount % 4 == 2)  && sensorValue[light] >650) {
    //digitalWrite(ledPin[light], HIGH);
    setRegisterPin(light, HIGH);
    //Serial.println("Light should be on (second is even, value >800");
  } 
  else if((secondcount % 4 == 1 || secondcount % 4 == 3)  && sensorValue[light] >650) {
    //Serial.println("Light should be off (second is odd, value >800");
    setRegisterPin(light, LOW);
    //digitalWrite(ledPin[light], LOW);
  } 
  else if(secondcount % 4 == 0 && sensorValue[light] >550) {
    //digitalWrite(ledPin[light], HIGH);
    setRegisterPin(light, HIGH);
    //Serial.println("Light should be on");
  } 
  else if(secondcount % 4 == 2 && sensorValue[light] >550) {
    setRegisterPin(light, LOW);
  } 
  else {
    setRegisterPin(light, LOW);
    //Serial.println("Light should be off");
  }

}
void lcdStatus() {
  int offset=secondcount%8/2*4;
  if(secondcount %1 == 0) {
    mySerialData.write(254); // move cursor to beginning of first line
    mySerialData.write(128);

    mySerialData.write("                "); // clear display
    mySerialData.write("                ");

    mySerialData.write(254); // move cursor to beginning of first line
    mySerialData.write(128);

    for(int i = 0;i<4;++i) {
      if(i == 2) {
        mySerialData.write(254); // move cursor to beginning of first line
        mySerialData.write(192);
      }      
      mySerialData.print(i+offset);
      mySerialData.print(":");

      mySerialData.print(sensorValue[i+offset]);
      mySerialData.print(" ");
    }
  }
}






