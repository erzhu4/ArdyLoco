// Arduino nano wifi with HighCurrent BTS7960 43A High Power Motor Driver
// ardNanoHighCurrent

#include <SPI.h>
#include <WiFiNINA.h>

#include "ardNanoHighCurrent.h"

int status = WL_IDLE_STATUS;

//WiFi Connection configuration
const char *ssid = "PrettyFly4aWifi";
const char *password = "Bitch123";

const int acceleration = 50;
const int decceleration = 50;

const int minimumThrottle = 0;
const int maxThrottle = 255;


const int offSet = 0;
const int initialThrottlePos = 0;

const int RPWM = 2;
const int LPWM = 3;
const int R_EN = 4;
const int L_EN = 5;
const int alwaysHigh = 6;

const int l1 = 8;
const int l2 = 9;

bool dir = false;

WiFiServer server(80);

//helpers--------------------------------------------------------------------------------
void sendResponseToClient(WiFiClient client, String r) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println(r);
    client.println();
}

String getRemainingQUeryString(WiFiClient client){
    String result = "";
    for (int i = 0; i < 40; i++){
        char x = client.read();
        if (x == ' '){
            break;
        }
        result += x;
    }
    return result;
}

String returnValueFromQUeryString(String str){
    int indexOfTheEqualSign = str.indexOf('=');
    String result = str.substring(indexOfTheEqualSign + 1, str.length());
    return result;
}

//helpers end--------------------------------------------------------------------------------

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
    int targetPin = RPWM;
    int downPin = LPWM;
    if (dir){
        targetPin = LPWM;
        downPin = RPWM;
    } else {
        targetPin = RPWM;
        downPin = LPWM;
    }
    if (throttlePos > targetPos){
        while (throttlePos != targetPos){
            stepDownThrottle();
            analogWrite(targetPin, throttlePos);
            analogWrite(downPin, 0);
            delay(decceleration);
        }
    } else {
        while (throttlePos != targetPos){
            stepUpThrottle();
            analogWrite(targetPin, throttlePos);
            analogWrite(downPin, 0);
            delay(acceleration);
        }
    }
    Serial.println("Throttle Position set complete");
}

void handleDirection(){
    Serial.print("Change Directions");
    setThrottlePosition(0);
    dir = !dir;
}

void turnOnLights(){
    Serial.print("turnOnLights");
    digitalWrite(l1, HIGH);
    digitalWrite(l2, HIGH);
}

void turnOffLights(){
    Serial.print("turnOnLights");
    digitalWrite(l1, LOW);
    digitalWrite(l2, LOW);
}


//================================================
void handleThrottle(String POS){
    int pos = POS.toInt();
    pos = pos + offSet;
    //limit pos
    if (pos > maxThrottle){
        pos = maxThrottle;
    } else if (pos < 0){
        pos = 0;
    }
    setThrottlePosition(pos);
}

void handleRoot(WiFiClient client) {
    String s = MAIN_page;
    sendResponseToClient(client, s);
}

//================================================
//            Setup
//================================================
void setup() {
    Serial.begin(115200);
    Serial.println();

    pinMode(RPWM, OUTPUT);
    pinMode(LPWM, OUTPUT);
    pinMode(R_EN, OUTPUT);
    pinMode(L_EN, OUTPUT);
    pinMode(alwaysHigh, OUTPUT);
    pinMode(l1, OUTPUT);
    pinMode(l2, OUTPUT);
    digitalWrite(alwaysHigh, HIGH);
    digitalWrite(R_EN, HIGH);
    digitalWrite(L_EN, HIGH);
    digitalWrite(l1, LOW);
    digitalWrite(l2, LOW);

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the firmware");
    }
    
    //Connect to wifi Network
    WiFi.begin(ssid, password);     //Connect to your WiFi router
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

//================================================
//     LOOP
//================================================
void loop() {
    if (status != WiFi.status()) {
        status = WiFi.status();
    }
    WiFiClient client = server.available();   // listen for incoming clients
    if (client) {                             // if you get a client,
        Serial.println("new client");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
            if (client.available()) {           // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                // Check to see if the client request was /X
                if (currentLine.endsWith("GET /setPOS")) {
                    String queryString = getRemainingQUeryString(client);
                    String pos = returnValueFromQUeryString(queryString);
                    handleThrottle(pos);
                    sendResponseToClient(client, String(throttlePos));
                    break;
                }

                if (currentLine.endsWith("GET /setDir")) {
                    handleDirection();
                    if (dir){
                        sendResponseToClient(client, "backwards");
                    } else {
                        sendResponseToClient(client, "forward");
                    }
                    break;
                }

                if (currentLine.endsWith("GET /lightOn")){
                    turnOnLights();
                    sendResponseToClient(client, "lightOn");
                    break;
                }

                if (currentLine.endsWith("GET /lightOff")){
                    turnOffLights();
                    sendResponseToClient(client, "lightOff");
                    break;
                }

                if (c == '\n') {                    // if the byte is a newline character
                    if (currentLine.length() == 0) {
                        handleRoot(client);
                        break;
                    } else {    // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
//================================================