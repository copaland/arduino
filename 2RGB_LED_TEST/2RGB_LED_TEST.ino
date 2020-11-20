/********************
2-RGB_LED_BLINK.ino
D2 ENC SW
D3 ENC DT
D4 ENC CLK
D5 X-DIR
D6 X-STEP
D7 SENS Photo  
D8 LED-R
D9 LED-G
D10 LED-B
D11 BUZZER
******************/
#define SW     2 //ENCODER button
#define DT     3 //ENCODER dt  B
#define CLK    4 //ENCODER clk A

#define DIR    5 //STEP MOTOR
#define STEP   6
#define SENS   7 //광센서

#define LED_R  8
#define LED_G  9
#define LED_B  10
#define BUZZ   11 //부저
//#define XEN    12 //STEP MOTOR LOW ENABLE
#define LED    13

bool swFlag = LOW;
int interval = 500;
int count=0;

void setup() 
{
  Serial.begin(9600);
  pinMode(SENS, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop() 
{ 
  static unsigned long nextMil = millis() + interval;
  if(millis() >= nextMil){
    nextMil = millis() + interval;
      count++;
      count = count % 3;  
  }
  
  if(digitalRead(SENS) == LOW) 
  { delay(5);
    swFlag=HIGH;
  } else {
    swFlag=LOW;
  }

  if(swFlag==HIGH){
    switch(count){
      case 0: 
        digitalWrite(LED_R, HIGH);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, LOW);
      break;
      case 1: 
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_B, LOW);      
      break;  
      case 2: 
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, HIGH); 
      break; 
    }
  } else {
        digitalWrite(LED_R, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_B, LOW);
  } 
  delay(1);
}
