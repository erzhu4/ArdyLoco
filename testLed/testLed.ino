const int RPWM = 2;
const int LPWM = 3;
const int R_EN = 4;
const int L_EN = 5;
const int alwaysHigh = 6;
const int led1 = 7;
const int led2 = 8;

// const int RPWM = D1;
// const int LPWM = D2;
// const int R_EN = D3;
// const int L_EN = D4;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println("SET UP!!!");
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(alwaysHigh, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(alwaysHigh, HIGH);
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);
  Serial.println("SET UP END!!!");
}

void loop() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);


  // digitalWrite(led1, HIGH);
  // digitalWrite(led2, LOW);
  // delay(1000);
  // digitalWrite(led1, LOW);
  // digitalWrite(led2, HIGH);
  // delay(1000);
}
