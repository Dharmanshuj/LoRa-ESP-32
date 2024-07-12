#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define IR_SENSOR_PIN 36
#define READING_LIGHT_PIN 12
#define CEILING_LIGHTS_PIN 13
#define STANDING_LAMP_PIN 17
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5

#define SEALEVELPRESSURE_HPA (1013.25)

static const int RXPin = 34, TXPin = 35;
static const uint32_t GPSBaud = 9600;

// Use this for hardware SPI
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

bool isReadingSensorData = false;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void send_sensor();

Ticker timer;

char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Practice</title>
    <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyCZJ450iqXt4Pb3VhXoMYeA4gCP_R9ePFU&callback=initMap" async defer></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            width: 100vw;
            margin: 0;
            padding: 0;
            background-color: rgb(113, 212, 251);
            scroll-behavior: smooth;
        }

        .container {
            width: 100%;
            height: 600px;
            display: grid;
            grid-template-columns: 2fr 1fr 1fr;
            grid-template-rows: 1fr 1fr 1fr;
            gap: 9px;
        }

        .light, .living{
            display: flex;
            justify-content: space-between;
            align-items: center;
            gap: 10px;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .room, .inside, .outside, .music, .map {
            background-color: white;
            padding: 20px;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        .room {
            grid-column: 1;
            grid-row: 1 / 3;
        }

        .inside {
            grid-column: 2;
            grid-row: 1;
        }

        #time {
          display: flex;
          justify-content: center;
          font-size: 40px;
          margin: auto;
          margin-top: 25px;
        }

        #date {
          display: flex;
          justify-content: center;
          font-size: 20px;
          margin: auto;
          margin-top: 25px;
        }

        .outside {
            grid-column: 3;
            grid-row: 1;
        }

        #player {
      background-color: #080747;
      padding: 28px;
      height: 120px;
    }
    
    #player img {
      width: 42px;
      height: 42px;
      object-fit: cover;
      border-radius: 10px;
    }
    
    .info {
      color: #E1E1E6;
    }

    .info h1 {
        font-size: 17px; /* Reduced font size */
    }
    
    .info p {
      opacity: 0.68;
      font-size: 14px;
    }
    
    .info-wrapper {
      display: flex;
      align-items: center;
      gap: 30px;
      flex: 1;
      height: 60px;
    }
    
    .controls {
      display: flex;
      justify-content: space-between;
      height: 40px;
    }
    
    .track {
        width: 100%;
        height: 6px;
        background-color: #D9D9D9;
        position: relative;
        margin-bottom: 10px;
    }

    .track-time {
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    
    .track::before {
      content: '';
      height: 6px;
      width: 85%;
      display: block;
      background: #D9D9D9;
      opacity: 0.3;
      border-radius: 10px;
      position: absolute;
    }
    
    .track::after {
      content: '';
      height: 6px;
      width: 85%;
      display: block;
      background: #D9D9D9;
      border-radius: 10px;
    }
    
    .time {
      opacity: 0.7;
      font-size: 14px;
      color: gainsboro;
      display: flex;
      justify-content: space-between;
      gap: 500px;
    }
    
    .prev, .play, .next {
      cursor: pointer;
      padding: 10px;
    }
    
    .prev svg, .play svg, .next svg {
      fill: #E1E1E6;
    }

        #map {
            grid-column: 2 / 4;
            grid-row: 2 / 4;
        }

        .slider {
          display: inline-block;  
          width: 40px;
          height: 20px;
          background-color: #ccc;
          border-radius: 10px;
          position: relative;
          cursor: pointer;
          transition: background-color 0.4s;
          margin-left: auto;
          margin-right: 50px;
        }

        .slider:before {
            content: "";
            position: absolute;
            width: 20px;
            height: 20px;
            background-color: #fff;
            border-radius: 10px;
            transition: transform 0.4s;
        }

        .switch input:checked + .slider {
            background-color: #2196F3;
        }

        .switch input:checked + .slider:before {
            transform: translateX(20px);
        }

        @media only screen and (max-width: 768px) {
            .container {
                grid-template-columns: 1fr 1fr;
                grid-template-rows: 1fr 1fr 1fr 1.7fr;
            }

            .room {
                grid-column: 1 / -1;
                grid-row: 1;
                display: grid;
                grid-template-columns: 1fr 1fr;
                gap: 9px;
            }

            .inside {
                grid-column: 1;
                grid-row: 2;
            }

            .outside {
                grid-column: 2;
                grid-row: 2;
            }

            .music {
                grid-column: 1 / -1;
                grid-row: 3;
            }

            #player {
                display: flex;
                flex-direction: column;
                margin: auto;
                width: 95%;
                height: 120px; 
                padding: 14px;
            }

            #map {
                grid-column: 1 / -1;
                grid-row: 4 / -1;
                overflow: hidden;
            }

            .slider {
                width: 40px;
                height: 20px;
            }

            .slider:before {
                width: 18px;
                height: 18px;
            }

            section {
                padding: 100px 20px;
                height: 50vh;
                box-sizing: border-box;
            }

            .info-wrapper{
                height: 50px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="room">
            <div class="living">
                <h2>Living Room</h2>
                <label class="switch">
                  <input type="checkbox" onchange="toggleLiving(this.checked)">
                  <div class="slider"></div>
                </label>
            </div>
            <div class="light" id="readingLight">
                <h4>Reading Light</h4>
                <label class="switch">
                  <input type="checkbox" onclick="toggleLED('readingLight', this.checked)">
                  <span class="slider"></span>
                </label>
            </div>
            <div class="light" id="ceilingLights">
                <h4>Ceiling Lights</h4>
                <label class="switch">
                  <input type="checkbox" onclick="toggleLED('ceilingLights', this.checked)">
                  <span class="slider"></span>
                </label>
            </div>
            <div class="light" id="standingLamp">
                <h4>Standing Lamp</h4>
                <label class="switch">
                  <input type="checkbox" onclick="toggleLED('standingLamp', this.checked)">
                  <span class="slider"></span>
                </label>
            </div>
        </div>
        <div class="inside temperature">
            <p id="time"></p>
            <p id="date"></p>
        </div>
        <div class="outside temperature">
            <p id="temperature"> Temperature : 0 °C</p>
            <p id="humidity"> Humidity : 0 %</p>
            <p id="pressure"> Pressure : 0 hPa</p>
        </div>
        <div id="player" class=" music wrapper horizontal">
            <div class="info-wrapper">
              <img src="https://cdn.pixabay.com/photo/2016/03/26/13/09/cup-of-coffee-1280537_1280.jpg" alt="LogoMusicImage">
              <div class="info">
                <h1>Way Down We Go</h1>
                <p>Banda OneBitMusic</p>
              </div>
            </div>
            <div class="controls">
              <div class="prev">
                <svg width="30" height="30" viewBox="0 0 30 30" fill="none" xmlns="http://www.w3.org/2000/svg">
                  <path d="M2.33917 13.7397L12.9664 7.38149C13.2293 7.22152 13.5303 7.13509 13.8381 7.13123C14.1458 7.12737 14.4489 7.20622 14.7157 7.35955C15.0053 7.52815 15.245 7.77036 15.4107 8.0616C15.5763 8.35284 15.6619 8.68272 15.6588 9.01775V13.4657L25.8274 7.3798C26.0903 7.21983 26.3914 7.13341 26.6991 7.12955C27.0068 7.12568 27.3099 7.20454 27.5768 7.35786C27.8663 7.52646 28.1061 7.76867 28.2717 8.05991C28.4373 8.35115 28.5229 8.68103 28.5198 9.01606V21.4512C28.5231 21.7863 28.4376 22.1163 28.2719 22.4077C28.1063 22.699 27.8664 22.9413 27.5768 23.1099C27.3099 23.2632 27.0068 23.3421 26.6991 23.3382C26.3914 23.3344 26.0903 23.2479 25.8274 23.088L15.6588 16.9993V21.4489C15.6625 21.7844 15.5771 22.1149 15.4114 22.4067C15.2458 22.6985 15.0057 22.9411 14.7157 23.1099C14.4489 23.2632 14.1458 23.3421 13.8381 23.3382C13.5303 23.3344 13.2293 23.2479 12.9664 23.088L2.33917 16.7298C2.08653 16.5715 1.87825 16.3516 1.73386 16.0908C1.58948 15.83 1.51373 15.5368 1.51373 15.2387C1.51373 14.9406 1.58948 14.6473 1.73386 14.3865C1.87825 14.1257 2.08653 13.9058 2.33917 13.7476V13.7397Z" fill="#E1E1E6"/>
                </svg>
              </div>
              <div class="play">
                <svg width="30" height="30" viewBox="0 0 30 30" fill="none" xmlns="http://www.w3.org/2000/svg">
                  <path d="M8.32137 25.586C7.9759 25.5853 7.63655 25.4948 7.33669 25.3232C6.66148 24.9406 6.24173 24.1978 6.24173 23.3915V7.07398C6.24173 6.26542 6.66148 5.52494 7.33669 5.14232C7.64369 4.96589 7.99244 4.87516 8.3465 4.87961C8.70056 4.88407 9.04692 4.98354 9.34938 5.16764L23.2952 13.5155C23.5859 13.6977 23.8255 13.9508 23.9916 14.251C24.1577 14.5511 24.2448 14.8886 24.2448 15.2316C24.2448 15.5747 24.1577 15.9121 23.9916 16.2123C23.8255 16.5125 23.5859 16.7655 23.2952 16.9478L9.34713 25.2979C9.0376 25.485 8.68307 25.5846 8.32137 25.586V25.586Z" fill="#E1E1E6"/>
                </svg>
              </div>
              <div class="next">
                <svg width="29" height="30" viewBox="0 0 29 30" fill="none" xmlns="http://www.w3.org/2000/svg">
                  <g clip-path="url(#clip0_42_51)">
                    <path d="M27.1426 13.7397L16.5154 7.38149C16.2525 7.22152 15.9514 7.13509 15.6437 7.13123C15.336 7.12737 15.0329 7.20622 14.766 7.35955C14.4765 7.52815 14.2368 7.77036 14.0711 8.0616C13.9055 8.35284 13.8199 8.68272 13.823 9.01775V13.4657L3.65435 7.3798C3.39144 7.21983 3.0904 7.13341 2.78268 7.12955C2.47495 7.12568 2.17183 7.20454 1.905 7.35786C1.61547 7.52646 1.37571 7.76867 1.21008 8.05991C1.04445 8.35115 0.958845 8.68103 0.961955 9.01606V21.4512C0.958745 21.7863 1.0443 22.1163 1.20994 22.4076C1.37558 22.699 1.61538 22.9413 1.905 23.1099C2.17183 23.2632 2.47495 23.3421 2.78268 23.3382C3.0904 23.3344 3.39144 23.2479 3.65435 23.088L13.823 16.9993V21.4489C13.8194 21.7844 13.9048 22.1149 14.0704 22.4066C14.2361 22.6984 14.4761 22.9411 14.766 23.1099C15.0329 23.2632 15.336 23.3421 15.6437 23.3382C15.9514 23.3344 16.2525 23.2479 16.5154 23.088L27.1426 16.7298C27.3953 16.5715 27.6035 16.3516 27.7479 16.0908C27.8923 15.83 27.968 15.5368 27.968 15.2387C27.968 14.9406 27.8923 14.6473 27.7479 14.3865C27.6035 14.1257 27.3953 13.9058 27.1426 13.7476V13.7397Z" fill="#E1E1E6"/>
                  </g>
                  <defs>
                    <clipPath id="clip0_42_51">
                      <rect width="28.8089" height="28.8089" fill="white" transform="translate(0.0612183 0.828369)"/>
                    </clipPath>
                  </defs>
                </svg>
              </div>
            </div>
            <div class="track-time">
              <div class="track"></div>
              <div class="time">
                <div class="totalTime">03:39</div>
                <div class="currentTime">00:00</div>
              </div>
            </div>
        </div>
        <section id="map"></section>
    </div>
    <script>
      window.onload = function() {
        setInterval(function() {
            var date = new Date();
            var displayDate = date.toLocaleDateString();
            var displayTime = date.toLocaleTimeString();
            document.getElementById('date').innerHTML = displayDate;
            document.getElementById('time').innerHTML = displayTime;
        }, 1000); // 1000 milliseconds = 1 second
      }
    </script>
    <script>
      function toggleLiving(state)
      {
        toggleLED('readingLight', state);
        toggleLED('ceilingLights', state);
        toggleLED('standingLamp', state);

      // Update the switch elements to reflect the new state
        const livingRoomSwitch = document.querySelector('.living .switch input');
        const readingLightSwitch = document.querySelector('#readingLight .switch input');
        const ceilingLightsSwitch = document.querySelector('#ceilingLights .switch input');
        const standingLampSwitch = document.querySelector('#standingLamp .switch input');

        if (state) {
          livingRoomSwitch.checked = true;
          readingLightSwitch.checked = true;
          ceilingLightsSwitch.checked = true;
          standingLampSwitch.checked = true;
          document.getElementById('readingLightLED').classList.add('on');
          document.getElementById('ceilingLightsLED').classList.add('on');
          document.getElementById('standingLampLED').classList.add('on');
        } 
        else {
          livingRoomSwitch.checked = false;
          readingLightSwitch.checked = false;
          ceilingLightsSwitch.checked = false;
          standingLampSwitch.checked = false;
        }
      }

      function toggleLED(led, state) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "http://" + location.hostname + "/" + led + "/" + (state ? "on" : "off"), true);
        xhr.send();
      }
    </script>
    <script>
      var map;
      var marker;

      function initMap() {
        var initialLocation = { lat: 25.262444, lng: 82.993278 };
        map = new google.maps.Map(document.getElementById("map"), {
          zoom: 17,
          center: initialLocation,
        });
        // const icon = {
        // url: 'https://drive.google.com/file/d/1RJvYP0uL1tcQDOgWgdPHABmBTRSA_ody/view?usp=drivesdk',
        // scaledSize: new google.maps.Size(30, 40) // adjust the size of the icon
        // };
        marker = new google.maps.Marker({
          position: initialLocation,
          map: map,
          title: "My Dog",
          // icon: icon,
        });

        var ws = new WebSocket('ws://'+location.hostname+':81/');

        ws.onmessage = function(event) {
          var data = JSON.parse(event.data);
          if(data.type == 'GPS') {
            var lat = data.lat;
            var lon = data.lon;
            updateDogPosition(lat, lon);
          }
          else if(data.type == 'BME') {
            document.getElementById('temperature').innerHTML = `Temperature : ${data.temperature} °C`;
            document.getElementById('humidity').innerHTML = `Humidity : ${data.humidity} %`;
            document.getElementById('pressure').innerHTML = `Pressure : ${data.pressure} hPa`;

            if(data.ir == '0')
              toggleLiving(true);
          }
          // else if(data.type == 'IR') {
          //   toggleLiving(true);
          // }
        };
      }

      function updateDogPosition(lat, lon) {
        var newLocation = { lat: lat, lng: lon };
        marker.setPosition(newLocation);
        map.panTo(newLocation);
      }
    </script>
    <script>
      const musicPlayer = {
        audio : new Audio(),
        currentTrackIndex : 0,
        tracks : [
          {
              title: "Track 1",
              url: "Audio1.mp3",
              duration: "3:39"
          },
          // Add more tracks as needed
      ],
      }
    </script>
