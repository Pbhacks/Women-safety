#include <DHT.h>

// DHT11 Configuration
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Sonar Sensor Configuration
#define TRIGPIN 9
#define ECHOPIN 10

// Push Button
#define BUTTONPIN 7

// Buzzer and LED
#define BUZZERPIN 11
#define LEDPIN 13

// Variables for timing
unsigned long previousMillis = 0;  // Stores the last time sensors were read
const long interval = 5000;  // Interval to wait for automatic sensor readings (5 seconds)
unsigned long lastButtonPress = 0; // Track button press timing
const long debounceDelay = 200; // Debounce delay in milliseconds

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize DHT Sensor
  dht.begin();

  // Pin Modes
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP); // Button with internal pull-up resistor
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);

  Serial.println("System Initialized. Press the button to take readings.");
}

void loop() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Check if 5 seconds have passed (automatic monitoring)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Save the last time sensors were read
    takeReadings();  // Take automatic readings
  }

  // Check for button press to take manual readings
  if (digitalRead(BUTTONPIN) == LOW) {
    if (millis() - lastButtonPress > debounceDelay) {
      lastButtonPress = millis();  // Update last button press time
      Serial.println("Button pressed! Taking readings...");
      takeReadings();  // Take readings when button is pressed
    }
  }
}

// Function to take and display sensor readings
void takeReadings() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Measure distance using sonar sensor
  float distance = measureDistance();

  // Check for valid DHT readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor! Check connections.");
  } else {
    // Print values in a comma-separated format for easy parsing in Processing
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(distance);
  }

  // Alert logic for proximity
  if (distance < 15) { // If object is closer than 15 cm
    tone(BUZZERPIN, 1000); // Activate buzzer
    digitalWrite(LEDPIN, HIGH); // Turn on LED
    Serial.println("Proximity alert: Object too close!");
  } else {
    noTone(BUZZERPIN); // Deactivate buzzer
    digitalWrite(LEDPIN, LOW); // Turn off LED
  }
}

// Function to measure distance using sonar sensor
float measureDistance() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  float duration = pulseIn(ECHOPIN, HIGH);
  if (duration == 0) {
    return 999; // Return high value if no echo is detected
  }

  return (duration * 0.034) / 2; // Convert duration to distance in cm
}
