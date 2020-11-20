//NO INTERRUPT - 오차발생
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
#define LED_B  4
#define LED_G  5
#define LED_R  6
#define encA  19 //CLK
#define encB  18 //D
#define encBtn 15 //SW
#define BUZZ  53

#define stepperDelayTime  1200
#define STEPPER_XSTEP A0//
#define STEPPER_XDIR A1//
#define STEPPER_XEN 38 //LOW ENABLE

#define STEPPER_YSTEP A6//
#define STEPPER_YDIR A7//
#define STEPPER_YEN A2 //LOW ENABLE

#define rd_ENCA digitalRead(encA)
#define rd_ENCB digitalRead(encB)
#define BUZZER(x) ((x)==1 ? digitalWrite(BUZZ,HIGH) : digitalWrite(BUZZ,LOW) )

byte MODE = 0;
uint8_t arrow[8] = {0x0, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00, 0x00};

int state, prevstate = 0, count = 0;
int nextEncoderState[4] = { 2, 0, 3, 1 };
int prevEncoderState[4] = { 1, 3, 0, 2 };

// Variables will change:       
unsigned long previousMillis = 0; 
unsigned long currentMillis=0;      
long interval = 10;  //timer

byte buttonState; 
byte oldButtonState = HIGH;  // 풀업 저항으로 인해 스위치가 열려 있다고 가정
unsigned long buttonPressTime;  // 스위치가 마지막으로 변경된 상태
boolean buttonPressed = 0; // a flag variable
byte mode_cnt=0;
int motorStep=200;

void read_encoder(void);
void Xmotor(byte dir, int numberOfSteps);
void Ymotor(byte dir, int numberOfSteps);


void setup()
{
  pinMode(encA, INPUT_PULLUP); //  
  pinMode(encB, INPUT_PULLUP);
  pinMode(encBtn, INPUT_PULLUP);
  pinMode(STEPPER_XEN, OUTPUT);
  pinMode(STEPPER_YEN, OUTPUT);
  pinMode(STEPPER_XSTEP, OUTPUT);
  pinMode(STEPPER_YSTEP, OUTPUT);
  digitalWrite(STEPPER_XEN,LOW); //stepper Xenable
  digitalWrite(STEPPER_YEN,LOW); //stepper Yenable
  
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(1, arrow);   //Create the arrow symbol
  lcd.home();

//  attachInterrupt(4,encoA,RISING); // INT4 set an interrupt on encA, looking for a rising edge signal and executing the "encA" Interrupt Service Routine (below)
//  attachInterrupt(5,encoB,RISING);
  
  Serial.begin(9600); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);  
  lcd.print("Choose coffee");
  lcd.setCursor(0,1);  
  lcd.print(" Sugar level");
  delay(2000);
  lcd.clear();
  
  Xmotor(0,motorStep); //0-CCW
  delay(2000);
  Xmotor(1,motorStep); //1-CW
  delay(2000);

  Ymotor(0,motorStep); //1-CcW
  delay(2000);
  Ymotor(1,motorStep); //0-CW
  delay(2000);  
}

void loop()
{

  if(millis() - previousMillis >= interval) {
    previousMillis = currentMillis;   
    read_encoder();   
  }
 // if(MODE){
 //   Xmotor(0,motorStep);
 //   MODE = 0;
 // }
  /*
  if(mode_cnt%2==0){
    Xmotor(0,motorStep);
  } else {
    Xmotor(1,motorStep);
  }
  */
  
/*
  switch(MODE){
    case 0:
      
    break;
    case 1:
      
    break;
    case 2:
    break;
  }
  */
  delay(10);
}

void read_encoder(void)
{
  state = (digitalRead(encA) << 1) | digitalRead(encB);
  if (state != prevstate) {
    if (state == nextEncoderState[prevstate]) {
       count++;
    } else if (state == prevEncoderState[prevstate]) {
       count--; 
    }
    Serial.println(count, DEC); //for Debug
    prevstate = state;
  }
  
  buttonState = digitalRead(encBtn);
  if (buttonState != oldButtonState){
    oldButtonState =  buttonState;
    if (buttonState == LOW){
        Serial.println ("Button PRESSED"); // DEBUGGING: 
        buttonPressed = 1;
        motorStep = count; MODE = 1; //mode_cnt++;
      } else {
 //       Serial.println ("Button RELEASE"); // DEBUGGING: 
        buttonPressed = 0;  
      }
  }
}


void Xmotor(byte dir, int numberOfSteps)
{
  digitalWrite(STEPPER_XDIR, dir); //LOW-CW
   
  for(int i=0;i<numberOfSteps;i++) //each loop is one step
  {
    digitalWrite(STEPPER_XSTEP,HIGH);
    delayMicroseconds(stepperDelayTime);  
    digitalWrite(STEPPER_XSTEP,LOW);
    delayMicroseconds(stepperDelayTime);
  }
}

void Ymotor(byte dir, int numberOfSteps)
{
  digitalWrite(STEPPER_YDIR, dir); //define the direction
   
  for(int i=0;i<numberOfSteps;i++) //each loop is one step
  {
    digitalWrite(STEPPER_YSTEP,HIGH);
    delayMicroseconds(stepperDelayTime);  
    digitalWrite(STEPPER_YSTEP,LOW);
    delayMicroseconds(stepperDelayTime);
  }
}
