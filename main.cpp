#include <ESP8266WiFi.h>  // Для ESP8266
// #include <WiFi.h>      // Для ESP32

const char* ssid = "SmartHome_Gateway";
const char* password = "12345678";

WiFiServer server(80);

#ifdef ESP8266
  const int ledPin = LED_BUILTIN;  // GPIO2
#else
  const int ledPin = 2;
#endif

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // выключен

  WiFi.softAP(ssid, password);
  Serial.println("AP запущена");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/on") != -1) {
    digitalWrite(ledPin, LOW);
  } else if (request.indexOf("/off") != -1) {
    digitalWrite(ledPin, HIGH);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta charset='UTF-8'>");
  client.println("<style>button{font-size:24px;margin:10px;}</style>");
  client.println("</head><body>");
  client.println("<h1>Управление LED</h1>");
  client.println("<button onclick='location.href=\"/on\"'>Включить</button>");
  client.println("<button onclick='location.href=\"/off\"'>Выключить</button>");
  client.println("</body></html>");

  delay(10);
}
