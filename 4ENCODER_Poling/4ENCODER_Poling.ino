//Rotary Encode SIMPLE OK 20201029
/*
D2 ENC BTN
D3 ENC DT
D4 ENC CLK
D5 X-DIR
D6 X-STEP
D7 SENS Psensor 
D8 LED-R
D9 LED-G
D10 LED-B
D11 BUZZER
*/

#define CLK 2 //ENCODER clk 
#define DT  3 //ENCODER dt 
#define SW  4 //ENCODER button

#define STEP  5
#define DIR   6
#define SENS  7 //광센서
#define LED_R  8
#define LED_G  9
#define LED_B  10

#define BUZZ   11 //부저
#define LED 13

#define BUZZER(x) (digitalWrite(BUZZ,x==HIGH?HIGH:LOW))

int encoderVal = 0;
int interval = 1000;
bool Flag = LOW;
int tmp=0;

void setup()
{ 
  //set clkPin,dtPin,swPin as INPUT
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(SW, HIGH);

  Serial.begin(9600); // 
}


void loop()
{
  static unsigned long nextMil = millis() + interval;
  int change = getEncoderTurn();//
  encoderVal = encoderVal + change;
  
  if(digitalRead(SW) == LOW)//pull down
  {
    while(digitalRead(SW) == LOW){
      delay(5);
    }
    encoderVal = 0;
  }
  
  if(millis() >= nextMil){
    nextMil = millis() + interval;   
  }
  
  if(tmp!=encoderVal){
    Serial.println(encoderVal);
    tmp=encoderVal;
  }
  
  digitalWrite(LED,Flag);
  delay(10);
}

int getEncoderTurn(void)
{
  int result = 0;
  static bool oldA = HIGH; //set the oldA as HIGH
  static bool oldB = HIGH; //set the oldB as HIGH
  bool newA = digitalRead(CLK);
  bool newB = digitalRead(DT);
  
  if (newA != oldA || newB != oldB) 
  { // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      if(oldB == HIGH){
        result = 1; Flag=HIGH;
      } else if(oldB == LOW) {
        result = -1; Flag=LOW;
      }
    }    
  }

  oldA = newA;
  oldB = newB;
  return result;
}  
  /*
   //ENCREMENT
  if (newA != oldA || newB != oldB) 
  {
    // something has changed
    if (oldA == HIGH && newA == LOW)
    {
      result = (oldB * 2 - 1);
    }
  }    
  */