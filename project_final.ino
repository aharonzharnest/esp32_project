

//************************************************
//* project pinchas & aaron                      *
//************************************************

// Import required libraries
#include "WiFi.h"
#include <Wire.h>
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include <DHT.h>
#include <AsyncTCP.h>

// Replace with your network credentials
const char* ssid = "AndroidAP4414";
const char* password = "rgdr1264";

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

Adafruit_BMP085 bmp;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  double c = thermocouple.readCelsius();
  return String(c);
  //return messerd temputre
}


String readPres() {
  return String(bmp.readPressure() );
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class=>Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    c 
    <span class=>Pressure</span>
    <span id="pressure">%PRESSURE%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
 xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pressure").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pressure", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with bmp values
String processor(const String& var){
  Serial.println(var);
  if(var == "TEMPERATURE"){
    return readTemp();
  }
 if(var == "PRESSURE"){
    return readPres();
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);

  
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){  
  request->send_P(200, "text/plain", readPres().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}
