#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the Serial port to communicate with ESP32
#define SerialPort Serial2  // Change this if you're using a different serial port

// Define the queries to send to the NPK sensor
uint8_t npkQuery[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD};
uint8_t phQuery[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
uint8_t soilMoistureQuery[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
uint8_t soilTemperatureQuery[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
uint8_t conductivityQuery[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};

// Define the LCD parameters
#define LCD_ADDRESS 0x27 // I2C address of the LCD
#define LCD_COLUMNS 20   // Number of columns
#define LCD_ROWS 4       // Number of rows

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); // Change the pins according to your ESP32's hardware serial pins

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // Read all values
  uint16_t nitrogen, phosphorus, potassium;
  float pH, moisture, temperature;
  uint16_t conductivity;

  // Read NPK values
  readNPKValues(nitrogen, phosphorus, potassium);
  
  // Read pH value
  pH = readPHValue();
  
  // Read soil moisture
  moisture = readSoilMoisture();
  
  // Read soil temperature
  temperature = readSoilTemperature();
  
  // Read conductivity value
  conductivity = readConductivity();

  // Display all values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("N:");
  lcd.print(nitrogen);
  lcd.print(" P:");
  lcd.print(phosphorus);
  lcd.print(" K:");
  lcd.print(potassium);
  
  lcd.setCursor(0, 1);
  lcd.print("pH:");
  lcd.print(pH);
  
  lcd.setCursor(0, 2);
  lcd.print("Moisture:");
  lcd.print(moisture);
  
  lcd.setCursor(0, 3);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print(" Cond:");
  lcd.print(conductivity);

  // Display all values on Serial Monitor
  Serial.print("N:");
  Serial.print(nitrogen);
  Serial.print(" P:");
  Serial.print(phosphorus);
  Serial.print(" K:");
  Serial.print(potassium);
  Serial.print(" pH:");
  Serial.print(pH);
  Serial.print(" Moisture:");
  Serial.print(moisture);
  Serial.print(" Temp:");
  Serial.print(temperature);
  Serial.print(" Cond:");
  Serial.println(conductivity);

  delay(1000); // Adjust delay according to your needs
}

void readNPKValues(uint16_t& nitrogen, uint16_t& phosphorus, uint16_t& potassium) {
  // Send query to the NPK sensor
  SerialPort.write(npkQuery, sizeof(npkQuery));

  // Wait for response from the sensor
  delay(100); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 11) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t npkResponse[11]; // Adjust the size based on the expected response length
    SerialPort.readBytes(npkResponse, sizeof(npkResponse));

    // Extract N, P, and K values from the received response
    nitrogen = (npkResponse[3] << 8) | npkResponse[4];
    phosphorus = (npkResponse[5] << 8) | npkResponse[6];
    potassium = (npkResponse[7] << 8) | npkResponse[8];
  }
}

float readPHValue() {
  // Send query to the NPK sensor
  SerialPort.write(phQuery, sizeof(phQuery));

  // Wait for response from the sensor
  delay(100); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 7) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t phResponse[7]; // Adjust the size based on the expected response length
    SerialPort.readBytes(phResponse, sizeof(phResponse));

    // Extract pH value from the received response
    uint16_t phValue = (phResponse[3] << 8) | phResponse[4];
    return phValue / 100.0; // Convert to decimal
  }

  return 0.0;
}

float readSoilMoisture() {
  // Send query to the NPK sensor
  SerialPort.write(soilMoistureQuery, sizeof(soilMoistureQuery));

  // Wait for response from the sensor
  delay(100); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 7) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t moistureResponse[7]; // Adjust the size based on the expected response length
    SerialPort.readBytes(moistureResponse, sizeof(moistureResponse));

    // Extract soil moisture value from the received response
    uint16_t moistureValue = (moistureResponse[3] << 8) | moistureResponse[4];
    return moistureValue / 10.0; // Convert to decimal
  }

  return 0.0;
}

float readSoilTemperature() {
  // Send query to the NPK sensor
  SerialPort.write(soilTemperatureQuery, sizeof(soilTemperatureQuery));

  // Wait for response from the sensor
  delay(100); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 7) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t temperatureResponse[7]; // Adjust the size based on the expected response length
    SerialPort.readBytes(temperatureResponse, sizeof(temperatureResponse));

    // Extract soil temperature value from the received response
    uint16_t temperatureValue = (temperatureResponse[3] << 8) | temperatureResponse[4];
    return temperatureValue / 10.0; // Convert to decimal
  }

  return 0.0;
}

uint16_t readConductivity() {
  // Send query to the NPK sensor
  SerialPort.write(conductivityQuery, sizeof(conductivityQuery));

  // Wait for response from the sensor
  delay(100); // Adjust delay as needed

  // Check if there's data available to read
  if (SerialPort.available() >= 7) { // Adjust the number based on the expected response length
    // Read the response
    uint8_t conductivityResponse[7]; // Adjust the size based on the expected response length
    SerialPort.readBytes(conductivityResponse, sizeof(conductivityResponse));

    // Extract conductivity value from the received response
    return (conductivityResponse[3] << 8) | conductivityResponse[4];
  }

  return 0;
}
