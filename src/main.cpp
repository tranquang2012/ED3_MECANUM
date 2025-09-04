#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// The following libraries are custom libraries.
// Header files (.h) contain function declarations, and are located in the include folder
// Sources (.cpp) contain function definitions, and are located in the src folder
#include "MySetup.h"      // Pin and variable definitions
#include "MyEncoder.h"    // Library for the encoder
#include "MyMotor.h"      // Library for the motor
#include "MyController.h" // Library for the controller
#include "MySerial.h"     // Library for serial communication

// ===================================================
// WiFi Configuration
// ===================================================

// Standard WiFi credentials - CHANGE THESE TO YOUR WIFI
const char* ssid = "Your_WiFi_Name";        // Replace with your actual WiFi network name
const char* password = "Your_WiFi_Password"; // Replace with your actual WiFi password

// WebSocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Create instances for all 4 motors
Encoder encoder1(ENC1_A, ENC1_B);                // Motor 1 - Front Left
Encoder encoder2(ENC2_A, ENC2_B);                // Motor 2 - Front Right  
Encoder encoder3(ENC3_A, ENC3_B);                // Motor 3 - Rear Left
Encoder encoder4(ENC4_A, ENC4_B);                // Motor 4 - Rear Right

Motor motor1(MOT1_A, MOT1_B, PWM1_A, PWM1_B);    // Motor 1 - Front Left
Motor motor2(MOT2_A, MOT2_B, PWM2_A, PWM2_B);    // Motor 2 - Front Right
Motor motor3(MOT3_A, MOT3_B, PWM3_A, PWM3_B);    // Motor 3 - Rear Left  
Motor motor4(MOT4_A, MOT4_B, PWM4_A, PWM4_B);    // Motor 4 - Rear Right

Controller controller1(&w1, &MOT1_cmd, &w1_ref); // Controller for Motor 1
Controller controller2(&w2, &MOT2_cmd, &w2_ref); // Controller for Motor 2
Controller controller3(&w3, &MOT3_cmd, &w3_ref); // Controller for Motor 3
Controller controller4(&w4, &MOT4_cmd, &w4_ref); // Controller for Motor 4

// ==============================================
// High-level movement functions
// ==============================================
void setMotorSpeeds(double frontLeft, double frontRight, double rearLeft, double rearRight) {
  w1_ref = frontLeft;   
  w2_ref = frontRight;  
  w3_ref = rearLeft;    
  w4_ref = rearRight;   
}

void moveForward(double speed) {
  setMotorSpeeds(speed, speed, speed, speed);
  Serial.println("Moving Forward at speed: " + String(speed));
}

void moveBackward(double speed) {
  setMotorSpeeds(-speed, -speed, -speed, -speed);
  Serial.println("Moving Backward at speed: " + String(speed));
}

void turnLeft(double speed) {
  setMotorSpeeds(speed * 0.5, speed, speed * 0.5, speed);  // Left wheels slower
  Serial.println("Turning Left at speed: " + String(speed));
}

void turnRight(double speed) {
  setMotorSpeeds(speed, speed * 0.5, speed, speed * 0.5);  // Right wheels slower
  Serial.println("Turning Right at speed: " + String(speed));
}

void spinLeft(double speed) {
  setMotorSpeeds(-speed, speed, -speed, speed);  // Left wheels reverse, right forward
  Serial.println("Spinning Left at speed: " + String(speed));
}

void spinRight(double speed) {
  setMotorSpeeds(speed, -speed, speed, -speed);  // Right wheels reverse, left forward
  Serial.println("Spinning Right at speed: " + String(speed));
}

void stopAllMotors() {
  setMotorSpeeds(0, 0, 0, 0);
  Serial.println("All Motors Stopped");
}

