#include <Wire.h>                     // i2C 통신을 위한 라이브러리
#include <LiquidCrystal_I2C.h>        // LCD 2004 I2C용 라이브러리
LiquidCrystal_I2C lcd(0x27,20,4);     // 접근주소: 0x3F or 0x27

// 연기 센서 핀
int GasPin = A0;  // 가스 센서의 아날로그 핀

// 팬 모터 핀
int A = 8;       // 팬 모터 제어 핀 A
int B = 9;       // 팬 모터 제어 핀 B

// 연기 감지 임계값
int threshold = 100;  // 연기 농도가 100 이상이면 팬 작동

void setup() {
  // 핀 설정
  pinMode(GasPin, INPUT);  // 연기 센서 입력 핀 설정
  pinMode(A, OUTPUT);      // 팬 모터 핀 A 출력 모드
  pinMode(B, OUTPUT);      // 팬 모터 핀 B 출력 모드
 
  lcd.init();                      // LCD 초기화
  lcd.backlight();                // 백라이트 켜기

  // 시리얼 통신 초기화
  Serial.begin(9600);
}

void loop() {
  // 연기 센서 값 읽기
  int smokeValue = analogRead(GasPin);
  lcd.init();                      // LCD 초기화
  // 시리얼 모니터에 연기 값 출력
  Serial.print("Smoke Value: ");
  Serial.println(smokeValue);
  lcd.setCursor(0, 0);          // 첫번째 줄 문자열 출력
  lcd.print("Smoke Value: ");
  lcd.print(smokeValue);

  // 연기 감지 여부 확인
  if (smokeValue > threshold) {
    // 팬 모터 작동 (A 방향 회전)
    lcd.init();                      // LCD 초기화
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
    Serial.println("Danger: Cleaning");
    lcd.setCursor(0, 1);          // 두번째 줄 문자열 출력
    lcd.print("Danger: Cleaning");
  } else {
    // 팬 모터 정지
    lcd.init();                      // LCD 초기화
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    Serial.println("Safe: No Dust");
    lcd.setCursor(0, 1);          // 두번째 줄 문자열 출력
    lcd.print("Safe: No Dust");
  }
  // 1초 대기
  delay(1000);
}
