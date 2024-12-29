import processing.serial.*;

Serial myPort;  // Create a serial port object
String sensorData = "";  // String to store incoming data from Arduino
float temperature = 0;
float humidity = 0;
float distance = 0;

void setup() {
  // Open the serial port (change "COM3" to your actual Arduino port)
  myPort = new Serial(this, "COM3", 9600);  // Ensure the port name is correct
  
  // Set the window size
  size(600, 400);
  background(255);
  
  // Display the title on the UI
  fill(0);
  textSize(16);
  text("Sensor Data Visualization", 10, 30);
}

void draw() {
  // Clear background on every frame
  background(255);
  
  // Display title
  fill(0);
  textSize(16);
  text("Sensor Data Visualization", 10, 30);
  
  // Display the temperature, humidity, and distance
  textSize(14);
  text("Temperature: " + temperature + " °C", 10, 70);
  text("Humidity: " + humidity + " %", 10, 100);
  text("Distance: " + distance + " cm", 10, 130);
  
  // Visualize distance as a bar (dynamic height of a rectangle)
  float distanceHeight = map(distance, 0, 400, 0, height - 150);
  fill(0, 150, 255);
  rect(10, height - distanceHeight, 30, distanceHeight);
  
  // Visualize proximity alert
  if (distance < 15) {  // Object too close
    fill(255, 0, 0);
    text("Proximity alert: Object too close!", 10, 160);
  } else {
    fill(0, 255, 0);
    text("Proximity alert: Object safe", 10, 160);
  }
}

// This function is called whenever data is received from Arduino
void serialEvent(Serial myPort) {
  // Read the incoming serial data
  sensorData = myPort.readStringUntil('\n'); // Read until newline character
  
  // If data is received, split it into temperature, humidity, and distance
  if (sensorData != null) {
    String[] values = split(sensorData, ',');  // Split by commas
    
    // Check if we got three values (temperature, humidity, distance)
    if (values.length == 3) {
      temperature = float(values[0]);  // Temperature in Celsius
      humidity = float(values[1]);     // Humidity in percentage
      distance = float(values[2]);     // Distance in centimeters
    }
  }
}
