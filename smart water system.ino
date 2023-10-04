#include <LiquidCrystal.h> // Include the LCD library if using an LCD
#include <WiFiClient.h>     // Include the Wi-Fi library if using ESP8266
#include <ESP8266WiFi.h>    // Include the ESP8266 Wi-Fi library
#include <Wire.h>

// Pin Definitions
const int waterSensorPin = A0; // Analog input for water level sensor
const int buzzerPin = 7;       // Digital pin for buzzer
const int relayPin = 8;        // Digital pin for relay control (pump/valve)
const int lcdPin = 12;         // Digital pin for LCD if used

// Wi-Fi Settings
const char* ssid = "YourSSID"; // Replace with your Wi-Fi network SSID
const char* password = "YourPassword"; // Replace with your Wi-Fi network password
const char* serverAddress = "example.com"; // Replace with your server address if sending data

// LCD Configuration (if used)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // Initialize LCD (if used)
  lcd.begin(16, 2); // Adjust these parameters for your LCD
  
  // Connect to Wi-Fi (if using ESP8266)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Other setup code here
}

void loop() {
  // Read water level sensor value
  int waterLevel = analogRead(waterSensorPin);
  
  // Check water level and take action
  if (waterLevel < 500) { // Adjust this threshold to your needs
    digitalWrite(buzzerPin, HIGH); // Activate buzzer
    digitalWrite(relayPin, HIGH);  // Activate relay to turn on the pump/valve
    lcd.setCursor(0, 0);
    lcd.print("Water Level: Low");
  } else if (waterLevel > 800) { // Adjust this threshold to your needs
    digitalWrite(buzzerPin, HIGH); // Activate buzzer
    digitalWrite(relayPin, LOW);   // Deactivate relay to turn off the pump/valve
    lcd.setCursor(0, 0);
    lcd.print("Water Level: High");
  } else {
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    lcd.setCursor(0, 0);
    lcd.print("Water Level: OK");
  }

  // Send data to a server (if using Wi-Fi)
  sendDataToServer(waterLevel);

  delay(1000); // Delay for stability
}

void sendDataToServer(int value) {
  // Create a client instance (if using ESP8266)
  WiFiClient client;

  if (client.connect(serverAddress, 80)) {
    // Construct the HTTP request
    String postData = "value=" + String(value);
    client.println("POST /your-endpoint HTTP/1.1");
    client.println("Host: " + String(serverAddress));
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);

    // Read the server's response (if needed)
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    client.stop(); // Close the connection
  } else {
    Serial.println("Failed to connect to server");
  }
}
