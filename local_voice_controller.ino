#include <ESP8266WiFi.h>
#include <AdafruitIO_WiFi.h>

// WiFi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"

// Adafruit IO credentials
#define AIO_USERNAME "YOUR_AIO_USERNAME"
#define AIO_KEY "YOUR_AIO_KEY"

// Pin definitions
#define LED_PIN 5        // External LED control pin (GPIO5/D1)
#define STATUS_LED 16    // Status LED (GPIO16/D0)

// Create AdafruitIO WiFi instance
AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);

// Create feed instance - replace "YOUR_FEED_NAME" with your actual feed name
AdafruitIO_Feed *lightFeed = io.feed("move-bitch");

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  
  // Start with light off
  digitalWrite(LED_PIN, LOW);
  digitalWrite(STATUS_LED, LOW);

  Serial.println("\n🚀 Starting ESP8266 Voice Control Light...");
  
  // Connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Set up message handler for the light feed
  lightFeed->onMessage(handleMessage);

  // Wait for connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  Serial.println("✅ Connected to Adafruit IO!");
  Serial.println("🎤 Ready to receive voice commands...");
  
  // Blink status LED to indicate ready
  for(int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }
}

void loop() {
  // Keep the connection alive
  io.run();
  
  // Optional: Blink status LED every 5 seconds to show it's alive
  static unsigned long lastBlink = 0;
  if(millis() - lastBlink > 5000) {
    digitalWrite(STATUS_LED, HIGH);
    delay(50);
    digitalWrite(STATUS_LED, LOW);
    lastBlink = millis();
  }
}

void handleMessage(AdafruitIO_Data *data) {
  String command = data->toString();
  command.toUpperCase();
  
  Serial.print("📨 Received command: ");
  Serial.println(command);
  
  if(command == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("💡 Light turned ON");
    
    // Blink status LED twice for ON
    for(int i = 0; i < 2; i++) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }
  else if(command == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("🌙 Light turned OFF");
    
    // Blink status LED once for OFF
    digitalWrite(STATUS_LED, HIGH);
    delay(300);
    digitalWrite(STATUS_LED, LOW);
  }
  else {
    Serial.println("❌ Unknown command: " + command);
  }
}
