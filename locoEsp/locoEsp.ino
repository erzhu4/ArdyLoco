/*
 * ESP8266 Servo Motor Control With Web Server 
 * https://circuits4you.com
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#include "index.h"

#define LED 2
#define ServoPin 14   //D5 is GPIO14

//WiFi Connection configuration
const char *ssid = "PrettyFly4aWifi";
const char *password = "Bitch123";

const int acceleration = 75;
const int decceleration = 75;


//Apparently 88 is the center pos for this config

const int offSet = 0;
const int initialThrottlePos = 88;

//raw pos needs to be between 0 - 180


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

ESP8266WebServer server(80);

int throttlePos = initialThrottlePos;

void stepUpThrottle(){
    throttlePos += 1;
}

void stepDownThrottle(){
    throttlePos -= 1;
}

void setThrottlePosition(int targetPos){ // throttle position 1 to 11
  Serial.println("setThrottlePosition");
  Serial.println(targetPos);
  if (throttlePos > targetPos){
      while (throttlePos != targetPos){
          stepDownThrottle();
          myservo.write(throttlePos);
          delay(decceleration);
      }
  } else {
      while (throttlePos != targetPos){
          stepUpThrottle();
          myservo.write(throttlePos);
          delay(acceleration);
      }
  }
  Serial.println("Throttle Position set complete");
}


//================================================
void handleServo(){
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  setThrottlePosition(pos + offSet);
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(pos);
  digitalWrite(LED,!(digitalRead(LED))); //Toggle LED
  server.send(200, "text/plane","");
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
  myservo.attach(ServoPin); // attaches the servo on GIO2 to the servo object
  
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
  server.on("/setPOS",handleServo); //Sets servo position from Web request
  server.begin();  
}

//================================================
//     LOOP
//================================================
void loop() {
 server.handleClient();
}
//================================================