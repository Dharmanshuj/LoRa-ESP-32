#include <WiFi.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>

#define LED1 15
#define LED2 12

char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bootstrap Sliding Switches</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        /* Custom CSS for the sliding switch */
        
        .container{
            display: flex;
            justify-content: center;
        }

        .box{
            background-color: black;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
        }

        .text{
            color: aliceblue;
        }

        .LED{
            display: flex;
            justify-content: center;
            gap: 200px;
            margin-top: 20%;
        }
        
        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 34px;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }

        input:checked + .slider {
            background-color: #2196F3;
        }

        input:checked + .slider:before {
            transform: translateX(26px);
        }
    </style>
</head>
<body>

<div class = "box">
    <div class="container mt-5">
        <h2 class="text">LED On and Off</h2>
    </div>
    <div class = "LED Switches">
        <div class = "switch1">
            <h3 class="text"> LED-1</h3>
            <label class="switch first">
                <input type="checkbox" onclick="toggleLED('led1', this.checked)">On/Off
                <span class="slider"></span>
            </label>
        </div>
        <div class = "switch2">
            <h3 class="text">LED-2</h3>
            <label class="switch second">
                <input type="checkbox" onclick="toggleLED('led1', this.checked)">On/Off
                <span class="slider"></span>
            </label>
        </div>
    </div>
</div>

<script>
    function toggleLED(led, state)
    {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "http://" + location.hostname + "/" + led + "/" + (state ? "on" : "off"), true);
        xhr.send();
    }
</script>
<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.2/dist/umd/popper.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>


)=====";

AsyncWebServer server(80); // server port 80

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Page Not found");
}

void setup(void) {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  WiFi.softAP("DJ", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());

  if (MDNS.begin("ESP")) { // esp.local/
    Serial.println("MDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest *request) { 
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request) { 
    digitalWrite(LED1, HIGH);
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request) { 
    digitalWrite(LED1, LOW);
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest *request) { 
    digitalWrite(LED2, HIGH);
    request->send_P(200, "text/html", webpage);
  });

  server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest *request) { 
    digitalWrite(LED2, LOW);
    request->send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
}

void loop(void) {
}
