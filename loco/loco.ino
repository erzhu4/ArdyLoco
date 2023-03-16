// ARDUINO NANO WIFI WITH DKKY 40A Brushed ESC

#include <Servo.h>
#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "theBitchTrain";        // your network SSID (name)
char pass[] = "Bitch123";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

Servo ESC;

//config
const int acceleration = 300;
const int decceleration = 300;

const int light1Pin = 4;
const int light2Pin = 5;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {

  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  Serial.println("Access Point Web Server");

  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);
  pinMode(light1Pin, OUTPUT);
  pinMode(light2Pin, OUTPUT);
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }


  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  ESC.attach(3, 1000, 2000);
}

// 0 to 180
int throttlePos = 0;

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
          ESC.write(throttlePos);
          delay(decceleration);
      }
  } else {
      while (throttlePos != targetPos){
          stepUpThrottle();
          ESC.write(throttlePos);
          delay(acceleration);
      }
  }
}

void lightOn(){
  digitalWrite(light1Pin, HIGH);
  digitalWrite(light2Pin, HIGH);
}

void lightOff(){
  digitalWrite(light1Pin, LOW);
  digitalWrite(light2Pin, LOW);
}

void loop() {
  // Serial.println("loop starts");
  // ESC.write(throttlePos);
  // setThrottlePosition(6);
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
  
  WiFiClient client = server.available();   // listen for incoming clients

   if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<style>");
            client.print(".container {margin: 0 auto; text-align: center; margin-top: 100px;}");
            client.print("button {color: white; width: 100px; height: 100px;");
            client.print("border-radius: 50%; margin: 20px; border: none; font-size: 20px; outline: none; transition: all 0.2s;}");
            client.print(".red{background-color: rgb(196, 39, 39);}");
            client.print(".green{background-color: rgb(39, 121, 39);}");
            client.print(".blue {background-color: rgb(5, 87, 180);}");
            client.print(".off{background-color: grey;}");
            client.print("button:hover{cursor: pointer; opacity: 0.7;}");
            client.print("</style>");
            client.print("<h1>DKKY 40A Brushed ESC</h1>");
            client.print("<div class='container'>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/0\"'>0</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/1\"'>1</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/2\"'>2</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/3\"'>3</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/4\"'>4</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/5\"'>5</button>");
            client.print("<br>");
            client.print("<button class='blue' type='submit' onmousedown='location.href=\"/stop\"'>STOP</button>");
            client.print("<br>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/7\"'>7</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/8\"'>8</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/9\"'>9</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/ten\"'>10</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/eleven\"'>11</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/twelve\"'>12</button>");
            client.print("<br>");
            client.print("<br>");
            client.print("<button class='blue' type='submit' onmousedown='location.href=\"/lightOn\"'>Light</button>");
            client.print("<button class='off' type='submit' onmousedown='location.href=\"/lightOff\"'>Light Off</button>");
            client.print("<br>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/up\"'>up</button>");
            client.print("<button class='green' type='submit' onmousedown='location.href=\"/down\"'>down</button>");
            client.print("</div>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was /X
        if (currentLine.endsWith("GET /0")) {
            setThrottlePosition(40); //
        }
        else if (currentLine.endsWith("GET /1")) {
            setThrottlePosition(43);
        }
        else if (currentLine.endsWith("GET /2")) {
            setThrottlePosition(47);          
        }
        else if (currentLine.endsWith("GET /3")) {
            setThrottlePosition(50);
        }
        else if (currentLine.endsWith("GET /4")) {
            setThrottlePosition(54);
        }
        else if (currentLine.endsWith("GET /5")) {
            setThrottlePosition(57);//59 is too weak
        }
        else if (currentLine.endsWith("GET /stop")) {
            setThrottlePosition(64);
        }
        else if (currentLine.endsWith("GET /7")) {
            setThrottlePosition(69);//good
        }
        else if (currentLine.endsWith("GET /8")) {
            setThrottlePosition(72);//good
        }
        else if (currentLine.endsWith("GET /9")) {
            setThrottlePosition(76);//good
        }
        else if (currentLine.endsWith("GET /ten")) {
            setThrottlePosition(79); //good
        }
        else if (currentLine.endsWith("GET /eleven")) {
            setThrottlePosition(82); //good
        }
        else if (currentLine.endsWith("GET /twelve")) {
            setThrottlePosition(86); 
        }
        else if (currentLine.endsWith("GET /up")) {
            stepUpThrottle();
        }
        else if (currentLine.endsWith("GET /down")) {
            stepDownThrottle();
        } ///twelve
        else if (currentLine.endsWith("GET /lightOn")) {
            stepDownThrottle();
        }
        else if (currentLine.endsWith("GET /lightOff")) {
            stepDownThrottle();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}