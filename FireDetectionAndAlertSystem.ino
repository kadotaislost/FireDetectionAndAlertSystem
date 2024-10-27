#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Pin Definitions
#define DHTPIN 2          // DHT11 sensor pin
#define DHTTYPE DHT11     // DHT sensor type
#define MQ2_PIN A0        // MQ2 gas sensor pin
#define RED_LED 3         // Red LED pin
#define GREEN_LED 4       // Green LED pin
#define BUZZER 5          // Buzzer pin
#define MOTOR1 8          // First motor relay pin
#define MOTOR2 9          // Second motor relay pin

// Constants
const int smokeThreshold = 300;       // threshold for MQ2 sensor
const float tempThreshold = 50.0;     // temperature threshold for DHT11 (in °C)

// SoftwareSerial for SIM800L and LiquidCrystal_I2C for LCD
SoftwareSerial sim800l(10, 11);         // RX, TX for SIM800L
LiquidCrystal_I2C lcd(0x27, 16, 2);   // LCD I2C address, 16 columns, 2 rows
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize components
  pinMode(MQ2_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
  sim800l.begin(9600);

  // Turn off relays
  digitalWrite(MOTOR1, HIGH); 
  digitalWrite(MOTOR2, HIGH); 

  // Initialize sensors and display
  dht.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  // Initial LCD message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
}

void loop() {
  // Read DHT11 sensor values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read MQ2 sensor value
  int smokeLevel = analogRead(MQ2_PIN);

  // Display sensor readings on LCD
  lcd.clear();
  displayStatus(temperature, smokeLevel);

  // Fire detection logic
  if (isFireDetected(smokeLevel, temperature)) {
    activateFireAlert(); // Activate alert for fire detected
    sendSMSAlert();      // Send SMS alert
  } else {
    resetAlert();        // Reset alerts if no fire is detected
  }

  delay(500);  // Delay for sensor update
}

// Function to check if fire is detected
bool isFireDetected(int smokeLevel, float temperature) {
  return smokeLevel > smokeThreshold || temperature > tempThreshold;
}

// Function to display status on LCD
void displayStatus(float temperature, int smokeLevel) {
  lcd.setCursor(0, 0);
  if (isFireDetected(smokeLevel, temperature)) {
    lcd.print("Status: FIRE");
  } else {
    lcd.print("Status: Normal");
  }

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);  // Print temperature
  lcd.print(" C  S:");
  lcd.print(smokeLevel);   // Print smoke level
}

// Function to activate alerts for fire detection
void activateFireAlert() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, HIGH);
  digitalWrite(MOTOR1, LOW); 

  // Control submersible water pump (MOTOR2)
  for (int i = 0; i < 4; i++) {  // Adjust the number of cycles as needed
    digitalWrite(MOTOR2, LOW);    // Turn on the pump
    delay(200);                   // Pump runs for 2 seconds (adjust as needed)
    digitalWrite(MOTOR2, HIGH);   // Turn off the pump
    delay(2000);                  // Pump off for 3 seconds (adjust as needed)
  }
}

// Function to reset alerts
void resetAlert() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, LOW);
  digitalWrite(MOTOR1, HIGH);
  digitalWrite(MOTOR2, HIGH);
}

// Function to send SMS alert
void sendSMSAlert() {
  sim800l.println("AT+CMGF=1"); // Set SMS to text mode
  delay(100);
  sim800l.print("AT+CMGS=\"+9779845353156\"\r");
  delay(100);
  sim800l.print("Fire detected! Temperature and smoke levels are high. Evacuate Immediately");
  delay(100);
  sim800l.write(26); // Send CTRL+Z to send SMS
}


