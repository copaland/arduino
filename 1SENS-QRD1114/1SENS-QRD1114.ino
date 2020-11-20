/********************
QRD1114_Proximity_Sensor.ino
Connect a QRD1114, 330 resistor and 10k resistor as follows:

QRD1114 Pin ---- Arduino ---- Resistors
    1              A0      10k Pull-up to 5V
    2              GND
    3                      330 Resistor to 5V
    4              GND
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

#define SENS 7 //49 QRD1114
#define LED 13

bool swFlag = LOW;
int interval = 500;
int count=0;

void setup() 
{
  Serial.begin(9600);
  pinMode(SENS, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() 
{ 
  static unsigned long nextMil = millis() + interval;
  if(millis() >= nextMil){
    nextMil = millis() + interval;
      count++;
      count = count % 2;  
  }
  
  if(digitalRead(SENS) == LOW) 
  { delay(5);
    swFlag=HIGH;
  } else {
    swFlag=LOW;
  }

  if(swFlag==HIGH){
    switch(count){
      case 0: digitalWrite(LED, LOW);
      break;
      case 1: digitalWrite(LED, HIGH);
      break;      
    }
  } else {
    digitalWrite(LED, LOW);
  } 
  delay(1);
}


/*
 * void loop() 
{
  if(digitalRead(SENS) == LOW) 
  {//if button pull down
    delay(5);
    swFlag=HIGH;
  } else {
    swFlag=LOW;
  }
  if(!digitalRead(SENS)){
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

*/
