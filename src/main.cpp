//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <SinricPro.h>
#include <SinricProSwitch.h>

#define WIFI_SSID         "YOUR-WIFI-SSID"    // your own wifi network 
#define WIFI_PASS         "YOUR-WIFI-PASS"    // wifi network password
#define APP_KEY           "YOUR-APP-KEY"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "YOUR-APP-SECRET"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define SWITCH_ID         "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"


#define BAUD_RATE          9600       

// Board: Sonoff RE5V1C 5V (Based on ESP8285)
// https://templates.blakadder.com/sonoff_RE5V1C.html
// GPIO00	Button1, GPIO12	Relay1, GPIO13	Led1i
#define BUTTON_PIN        0
#define RELAY_PIN         12                
#define LED_PIN           13

#define TOGGLE_LED !digitalRead(LED_PIN)


bool buttonState = false;  
bool lastButtonState = true;  
bool relayState = false;


void relayOn()
{
  Serial.println("Heater Turned ON");                                                        
  digitalWrite(RELAY_PIN, HIGH); 
  delay(500); //0.5 sec pulse
  digitalWrite(RELAY_PIN, LOW);    
}
void relayOff()
{
  Serial.println("Heater Turned OFF");
  digitalWrite(RELAY_PIN, HIGH); 
  delay(500); //0.5 sec pulse
  digitalWrite(RELAY_PIN, LOW);  
}


bool onPowerState(const String &deviceId, bool &state) 
{
  if(state && !relayState)
  { 
    relayOn();  
    relayState = true;  
  }
  else if(!state && relayState)
  { 
    relayOff(); 
    relayState = false; 
  }
  //Serial.printf("device %s turned %s\r\n", deviceId.c_str(), state?"on":"off");
  return true;                                // request handled properly
}

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);                 // set relay-pin to output mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  delay(1000);
  Serial.begin(BAUD_RATE);

  
  WiFi.begin(WIFI_SSID, WIFI_PASS);           // start wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    digitalWrite(LED_PIN, TOGGLE_LED);
    delay(500);
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi successful");
  IPAddress localIP = WiFi.localIP();
  Serial.printf("[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);

  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];   // create new switch device
  mySwitch.onPowerState(onPowerState);                // apply onPowerState callback
  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  //SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET); // start SinricPro
  
  digitalWrite(LED_PIN, LOW); // status led always on
}

void loop() 
{
  // if disconnected wifi. try to connect again
  if (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, HIGH); // connection problem turn off led
    Serial.print("Trying to connect ");
    Serial.print(WIFI_SSID);
    Serial.println("...");
    WiFi.begin(WIFI_SSID, WIFI_PASS); 
    delay(500);
    
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi Connected");
    digitalWrite(LED_PIN, LOW); // connected again turn on led
  }
  
  // handle SinricPro commands
  SinricPro.handle(); // handle SinricPro commands

  //handel button commands
  buttonState = digitalRead(BUTTON_PIN);
  if(!buttonState && (buttonState != lastButtonState))
  {
    if(!relayState)
    {
      relayOn();
      relayState = true;
    }
    else if (relayState)
    {
      relayOff();
      relayState = false;
    } 
  }
  lastButtonState = buttonState;
}