// WebSocket event handler
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      
      // Send initial status for all 4 motors
      String status = "{\"type\":\"status\"," +
                     String("\"motor1_speed\":") + String(w1) + 
                     ",\"motor1_target\":" + String(w1_ref) + 
                     ",\"motor1_pwm\":" + String(MOT1_cmd) + 
                     ",\"encoder1\":" + String(EncoderTick1) + 
                     ",\"motor2_speed\":" + String(w2) + 
                     ",\"motor2_target\":" + String(w2_ref) + 
                     ",\"motor2_pwm\":" + String(MOT2_cmd) + 
                     ",\"encoder2\":" + String(EncoderTick2) + 
                     ",\"motor3_speed\":" + String(w3) + 
                     ",\"motor3_target\":" + String(w3_ref) + 
                     ",\"motor3_pwm\":" + String(MOT3_cmd) + 
                     ",\"encoder3\":" + String(EncoderTick3) + 
                     ",\"motor4_speed\":" + String(w4) + 
                     ",\"motor4_target\":" + String(w4_ref) + 
                     ",\"motor4_pwm\":" + String(MOT4_cmd) + 
                     ",\"encoder4\":" + String(EncoderTick4) + "}";
      webSocket.sendTXT(num, status);
      break;
    }
    
    case WStype_TEXT: {
      Serial.printf("[%u] Received Text: %s\n", num, payload);
      
      // Parse JSON command with error handling
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      
      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        String errorResponse = "{\"type\":\"error\",\"message\":\"Invalid JSON\"}";
        webSocket.sendTXT(num, errorResponse);
        break;
      }
      
      String command = doc["command"];
      double speed = doc["speed"];
      
      Serial.println("Command: " + command + ", Speed: " + String(speed));
      
      // Individual motor control (legacy - now uses master speed system)
      if (command == "motor1") {
        w1_ref = speed;
        Serial.println("Motor 1 speed set to: " + String(w1_ref));
      }
      else if (command == "motor2") {
        w2_ref = speed;
        Serial.println("Motor 2 speed set to: " + String(w2_ref));
      }
      else if (command == "motor3") {
        w3_ref = speed;
        Serial.println("Motor 3 speed set to: " + String(w3_ref));
      }
      else if (command == "motor4") {
        w4_ref = speed;
        Serial.println("Motor 4 speed set to: " + String(w4_ref));
      }
      // Master speed control movement commands
      else if (command == "forward" || command == "moveForward") {
        moveForward(speed);
      }
      else if (command == "backward" || command == "moveBackward") {
        moveBackward(speed);
      }
      else if (command == "turnLeft") {
        turnLeft(speed);
      }
      else if (command == "turnRight") {
        turnRight(speed);
      }
      else if (command == "spinLeft") {
        spinLeft(speed);
      }
      else if (command == "spinRight") {
        spinRight(speed);
      }
      else if (command == "stop") {
        stopAllMotors();
      }
      else if (command == "getStatus") {
        // Frontend requesting status update - send current status
        Serial.println("Status update requested");
        String status = "{\"type\":\"status\"," +
                       String("\"motor1_speed\":") + String(w1) + 
                       ",\"motor1_target\":" + String(w1_ref) + 
                       ",\"motor1_pwm\":" + String(MOT1_cmd) + 
                       ",\"encoder1\":" + String(EncoderTick1) + 
                       ",\"motor2_speed\":" + String(w2) + 
                       ",\"motor2_target\":" + String(w2_ref) + 
                       ",\"motor2_pwm\":" + String(MOT2_cmd) + 
                       ",\"encoder2\":" + String(EncoderTick2) + 
                       ",\"motor3_speed\":" + String(w3) + 
                       ",\"motor3_target\":" + String(w3_ref) + 
                       ",\"motor3_pwm\":" + String(MOT3_cmd) + 
                       ",\"encoder3\":" + String(EncoderTick3) + 
                       ",\"motor4_speed\":" + String(w4) + 
                       ",\"motor4_target\":" + String(w4_ref) + 
                       ",\"motor4_pwm\":" + String(MOT4_cmd) + 
                       ",\"encoder4\":" + String(EncoderTick4) + "}";
        webSocket.sendTXT(num, status);
      }
      else {
        Serial.println("Unknown command: " + command);
      }
      
      // Send acknowledgment
      String response = "{\"type\":\"ack\",\"command\":\"" + command + "\"}";
      webSocket.sendTXT(num, response);
      break;
    }
    
    default:
      break;
  }
}

