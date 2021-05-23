#include <Adafruit_NeoPixel.h>
#include "EspMQTTClient.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        12
#define NUMPIXELS 7
#include <ArduinoJson.h>

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

EspMQTTClient client(
  "viana2021",
  "vn8463ds",
  "test.mosquitto.org",  // MQTT Broker server ip
  "MyMedDevice",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  Serial.begin(115200);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();

  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

void turnOnLed(int led_number) {
  pixels.setPixelColor(led_number, pixels.Color(150, 0, 0));
  pixels.show();
}

void turnOffLed(int led_number) {
   pixels.setPixelColor(led_number, pixels.Color(0, 0, 0));
   pixels.show();
}
DynamicJsonDocument doc(1024);
// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
//  client.subscribe("mymed/turnonled", [](const String & payload) {
//    Serial.println(payload);
//    turnOnLed(3);
//  });

  client.subscribe("mymed/setled", [](const String & payload) {
    deserializeJson(doc, payload);
    if(doc["operation"] == "ON") {
      turnOnLed(doc["led"]);
    } else {
      turnOffLed(doc["led"]);
    }
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
//  client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
//    Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
//  });

  // Publish a message to "mytopic/test"
//  client.publish("mymed/pilltaken", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
//  client.executeDelayed(5 * 1000, []() {
//    client.publish("mymed/pilltaken", "This is a message sent 5 seconds later");
//  });
}

void loop() {
    client.loop();
}