</body>
</html>

)=====";

AsyncWebServer server(80);
WebSocketsServer websockets(81);

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
    {
      IPAddress ip = websockets.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

      // send message to client
      websockets.sendTXT(num, "Connected from server");
    }
    break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char *)(payload));
      Serial.println(message);
      DynamicJsonDocument doc(1024);
    // deserialize the data
      DeserializationError error = deserializeJson(doc, message);
      // parse the parameters we expect to receive (TO-DO: error handling)
        // Test if parsing succeeds.
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }
      String dataType = doc["type"];
      if (dataType == "BME") {
        // Handle BME sensor data
        double temperature = doc["temperature"];
        double pressure = doc["pressure"];
        double humidity = doc["humidity"];
        int ir = doc["ir"];
        String updateBMEScript = "updateBME(" + String(temperature, 1) + ", " + String(pressure, 1) + ", " + String(humidity, 1) + ", " + String(ir) + ");";
        websockets.sendTXT(num, updateBMEScript);
      } 
      else if (dataType == "GPS") {
        // Handle GPS sensor data
        double lat = doc["lat"];
        double lon = doc["lon"];
        String updateGPSScript = "updateGPS(" + String(lat, 6) + ", " + String(lon, 6) + ");";
        websockets.sendTXT(num, updateGPSScript);
      }
      // else if(dataType == "IR") {
      //   int ir_sensor = doc["ir"];
      //   String updateLEDScript = "updateLED(" + String(ir_sensor, 1) + ");";
      //   websockets.sendTXT(num, updateLEDScript);
      // }
      break;
  }
}

