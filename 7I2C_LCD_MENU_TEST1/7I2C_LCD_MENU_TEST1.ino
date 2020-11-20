#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
#define BUZZER(x) (digitalWrite(BUZZ,x==HIGH?HIGH:LOW))
//uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00, 0x00};
byte arrow[8] = {0x00, 0x04 ,0x02, 0x01, 0x02, 0x04, 0x00, 0x00};
/*
byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
*/
char * main_screen[] = {"Exit Menu      ", "Set Temp       ", "ITEM 3         ", "ITEM 4    ", "ITEM 5    "};
char *clr_screen[]    ={"                 ","                 "};
//char *main_screen[]   ={"Exit           ","Motor Test     ","Nozzle Set     ","Emergency Set   "};
//char *mot2_screen[]   ={"Exit           ","Motor Test     ","Nozzle Set     ","Emergency Set   "};

char *pre10_screen[]  ={"Back           ","Move X Angle?  ","Move Y Angle?  "};
char *con20_screen[]  ={"Back           ","HeatingBed Tp  ","Emergency Set  "};

char *pre11_screen[]  ={"Back           ","X Angle:"};
char *pre12_screen[]  ={"Back           ","Y Angle:"};

char *con21_screen[]  ={"TEMP SETTING   ","BED:"};
char *con22_screen[]  ={"Emergency Set  ","DISTANCE:"};

int menuNum = sizeof(main_screen)/sizeof(char *); // array size
int menuStep = 0;  //0 메뉴 단계 (0:초기 화면, 1:1단계 메뉴, 2:2단계 메뉴)
int itemStep = 0;  //0 메뉴 값 (0:초기 화면, 1:메뉴_1, 2:메뉴_2, 3:메뉴_3)

struct select //메뉴
{
  uint8_t main; //main screen
  uint8_t pre10; //prepare mode
  uint8_t con20; //control mode
  uint8_t pre11; //
  uint8_t pre12;
  uint8_t con21;
  uint8_t con22;
}menu={4,3,3,2,2,2,2}; //menu count
bool X=LOW;
int setTemp = 25;  // 온도 설정값
int interval = 1000;

int lcdCols = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x3F, 16, 2); //0x3f 0x27

void setup() {
  pinMode(SW, INPUT_PULLUP);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  // Initializes and clears the LCD screen
  Serial.begin(9600); 
    
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(1, arrow);   //Create the arrow symbol
  lcd.home(); 
  lcd.clear();
  lcd.setCursor(1,0);
//  lcd.write(1); //arrow  

  //         0123456789ABCDEF  
  lcd.print("3D PRINTER A001");
  lcd.setCursor(1,1);
  lcd.print("READY TO DRIVE"); 

  delay(5000); //5초 동안 lcd 표시

  home_screen();//x,y
}

void loop() {
  static unsigned long nextMil = millis() + interval;
  
  if(digitalRead(SW) == LOW)//if button pull down
  {
    while(digitalRead(SW) == LOW){
      delay(5);
    }
    menuStep++;  // 버튼이 클릭되면 1씩 증가
  }
  if(millis() >= nextMil){
    nextMil = millis() + interval; 
    X = HIGH;  
  }

  if(menuStep == 0) home_screen();
  else if(menuStep == 1) displayMenu();
  else if(menuStep >= 3) { displayMenu(); menuStep = 1; lcd.setCursor(12, 1); lcd.print("  "); }
  else {
    switch(itemStep) {
    case 1:
      menuItem1();
      break;
    case 2:
      menuItem2();
      break;
    case 3:
      menuItem3();
      break;
    case 4:
      menuItem4();
      break;
    case 5:
      menuItem5();
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
    }
  }

  if(X){
    X=LOW;
//    Serial.print("m:");
//    Serial.print(menuStep);
//    Serial.print(" i:");
//    Serial.println(itemStep);
  }
}

void menuItem1() {  // 메뉴에서 나감, 디스플레이 모드로
  menuStep = 0;
  itemStep = 0;
}

void menuItem2() {
  lcd.setCursor(12, 1); lcd.print(setTemp);
  int change = getEncoderTurn();
  setTemp = setTemp + change;
}

void menuItem3() {
}

void menuItem4() {
}

void menuItem5() {
}

void home_screen(void)
{
  lcd.setCursor(1,0);
  //         0123456789ABCDEF 
  lcd.print("X:   Y:    A001");
  lcd.setCursor(0,1);
  lcd.print("3D PRINTER READY");  
}

void displayMenu() {
  int change = getEncoderTurn();
  itemStep = itemStep + change;
  
 // if(itemStep > menuNum) itemStep = 1;//itemStep = menuNum;
 // else if(itemStep < 2) itemStep = 5;//ITEMmAX
  if(itemStep >= menuNum) itemStep = menuNum;
  else if(itemStep <= 1) itemStep = 1;
 
  if(itemStep <= lcdRows) { 
    lcd.setCursor(0, itemStep - 1); lcd.write(1);
    lcd.setCursor(0, lcdRows - itemStep); lcd.print(" ");
  } else {
    lcd.setCursor(0, lcdRows - 1); lcd.write(1);
  }

  if(itemStep == 1) {
    lcd.setCursor(1, 0);
    lcd.print(main_screen[0]);
    lcd.setCursor(1, 1);
    lcd.print(main_screen[itemStep]);
  } else {
    lcd.setCursor(1, 1);
    lcd.print(main_screen[itemStep - 1]);
  }
} 

int getEncoderTurn() {  // 로터리 엔코더 값 반환 (-1, 0, +1)
  static bool oldA = HIGH; //set the oldA as HIGH
  static bool oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  bool newA = digitalRead(CLK);//
  bool newB = digitalRead(DT);//

  if (newA != oldA || newB != oldB) {
    if (oldA == HIGH && newA == LOW){
      result = (oldB * 2 - 1); 
    }
  }
  oldA = newA;
  oldB = newB;
  return result;
}
/*
void menu_navi(void)
{
  screen_display();
  if((ROTATION==1)&&(LINE>1)&&(SCREEN<4)) //(SCREEN<menu num-1)
    SCREEN=SCREEN+1; //CW, CUSOR아래, 아래로 화면전환
  if((ROTATION==2)&&(LINE<2)&&(SCREEN>1)) 
    SCREEN=SCREEN-1; //CCW, CUSOR위, 위로 화면전환
  if((ROTATION==1)&&(LINE<2))
    LINE=LINE+1; //1->2
  if((ROTATION==2)&&(LINE>1)) 
    LINE=LINE-1; //2->1
  ROTATION=0;
  if(MODE==0)
  {
    if(PUSH==1)
    {
      PUSH=0;
      MODE=1;
    }     
  }
  if(MODE==1)
  {
    if(PUSH==1){
      PUSH=0;
      switch(LINE+SCREEN){
        case 2: Function_A(); break;
        case 3: Function_B(); break;
        case 4: Function_C(); break; 
        default: break;
      }
    }    
  }
}

 */
