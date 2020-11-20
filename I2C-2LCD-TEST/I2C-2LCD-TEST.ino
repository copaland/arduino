/*
 제목    : I2C LCD에 문자 출력하기 
 */

// i2c & LCD를 제어하기 위한 라이브러리.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 0x27 I2C 주소를 가지고 있는 16x2 LCD객체를 생성합니다.
//(I2C 주소는 LCD에 맞게 수정해야 합니다.)
LiquidCrystal_I2C lcd(0x27, 16, 2);
int count = 0;

void setup() {
  // I2C LCD를 초기화 합니다..
  lcd.init();
  // I2C LCD의 백라이트를 켜줍니다.
  lcd.backlight();
  lcd.setCursor(0,0);  // 0번째 줄 0번째 셀에 커서 위치
  lcd.print("0123456789012345"); 
  delay(2000); 
  lcd.print("                ");
}

void loop() { 
  count++;
  count = count % 100;
  lcd.setCursor(0,0);  // 0번째 줄 0번째 셀에 커서 위치
  lcd.print("Hello World!!"); 
  lcd.setCursor(4,1);  // 1번째 줄 0번째 셀에 커서 위치.
  lcd.print(count); // lcd에 문구를 출력.

  delay(3000);
  // LCD의 모든 내용을 삭제합니다.
  lcd.clear();
}
