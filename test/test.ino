void setup() {
  // put your setup code here, to run once:
  pinMode(D1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(D1, 0);
  delay(100);
  analogWrite(D1, 25);
  delay(100);
  analogWrite(D1, 50);
  delay(100);
  analogWrite(D1, 75);
  delay(100);
  analogWrite(D1, 100);
  delay(100);
  analogWrite(D1, 125);
  delay(100);
  analogWrite(D1, 150);
  delay(100);
  analogWrite(D1, 175);
  delay(100);
  analogWrite(D1, 200);
  delay(100);
  analogWrite(D1, 225);
  delay(100);
  analogWrite(D1, 255);
  delay(100);
}
