#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define LED_PIN D0
const char* ssid = "*******";
const char* password = "********";

ESP8266WebServer server(80);
String LightStatus = "0";
String Prv="";
void ChangeLight() {
  LightStatus = server.arg("value");
  if (LightStatus == "1") {
    if(LightStatus!=Prv){
      Serial.println("Light ON");
      Prv=LightStatus;
    }
    digitalWrite(LED_BUILTIN, LOW);
  } else if (LightStatus == "0") {
    if(LightStatus!=Prv){
      Serial.println("Light Off");
      Prv=LightStatus;
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    BlinkLight();
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Value received: " + LightStatus);
}

void GetValue() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", LightStatus);
}
int checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection Not Found..");
    delay(1000);
    return false;
  }
  return true;
}
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (!checkWiFiConnection()) {
    Serial.println("Connecting..");
    for (int li = 0; li <= 10; li++) {
      BlinkLight();
    }
  }
  Serial.println("Connected to WiFi");
  server.on("/LightChange", ChangeLight);
  server.on("/GetValue", GetValue);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

void BlinkLight() {
  digitalWrite(LED_PIN, LOW);
  delay(125);
  digitalWrite(LED_PIN, HIGH);
  delay(125);
  digitalWrite(LED_PIN, LOW);
  delay(125);
  digitalWrite(LED_PIN, HIGH);
}