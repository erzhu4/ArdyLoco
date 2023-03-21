const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>ESP8266 HIGH CURRENT MOTOR DRIVER</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <style>
        .flx{
            display: flex;
        }
        .slider p {
            font-size: 26px;
            font-weight: 600;
            font-family: Open Sans;
            padding-left: 30px;
            color: black;
        }
        .slider input[type="range"] {
            width: 420px;height: 2px;background: black;border: none;outline: none;
        }
        .slider input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none !important;width: 30px;height:30px;background: black;border: 2px solid black;border-radius: 50%;cursor: pointer;
        }
        .slider input[type="range"]::-webkit-slider-thumb:hover {
            background: black;
        }

    </style>
    <body>
        <div style="width:100%;">
            <div style="width:50%;  margin: 0 auto;">
                <h1>ESP8266 with HighCurrent BTS7960 43A High Power Motor Driver</h1>
                <h4>ESP8266 Train Control.</h4>
            </div>
            <div id="errorField"></div>
        </div>

        <div style="width: 50%; margin: 0 auto;" class="flx">
            <div class="slider">
                <input type="range" min="0" max="255" value="0" onmouseup="rangeValueChange(this.value)" oninput="rangeValue.innerText = this.value">
                <p id="rangeValue">0</p>
            </div>
        </div>
        <div style="text-align:center;">
            <button onmouseup="stop()" id="stopButton">Stop</button>
        </div>
        <div style="text-align:center;">
            Set SPEED:
            <span id="speedText">forward</span>
        </div>
        <div style="text-align:center;">
            <span id="directionText">forward</span>
            <button id="changeDirection" onmouseup="directionChange()">Change Direction</button>
        </div>
        <div style="text-align:center;">
            <button id="light-on" onmouseup="turnLightON()">Light On</button>
        </div>
        <div style="text-align:center;">
            <button id="light-off" onmouseup="turnLightOff()">Light Off</button>
        </div>

        <script>
            const mainDescription = "Throttle Setting";
            function sendGetReq(req){
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {};
                xhttp.open("GET", req, true);
                xhttp.send();
            }
            function sendPosData(pos) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("speedText").innerHTML = this.responseText;
                        console.log(this.responseText);
                    }
                };
                xhttp.open("GET", "setPOS?throttlePOS="+pos, true);
                xhttp.send();
            }
            function rangeValueChange(value) {
                document.getElementById("rangeValue").innerHTML = value;
                sendPosData(value);
            }
            function directionChange(){
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("directionText").innerHTML = this.responseText;
                    }
                };
                xhttp.open("GET", "setDir", true);
                xhttp.send();
            }
            function stop(){
                sendPosData(0);
            }
            function turnLightON(){
                sendGetReq('lightOn');
            }
            function turnLightOff(){
                sendGetReq('lightOff');
            }

        </script>

    </body>
</html>
)=====";