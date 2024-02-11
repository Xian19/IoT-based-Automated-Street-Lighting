#include <ESP8266WiFi.h>
 
 // ThingSpeak API key
String apiKey = "FK4ZE9TFZ4FVM1CF";     //  Enter your Write API key from ThingSpeak
 
 // Wi-Fi credentials
const char *ssid =  "HOKAZONO_FIBER_2";     // replace with your wifi ssid and wpa2 key
const char *pass =  "KeanReikaClyde2?";
const char* server = "api.thingspeak.com";
 
WiFiClient client;

int lightLevel = 0;
int isOn = 0;
 
void setup() 
{
  Serial.begin(115200);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Set pin modes
  pinMode (D0, OUTPUT);
  pinMode (D1, OUTPUT);
  pinMode (D2, OUTPUT);
  pinMode (D3, OUTPUT);
  pinMode (D4, OUTPUT);
  pinMode (A0, INPUT);
}
 
void loop() 
{
  // Read light level from analog pin
  lightLevel = analogRead(A0);

  // Determine LED state based on light level
  if(lightLevel <= 100) {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
    isOn = 5;
  }
  else if (lightLevel > 100 && lightLevel <= 236 ) {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW); 
    isOn = 4;
  }
  else if (lightLevel > 236 && lightLevel <= 441 ) {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW); 
    isOn = 3;
  }
  else if (lightLevel > 441 && lightLevel <= 646 ) {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW); 
    isOn = 2;
  }
  else if (lightLevel > 646 && lightLevel <= 852 ) {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW); 
    isOn = 1;
  }
  else { // Default case, turn off all LEDs
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW); 
    isOn = 0;
  }

  // Connect to ThingSpeak and send data
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {  
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(isOn);
    postStr +="&field2=";
    postStr += String(lightLevel);
    postStr += "\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    // Print debug information
    Serial.print("Number of LED: ");
    Serial.print(isOn);
    Serial.print(" Light level: ");
    Serial.print(lightLevel);
    Serial.println("%. Send to Thingspeak.");
  }
    // Disconnect from ThingSpeak
    client.stop();
 
    Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}