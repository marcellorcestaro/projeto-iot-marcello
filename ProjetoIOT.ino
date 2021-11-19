#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "GallifreyStands2G";
const char* password = "AdorableHome@311089";

ESP8266WebServer server(80);

String page;

// defines pins numbers
const int trigPin = 4;
const int echoPin = 15;
// defines variables
long duration;
int distance;
int colisoes = 0;

int motorLeftFwd = 12;
int motorLeftBck = 13;

int motorRightBck = 5;
int motorRightFwd = 16;

void setup() {
  page = "<h1>Simple NodeMCU Web Server</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(motorRightBck, OUTPUT);
  pinMode(motorRightFwd, OUTPUT);
  pinMode(motorLeftFwd, OUTPUT);
  pinMode(motorLeftBck, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(LED_BUILTIN, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", [](){
    server.send(200, "text/html", SendHTML(distance, colisoes));
  });
  server.begin();
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  
  delay(100);
  
  server.handleClient();

  if(distance > 19){
    goForward();
  }

  if(distance < 18){
    colisoes += 1;
    stopWheels(); 
    delay(500);
    goBackwards();
    delay(500);
    goRight();
    delay(600); 
  }

}

void goForward(){
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorLeftFwd, HIGH);
  
  digitalWrite(motorRightBck, LOW);
  digitalWrite(motorLeftBck, LOW);
}

void goBackwards(){
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorLeftFwd, LOW);
  
  digitalWrite(motorRightBck, HIGH);
  digitalWrite(motorLeftBck, HIGH);
}

void stopWheels(){
  digitalWrite(motorRightFwd, LOW);
  digitalWrite(motorLeftFwd, LOW);
  
  digitalWrite(motorRightBck, LOW);
  digitalWrite(motorLeftBck, LOW);
}

void goRight(){
  digitalWrite(motorRightFwd, HIGH);
  digitalWrite(motorLeftFwd, LOW);
  
  digitalWrite(motorRightBck, LOW);
  digitalWrite(motorLeftBck, LOW);
}

String SendHTML(int dist, int colisao){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Obstacle Avoiding Robot Info</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Deteccao de colisoes</h1>\n";
  ptr +="<h3>Nos tivemos "+ String(colisao) +" colisao</h3>\n";
  ptr +="<h3>Estamos com uma distancia de "+ String(dist) +" centimetros\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
