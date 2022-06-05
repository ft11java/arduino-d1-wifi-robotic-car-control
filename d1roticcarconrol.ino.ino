#include <ESP8266WiFi.h>

const char* ssid     = "Android";
const char* password = "********";

// Set web server port number to 80
WiFiServer server(80);
String header;
String in1 = "off";
String in2 = "off";
String in3 = "off";
String in4 = "off";

const int output6 = 13;
const int output7 = 12;
const int output5 = 14;
const int output9 = 02;

unsigned long currentTime = millis();

unsigned long previousTime = 0;

const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(output6, OUTPUT);
  pinMode(output7, OUTPUT);
  pinMode(output5, OUTPUT);
  pinMode(output9, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output6, LOW);
  digitalWrite(output7, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(output9, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   

  if (client) {                            
    Serial.println("New Client.");         
    String currentLine = "";               
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) {            
        char c = client.read();           
        Serial.write(c);                 
        header += c;
        if (c == '\n') {                   
         
          if (currentLine.length() == 0) {
          
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /ileri") >= 0) {
              Serial.println("ileri");
              digitalWrite(output6, HIGH);
              digitalWrite(output7, LOW);
              digitalWrite(output5, HIGH);
              digitalWrite(output9, LOW);
           
            } else if (header.indexOf("GET /geri") >= 0) {
              digitalWrite(output6, LOW);
              digitalWrite(output7, HIGH);
              digitalWrite(output5, LOW);
              digitalWrite(output9, HIGH);

            } else if (header.indexOf("GET /dur") >= 0) {
              digitalWrite(output6, LOW);
              digitalWrite(output7, LOW);
              digitalWrite(output5, LOW);
              digitalWrite(output9, LOW);

            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>Ayvaz Burckan CPAL</h1>");
            client.println("<body><h3>Bilisim Teknolojileri Bölümü 9/A Sınıfı</h3>");
            client.println("<p><a href=\"/ileri\"><button class=\"button\">İLERİ</button></a></p>");
            client.println("<p><a href=\"/geri\"><button class=\"button\">GERİ</button></a></p>");
            client.println("<p><a href=\"/dur\"><button class=\"button\">DUR</button></a></p>");
            client.println("</body></html>");

           
            client.println();
         
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;     
        }
      }
    }
  
    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
