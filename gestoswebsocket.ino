/*
 * WebSocketClientSocketIOack.ino
 *
 *  Created on: 20.07.2019
 *
 */

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
//#include <FastLED.h>
#include <SparkFun_APDS9960.h>

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial

// Pins
//#define APDS9960_INT 25 // Needs to be an interrupt pin
#define APDS9960_INT 18 // Needs to be an interrupt pin
#define DATA_PIN 26

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
bool Gesture_Up, Gesture_Down, Gesture_Left, Gesture_Right, Gesture_Far, Gesture_Near;

// Functions
void interruptRoutine();
void handleGesture();

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
        {
            char * sptr = NULL;
            int id = strtol((char *)payload, &sptr, 10);
            USE_SERIAL.printf("[IOc] get event: %s id: %d\n", payload, id);
            if(id) {
                payload = (uint8_t *)sptr;
            }
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, payload, length);
            if(error) {
                USE_SERIAL.print(F("deserializeJson() failed: "));
                USE_SERIAL.println(error.c_str());
                return;
            }

            String eventName = doc[0];
            USE_SERIAL.printf("[IOc] event name: %s\n", eventName.c_str());

            // Message Includes a ID for a ACK (callback)
            if(id) {
                // creat JSON message for Socket.IO (ack)
                DynamicJsonDocument docOut(1024);
                JsonArray array = docOut.to<JsonArray>();

                // add payload (parameters) for the ack (callback function)
                JsonObject param1 = array.createNestedObject();
                param1["now"] = millis();

                // JSON to String (serializion)
                String output;
                output += id;
                serializeJson(docOut, output);

                // Send event
                socketIO.send(sIOtype_ACK, output);
            }
        }
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            break;
    }
}

void setup() {
    //USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    // ---------------- APDS 9960 ----------------
  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true))
  {
    Serial.println(F("Gesture sensor is now running"));

    // if (apds.setLEDDrive(0))
    // {
    //   Serial.println("LED Drive Set");
    // }
    if (apds.setGestureGain(GGAIN_2X))
    {
      Serial.println("Gesture Gain Set");
    }
  }
  else
  {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    //WiFiMulti.addAP("RED ACCESA", "037E32E7");
    WiFiMulti.addAP("ACCESA MOVIL", "123456789A");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
   socketIO.begin("192.168.147.197", 3000, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
}
unsigned long messageTimestamp = 0;

void loop() {

  if (isr_flag == 1)
  {
    detachInterrupt(APDS9960_INT);
     Serial.println("Interrupt Function");
    handleGesture();
   // ControlLEDStrip();
    isr_flag = 0;
    attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);
  }
    socketIO.loop();

    uint64_t now = millis();

    if(now - messageTimestamp > 2000) {
        messageTimestamp = now;
        isr_flag = 0;


        

        // creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();

        // add evnet name
        // Hint: socket.on('event_name', ....
        array.add("event_name");

        // add payload (parameters) for the event
        JsonObject param1 = array.createNestedObject();
        //param1["ACTION"] = (uint32_t) now;
         param1["action"] = "HEART BEAT!";

        // JSON to String (serializion)
        String output;
        serializeJson(doc, output);

        // Send event
       // socketIO.sendEVENT(output);

        // Print JSON for debugging
        USE_SERIAL.println(output);
    }
}


void sendsocketio(String datos){
// creat JSON message for Socket.IO (event)
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();

        // add evnet name
        // Hint: socket.on('event_name', ....
        array.add("event_name");

        // add payload (parameters) for the event
        JsonObject param1 = array.createNestedObject();
        //param1["ACTION"] = (uint32_t) now;
         param1["action"] = datos;

        // JSON to String (serializion)
        String output;
        serializeJson(doc, output);

        // Send event
        socketIO.sendEVENT(output);

        // Print JSON for debugging
        USE_SERIAL.println(output); 
}


void interruptRoutine()
{
  isr_flag = 1;
}

void handleGesture()
{
  bool GestureAvailability = apds.isGestureAvailable();
   Serial.print("Gesture Avail : ");
   Serial.println(GestureAvailability);

  if (GestureAvailability)
  {
    int GestureType = apds.readGesture();
     Serial.print("Gesture Type : ");
     Serial.println(GestureType);

    switch (GestureType)
    {
    case DIR_UP:
      Serial.println("UP");
      sendsocketio("UP");
      Gesture_Up = true;
      break;
    case DIR_DOWN:
      Serial.println("DOWN");
      sendsocketio("DOWN");
      Gesture_Down = true;
      break;
    case DIR_LEFT:
      Serial.println("LEFT");
      sendsocketio("LEFT");
      Gesture_Left = true;
      break;
    case DIR_RIGHT:
      Serial.println("RIGHT");
      sendsocketio("RIGHT");
      Gesture_Right = true;
      break;
    case DIR_NEAR:
      Serial.println("NEAR");
      sendsocketio("NEAR");
      Gesture_Near = true;
      break;
    case DIR_FAR:
      Serial.println("FAR");
      sendsocketio("FAR");
      Gesture_Far = true;
      break;
    default:
      Serial.println("NONE");
      sendsocketio("NONE");
    }
  }
}
