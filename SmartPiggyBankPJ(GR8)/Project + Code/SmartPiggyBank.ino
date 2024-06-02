#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Khai báo các chân cắm
const int S0 = 8;
const int S1 = 9;
const int S2 = 11;
const int S3 = 12;
const int sensorOut = 10;
const int irSensorPin = 7;

//Khai báo màu sắc
int red = 0;
int green = 0;
int blue = 0;
int total = 0;
int a = 0, b = 0, c = 0;

void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(irSensorPin, INPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  lcd.begin();
  lcd.backlight();
  lcd.print("Smart Piggy Bank");
  lcd.setCursor(5, 1);
  lcd.print("GROUP 8  ");
  delay(3000);
  lcd.clear();
}

void loop() {
  int sensor = digitalRead(irSensorPin);

  readColorSensor();

  //In ra các giá trị màu để kiểm tra
  Serial.print("Red: "); Serial.println(red);
  Serial.print("Blue: "); Serial.println(blue);
  Serial.print("Green: "); Serial.println(green);
  Serial.println("-----------------------------");

  if (sensor == LOW) {
    if (isRed()) {
      a = 1;
    } else if (isBlue()) {
      b = 1;
    } else if (isGreen()) {
      c = 1;
    }
  }

  if (sensor == HIGH) {
    if (a == 1) {
      a = 0;
      lcd.setCursor(0, 1);
      lcd.print("  +50.000 VND!");
      total += 50;
    } else if (b == 1) {
      b = 0;
      lcd.setCursor(0, 1);
      lcd.print("  +20.000 VND!");
      total += 20;
    } else if (c == 1) {
      c = 0;
      lcd.setCursor(0, 1);
      lcd.print("  +100.000 VND!");
      total += 100;
    }

    delay(1000);
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Total:");
  lcd.setCursor(11, 0);
  lcd.print(total);
  lcd.print("K");
  delay(500);
}

void readColorSensor() {
  int redSum = 0, blueSum = 0, greenSum = 0;
  const int numReadings = 30;

  for (int i = 0; i < numReadings; i++) {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redSum += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);


    digitalWrite(S3, HIGH);
    blueSum += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);

    digitalWrite(S2, HIGH);
    greenSum += pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  }

  red = redSum;
  blue = blueSum;
  green = greenSum;
}

//Đỏ
bool isRed() {
  return (red < blue && red < green && red <= 210);
}

//Xanh dương
bool isBlue() {
  return (blue < red && blue < green);
}


//Xanh lá
bool isGreen() {
  return (red < blue && red < green && red > 230);
}
