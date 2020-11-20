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
#define BUZZER(x) digitalWrite(BUZZ,x==HIGH?HIGH:LOW)

bool swFlag = LOW;//button flag
bool bzFlag = LOW;//buzzer flag
bool X = LOW;
int interval = 500;
int ldCount=0;
int bzCount=0;

void setup() 
{
  Serial.begin(9600);
  pinMode(SENS, INPUT_PULLUP);
  pinMode(BUZZ, OUTPUT);
}

void loop() 
{ 
  static unsigned long bzMil = millis() + interval;

  if((millis() > bzMil) && (bzFlag==HIGH)){
    bzMil = millis() + interval;
    bzCount++;
    X=!X;
    if(X==HIGH) BUZZER(1);
    else BUZZER(0);    
    if(bzCount > 6) {
      bzCount = 0; 
      bzFlag=LOW;
      BUZZER(0);
    }
  }
    
  if(digitalRead(SENS) == LOW) 
  { delay(5);
    bzFlag=HIGH;
  }

  delay(1);
}
