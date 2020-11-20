/*NO INTERRUPT - 
D2 ENC CLK
D3 ENC DT
D4 ENC BTN
D5 X-DIR
D6 X-STEP
D7 SENS Photo  
D8 LED-R
D9 LED-G
D10 LED-B
D11 BUZZER
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <Stepper.h>
#include <A4988.h>
#include <BasicStepperDriver.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); //0x3f 0x27

// Pin Definitions
#define SW     4 //ENCODER button
#define DT     3 //ENCODER dt  B
#define CLK    2 //ENCODER clk A

#define stepperDelayTime  1200
#define XDIR    5 //STEP MOTOR
#define XSTEP   6
#define SENS   7 //광센서

#define LED_R  8
#define LED_G  9
#define LED_B  10
#define BUZZ   11 //부저
#define XEN    12 //STEP MOTOR LOW ENABLE
#define ledPin 13

#define rd_ENCA digitalRead(CLK)//a
#define rd_ENCB digitalRead(DT) //b
#define BUZZER(x) digitalWrite(BUZZ,x==HIGH?HIGH:LOW)
#define RLAMP(x) digitalWrite(LED_R,x==HIGH?HIGH:LOW)
#define GLAMP(x) digitalWrite(LED_G,x==HIGH?HIGH:LOW)
#define BLAMP(x) digitalWrite(LED_B,x==HIGH?HIGH:LOW)

//Navigation algorithm
byte MODE = 0; //SCREEN LEVEL
int ROTATION=0; //0:STOP, 1:CW, 2:CCW encoder Dir
int PUSH=0; //0:OFF, 1:ON EncoderBT ?
int SCREEN=1; //LCD screen index
int LINE=1; // LCD cursor position

// Variables will change:       
unsigned long preMil = 0; 
unsigned long curMil = 0;      
long interval = 10;  //timer
bool X = LOW;
bool ACT=LOW;

byte buttonState; 
unsigned long lastButtonPress = 0;
byte oldButtonState = HIGH;  // 풀업 저항으로 인해 스위치가 열려 있다고 가정
unsigned long buttonPressTime;  // 스위치가 마지막으로 변경된 상태
boolean buttonPressed = 0; // a flag variable
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
int counter = 0; //rotary counter
int oldC = 0;
int newC = 0;
  
byte mode_cnt=0;
int  STEPS=200;
#define MICROSTEPS 1
#define RPM 60

int deg=0; //STEP MOTOR DEGREE
int setDeg=0; 
byte motorState=0; //1:start, 2:pause, 4: stop
//Stepper stepper(STEPS, XSTEP, XDIR);
BasicStepperDriver stepper(STEPS, XDIR, XSTEP, XEN);

void setup()
{
  pinMode(CLK, INPUT_PULLUP); //  
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  digitalWrite(SW,HIGH);
  
  pinMode(XSTEP, OUTPUT);
  pinMode(XDIR, OUTPUT);
//  pinMode(XEN, OUTPUT);  
//  digitalWrite(XEN,LOW); //stepper Xenable

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZ, OUTPUT);

 /// stepper.setSpeed(30);//SPEED
  stepper.begin(RPM, MICROSTEPS);
  stepper.setEnableActiveState(LOW);//enable/disable
  
  Serial.begin(9600); 
    
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
///  lcd.createChar(1, arrow);   //Create the arrow symbol
  lcd.home(); 
  lcd.clear();
  lcd.setCursor(1,0);
//  lcd.write(1); //arrow  

  //         0123456789ABCDEF  
  lcd.print("3D PRINTER A002");
  lcd.setCursor(1,1);
  lcd.print("READY TO DRIVE"); 

  delay(2000); //5초 동안 lcd 표시

  home_screen();//x,y
  preMil=millis();    
/*  
  Xmotor(-200); //0-CCW
  delay(2000);
  Xmotor(200); //1-CW
  delay(2000);
 */ 
}

void loop()
{

  static unsigned long nextMil = millis() + interval;
  if(millis() >= nextMil){
    nextMil = millis() + interval;  
  }
  newC = read_encoder(); 
  if(oldC != newC){
    oldC = newC;
    lcd.setCursor(1,1);
    lcd.print(newC);   
  }
  if(PUSH){
    PUSH=LOW;
    setDeg = newC * 100;
    lcd.setCursor(0,1);

    ACT=HIGH;
  }
  if(ACT==HIGH){
    stepper.enable();
//    stepper.step(setDeg);
    stepper.rotate(setDeg);
    //stepper.move(-MOTOR_STEPS*MICROSTEPS);
    stepper.disable();
    ACT=LOW;
  }
  delay(1);
}
void home_screen(void)
{
  lcd.setCursor(0,0);
  //         0123456789ABCDEF 
  lcd.print(" X:   Y:    A001");
  lcd.setCursor(0,1);
 // lcd.print("3D PRINTER READY"); 
  lcd.print("                "); 
}

int read_encoder(void)
{
  static bool oldA = HIGH; //set the oldA as HIGH
  static bool oldB = HIGH; //set the oldB as HIGH

  bool newA = digitalRead(CLK);//
  bool newB = digitalRead(DT);//

  if(digitalRead(SW) == LOW)//if button pull down
  {
    while(digitalRead(SW) == LOW){
      delay(5);
    }
      PUSH=1; 
  }  
  if (newA != oldA || newB != oldB) //
  {
    // something has changed
    if (oldA == HIGH && newA == LOW){
      if(oldB == HIGH){
        counter++;//
        ROTATION=1; currentDir ="CW"; 
      } else if(oldB == LOW) {
        counter--;//
        ROTATION=2; currentDir ="CCW"; 
      } 
    } 
  }
  oldA = newA;
  oldB = newB;
  return counter;//result;
}
/*
void Xmotor(int numberOfSteps)
{
  int steps = abs(numberOfSteps); 
  int dir = (numberOfSteps>0) ? HIGH : LOW;
  digitalWrite(XDIR, dir); //LOW-CW
   
  for(int i=0;i<steps;i++) //each loop is one step
  {
    digitalWrite(XSTEP,HIGH);
    delayMicroseconds(stepperDelayTime);  
    digitalWrite(XSTEP,LOW);
    delayMicroseconds(stepperDelayTime);
  }
}
*/