//==============================================
// Arduino Setup
//==============================================
void setup()
{
  // Initialize all 4 encoders
  encoder1.begin();    
  encoder2.begin();    
  encoder3.begin();    
  encoder4.begin();    
  
  // Initialize all 4 motors
  motor1.begin();      
  motor2.begin();      
  motor3.begin();      
  motor4.begin();      
  
  // Initialize all 4 controllers
  controller1.begin(); 
  controller2.begin(); 
  controller3.begin(); 
  controller4.begin(); 
  
  SerialBegin();       // Initialize the serial communication

#ifdef WEBSOCKET_CONTROL
  // Initialize WiFi with better error handling
  WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connect to: ws://");
    Serial.print(WiFi.localIP());
    Serial.println(":81");
  } else {
    Serial.println();
    Serial.println("WiFi connection failed! Check credentials.");
    Serial.println("Robot will work locally but no remote control.");
  }
  
  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
  Serial.println("=== WEBSOCKET CONTROL MODE ===");
  Serial.println("Use web interface for remote control");
#endif

#ifdef SERIAL_CONTROL
  Serial.println("=== SERIAL CONTROL MODE ===");
  Serial.println("Commands:");
  Serial.println("q5 = Motor 1 at 5 rad/s");
  Serial.println("w5 = Motor 2 at 5 rad/s"); 
  Serial.println("e5 = Motor 3 at 5 rad/s");
  Serial.println("r5 = Motor 4 at 5 rad/s");
  Serial.println("f5 = Forward at 5 rad/s");
  Serial.println("b5 = Backward at 5 rad/s");
  Serial.println("l5 = Turn left at 5 rad/s");
  Serial.println("g5 = Turn right at 5 rad/s");
  Serial.println("s = Stop all motors");
#endif
}

void loop()
{
#ifdef WEBSOCKET_CONTROL
  // Check WiFi connection and reconnect if needed
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > 10000) {  // Check every 10 seconds
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected, attempting reconnection...");
      WiFi.reconnect();
    }
    lastWiFiCheck = millis();
  }
  
  // Handle WebSocket connections (only if WiFi is connected)
  if (WiFi.status() == WL_CONNECTED) {
    webSocket.loop();
  }
#endif

  // Process all 4 motors
  EncoderTick1 = encoder1.getCount(); 
  w1 = encoder1.getVelocity();        
  controller1.compute();              
  motor1.send_pwm(MOT1_cmd);          
  
  EncoderTick2 = encoder2.getCount(); 
  w2 = encoder2.getVelocity();        
  controller2.compute();              
  motor2.send_pwm(MOT2_cmd);          
  
  EncoderTick3 = encoder3.getCount(); 
  w3 = encoder3.getVelocity();        
  controller3.compute();              
  motor3.send_pwm(MOT3_cmd);          
  
  EncoderTick4 = encoder4.getCount(); 
  w4 = encoder4.getVelocity();        
  controller4.compute();              
  motor4.send_pwm(MOT4_cmd);          
  
  SerialDataPrint();                  // Print the data to the Serial Monitor
  SerialDataWrite();                  // Write the data to the Serial Monitor
  
#ifdef WEBSOCKET_CONTROL
  // Send periodic status update via WebSocket for all motors (every 100ms)
  static unsigned long lastWebSocketUpdate = 0;
  if (WiFi.status() == WL_CONNECTED && millis() - lastWebSocketUpdate > 100) {
    String status = "{\"type\":\"status\"," +
                   String("\"motor1_speed\":") + String(w1) + 
                   ",\"motor1_target\":" + String(w1_ref) + 
                   ",\"motor1_pwm\":" + String(MOT1_cmd) + 
                   ",\"encoder1\":" + String(EncoderTick1) + 
                   ",\"motor2_speed\":" + String(w2) + 
                   ",\"motor2_target\":" + String(w2_ref) + 
                   ",\"motor2_pwm\":" + String(MOT2_cmd) + 
                   ",\"encoder2\":" + String(EncoderTick2) + 
                   ",\"motor3_speed\":" + String(w3) + 
                   ",\"motor3_target\":" + String(w3_ref) + 
                   ",\"motor3_pwm\":" + String(MOT3_cmd) + 
                   ",\"encoder3\":" + String(EncoderTick3) + 
                   ",\"motor4_speed\":" + String(w4) + 
                   ",\"motor4_target\":" + String(w4_ref) + 
                   ",\"motor4_pwm\":" + String(MOT4_cmd) + 
                   ",\"encoder4\":" + String(EncoderTick4) + "}";
    webSocket.broadcastTXT(status);
    lastWebSocketUpdate = millis();
  }
#endif
}
