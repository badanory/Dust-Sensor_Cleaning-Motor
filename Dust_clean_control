#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// 센서/모터 핀
const int GasPin = A0;
const int A = 8;
const int B = 9;

// 히스테리시스 임계값
// smoke >= UPPER 이면 ON, smoke <= LOWER 이면 OFF, 그 사이에서는 이전 상태 유지
int UPPER = 140;   // 예: 오염이 충분히 높아지면 켜짐
int LOWER = 80;    // 예: 충분히 낮아지면 꺼짐

// 상태 변수
bool fanOn = false;

// (선택) 간단한 노이즈 완화용 이동평균
int readSmokeAveraged(uint8_t samples = 8) {
  long sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += analogRead(GasPin);
    delay(2); // 샘플 사이 약간의 간격
  }
  return (int)(sum / samples);
}

void setFan(bool on) {
  if (on) {
    // A 방향 회전
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
  } else {
    // 정지
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
  }
}

void printStatus(int smoke) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smoke Value: ");
  lcd.print(smoke);

  lcd.setCursor(0, 1);
  if (fanOn) {
    lcd.print("Danger: Cleaning");
  } else {
    lcd.print("Safe: No Dust");
  }
}

void setup() {
  pinMode(GasPin, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);

  // 초기 상태 표시
  setFan(fanOn);
  printStatus(0);
}

void loop() {
  // 센서 읽기 (노이즈 완화를 위해 평균값 사용)
  int smokeValue = readSmokeAveraged(8);

  // 시리얼 출력
  Serial.print("Smoke Value: ");
  Serial.println(smokeValue);

  // 히스테리시스 로직
  if (!fanOn && smokeValue >= UPPER) {
    fanOn = true;
    setFan(true);
    printStatus(smokeValue);
    Serial.println("Fan -> ON (upper reached)");
  } else if (fanOn && smokeValue <= LOWER) {
    fanOn = false;
    setFan(false);
    printStatus(smokeValue);
    Serial.println("Fan -> OFF (lower reached)");
  } else {
    // 상태 변화 없을 때도 값만 갱신해서 보여주고 싶다면 아래 두 줄 사용
    lcd.setCursor(13, 0); // "Smoke Value: " 다음 위치
    lcd.print("    ");    // 자릿수 지우기
    lcd.setCursor(13, 0);
    lcd.print(smokeValue);
  }

  delay(200); // 루프 주기
}
