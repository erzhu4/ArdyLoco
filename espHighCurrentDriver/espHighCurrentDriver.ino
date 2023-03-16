#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// #include <SPI.h>
// #include <WiFiNINA.h>

#include "indexespHighCurrent.h"

#define LED 2

//WiFi Connection configuration
const char *ssid = "PrettyFly4aWifi";
const char *password = "Bitch123";

const int acceleration = 50;
const int decceleration = 50;

const int minimumThrottle = 0;
const int maxThrottle = 255;


const int offSet = 0;
const int initialThrottlePos = 0;

const int IN1 = D0;
const int IN2 = D1;
const int EN1 = D2;
const int EN2 = D3;

bool dir = false;

ESP8266WebServer server(80);

int throttlePos = initialThrottlePos;

void stepUpThrottle(){
    throttlePos += 1;
}

void stepDownThrottle(){
    throttlePos -= 1;
}

void setThrottlePosition(int targetPos){ // throttle position 1 to 11
    Serial.println("initial target pos");
    Serial.println(targetPos);
    targetPos = targetPos + minimumThrottle;

    if (targetPos < (minimumThrottle + 2)){
        targetPos = 0;
    }
    Serial.println("setThrottlePosition");
    Serial.println(targetPos);
    int targetPin = IN1;
    if (dir){
        targetPin = IN2;
    } else {
        targetPin = IN1;
    }
    if (throttlePos > targetPos){
        while (throttlePos != targetPos){
            stepDownThrottle();
            analogWrite(targetPin, throttlePos);
            delay(decceleration);
        }
    } else {
        while (throttlePos != targetPos){
            stepUpThrottle();
            analogWrite(targetPin, throttlePos);
            delay(acceleration);
        }
    }
    Serial.println("Throttle Position set complete");
}

void handleDirection(){
    Serial.print("Change Directions");
    setThrottlePosition(0);
    dir = !dir;
    if (dir){
        server.send(200, "text/plane","backwards");
    } else {
        server.send(200, "text/plane","forward");
    }
}


//================================================
void handleThrottle(){
  String POS = server.arg("throttlePOS");
  int pos = POS.toInt();
  pos = pos + offSet;
  //limit pos
  if (pos > maxThrottle){
    pos = maxThrottle;
  } else if (pos < 0){
    pos = 0;
  }
  setThrottlePosition(pos);
  digitalWrite(LED,!(digitalRead(LED))); //Toggle LED
//   server.send(200, "text/plane","");
  server.send(200, "text/plane", String(pos));
}

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

//================================================
//            Setup
//================================================
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  pinMode(LED,OUTPUT);
  
  //Connect to wifi Network
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //Initialize Webserver
  server.on("/",handleRoot);
  server.on("/setPOS",handleThrottle); //Sets servo position from Web request
  server.on("/setDIR",handleDirection);
  server.begin();
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
}

//================================================
//     LOOP
//================================================
void loop() {
 server.handleClient();
}
//================================================