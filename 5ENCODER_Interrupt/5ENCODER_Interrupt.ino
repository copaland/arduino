
#define SW     4 //ENCODER button
#define DT     3 //ENCODER dt  B
#define CLK    2 //ENCODER clk A

#define DIR    5 //STEP MOTOR
#define STEP   6
#define SENS   7 //광센서

#define LED_R  8
#define LED_G  9
#define LED_B  10
#define BUZZ   11 //부저
//#define XEN    12 //STEP MOTOR LOW ENABLE
#define ledPin 13

int encPos = 0;
static int pinA = 2;//19 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3;//18 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;

void setup() {
  pinMode(CLK, INPUT); 
  pinMode(DT, INPUT); 
  attachInterrupt(0, encA, RISING);//CHANGE);
  attachInterrupt(1, encB, RISING);//CHANGE);     
  Serial.begin (9600);
}

void loop(){
}
//Rotary encoder interrupt service routine for one encoder pin
void encA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PINE & 0x30; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00110000 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00010000) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

//Rotary encoder interrupt service routine for the other encoder pin
void encB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PINE & 0x30; //reading = PIND & 0xC;read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00110000 && bFlag) { //if (reading == B00001100 && bFlag) {check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00100000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
/*
void doEncoderA(){ //low-to-high
  cli();
  if (digitalRead(CLK) == HIGH) { 
    if (digitalRead(DT) == LOW) {  
      encPos = encPos + 1; //cw
    } else {
      encPos = encPos - 1; //ccw
    }
  } else { // high-to-low edge on channel A                                       
   if (digitalRead(DT) == HIGH) {   
      encPos = encPos + 1; //cw
    } else {
      encPos = encPos - 1; //ccw
    }
  }
  sei();
  Serial.println (encPos); //debug
}

void doEncoderB(){ //low-to-high
  cli();
  if (digitalRead(DT) == HIGH) { 
    if (digitalRead(CLK) == HIGH) {  
      encPos = encPos + 1;
    } else {
      encPos = encPos - 1;
    }
  } else { // high-to-low edge on channel A                                       
   if (digitalRead(DT) == LOW) {   
      encPos = encPos + 1;
    } else {
      encPos = encPos - 1;
    }
  }
  sei();
  Serial.println (encPos);
}

*/