void setup(void)
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  pinMode(READING_LIGHT_PIN, OUTPUT);
  pinMode(CEILING_LIGHTS_PIN, OUTPUT); // Add this line
  pinMode(STANDING_LAMP_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("ESP"))
  { // esp.local/
    Serial.println("MDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", webpage); });

  server.on("/readingLight/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(READING_LIGHT_PIN, HIGH);
    request->send_P(200, "text/html", webpage); });

  server.on("/readingLight/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(READING_LIGHT_PIN, LOW);
    request->send_P(200, "text/html", webpage); });

  server.on("/ceilingLights/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(CEILING_LIGHTS_PIN, HIGH);
    request->send_P(200, "text/html", webpage); });

  server.on("/ceilingLights/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(CEILING_LIGHTS_PIN, LOW);
    request->send_P(200, "text/html", webpage); });

  server.on("/standingLamp/on", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(STANDING_LAMP_PIN, HIGH);
    request->send_P(200, "text/html", webpage); });

  server.on("/standingLamp/off", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    digitalWrite(STANDING_LAMP_PIN, LOW);
    request->send_P(200, "text/html", webpage); });

  server.onNotFound(notFound);

  server.begin();
  websockets.begin();
  websockets.onEvent(webSocketEvent);
}

void loop(void) {
  websockets.loop();

  if (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      double lat = gps.location.lat();
      double lon = gps.location.lng();


      String GPS_JSON_Data = "{\"type\":\"GPS\",\"lat\":";
      GPS_JSON_Data += String(lat, 6);
      GPS_JSON_Data += ",\"lon\":";
      GPS_JSON_Data += String(lon, 6);
      GPS_JSON_Data += ",\"date\":";
      GPS_JSON_Data += String()
      GPS_JSON_Data += "}";
      Serial.println(GPS_JSON_Data);
      websockets.broadcastTXT(GPS_JSON_Data);
    }
  }

  if (bme.performReading()) {
    float temperature = bme.temperature;
    float pressure = bme.pressure / 100.0; // convert to hPa
    float humidity = bme.humidity;
    int irValue = digitalRead(IR_SENSOR_PIN);

    String BME_JSON_Data = "{\"type\":\"BME\",\"temperature\":";
    BME_JSON_Data += String(temperature, 1);
    BME_JSON_Data += ",\"pressure\":";
    BME_JSON_Data += String(pressure, 1);
    BME_JSON_Data += ",\"humidity\":";
    BME_JSON_Data += String(humidity, 1);
    BME_JSON_Data += ",\"ir\":";
    BME_JSON_Data += String(irValue);
    BME_JSON_Data += "}";
    Serial.println(BME_JSON_Data);
    websockets.broadcastTXT(BME_JSON_Data);
  } else {
    Serial.println("Failed to read BME sensor data");
  }
  delay(1000);
}
