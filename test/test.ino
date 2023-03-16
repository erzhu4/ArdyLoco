const int RPWM = 3;
const int LPWM = 5;
const int R_EN = 6;
const int L_EN = 7;

const int alwaysHigh = 8;

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
  digitalWrite(alwaysHigh, HIGH);
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);
  Serial.println("SET UP END!!!");
}

void loop() {
  Serial.println("LOOP~!!!~~~!");
  for (int i = 0; i < 200; i++){
    analogWrite(RPWM, i);
    analogWrite(LPWM, 0);
    delay(20);
  }
  Serial.println("Loop Back");
  for (int i = 200; i > 0; i--){
    analogWrite(RPWM, i);
    analogWrite(LPWM, 0);
    delay(20);
  }
}
