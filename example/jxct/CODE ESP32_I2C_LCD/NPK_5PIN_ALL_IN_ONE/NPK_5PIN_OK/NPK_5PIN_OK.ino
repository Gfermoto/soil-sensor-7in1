// Define the Serial port to communicate with ESP32
#define SerialPort Serial2  // Change this if you're using a different serial port

// Define the queries to send to the NPK sensor
uint8_t npkQuery[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD};
uint8_t phQuery[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
uint8_t soilMoistureQuery[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
uint8_t soilTemperatureQuery[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
uint8_t conductivityQuery[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};

void setup()
{
    Serial.begin(9600);
    SerialPort.begin(9600, SERIAL_8N1, 16, 17);  // Change the pins according to your ESP32's hardware serial pins
}

void loop()
{
    // Read NPK values
    printNPKValues();
    delay(1000);  // Adjust delay according to your needs

    // Read pH value
    printPHValue();
    delay(1000);  // Adjust delay according to your needs

    // Read soil moisture
    printSoilMoisture();
    delay(1000);  // Adjust delay according to your needs

    // Read soil temperature
    printSoilTemperature();
    delay(1000);  // Adjust delay according to your needs

    // Read conductivity value
    printConductivity();
    delay(1000);  // Adjust delay according to your needs
}

void printNPKValues()
{
    // Send query to the NPK sensor
    SerialPort.write(npkQuery, sizeof(npkQuery));

    // Wait for response from the sensor
    delay(100);  // Adjust delay as needed

    // Check if there's data available to read
    if (SerialPort.available() >= 11)
    {  // Adjust the number based on the expected response length
        // Read the response
        uint8_t npkResponse[11];  // Adjust the size based on the expected response length
        SerialPort.readBytes(npkResponse, sizeof(npkResponse));

        // Extract N, P, and K values from the received response
        uint16_t nitrogen = (npkResponse[3] << 8) | npkResponse[4];
        uint16_t phosphorus = (npkResponse[5] << 8) | npkResponse[6];
        uint16_t potassium = (npkResponse[7] << 8) | npkResponse[8];

        // Print the values
        Serial.println("NPK Values:");
        Serial.print("Nitrogen: ");
        Serial.println(nitrogen);
        Serial.print("Phosphorus: ");
        Serial.println(phosphorus);
        Serial.print("Potassium: ");
        Serial.println(potassium);
    }
}

void printPHValue()
{
    // Send query to the NPK sensor
    SerialPort.write(phQuery, sizeof(phQuery));

    // Wait for response from the sensor
    delay(100);  // Adjust delay as needed

    // Check if there's data available to read
    if (SerialPort.available() >= 7)
    {  // Adjust the number based on the expected response length
        // Read the response
        uint8_t phResponse[7];  // Adjust the size based on the expected response length
        SerialPort.readBytes(phResponse, sizeof(phResponse));

        // Extract pH value from the received response
        uint16_t phValue = (phResponse[3] << 8) | phResponse[4];
        float pH = phValue / 100.0;  // Convert to decimal

        // Print the value
        Serial.println("pH Value:");
        Serial.println(pH);
    }
}

void printSoilMoisture()
{
    // Send query to the NPK sensor
    SerialPort.write(soilMoistureQuery, sizeof(soilMoistureQuery));

    // Wait for response from the sensor
    delay(100);  // Adjust delay as needed

    // Check if there's data available to read
    if (SerialPort.available() >= 7)
    {  // Adjust the number based on the expected response length
        // Read the response
        uint8_t moistureResponse[7];  // Adjust the size based on the expected response length
        SerialPort.readBytes(moistureResponse, sizeof(moistureResponse));

        // Extract soil moisture value from the received response
        uint16_t moistureValue = (moistureResponse[3] << 8) | moistureResponse[4];
        float moisture = moistureValue / 10.0;  // Convert to decimal

        // Print the value
        Serial.println("Soil Moisture:");
        Serial.println(moisture);
    }
}

void printSoilTemperature()
{
    // Send query to the NPK sensor
    SerialPort.write(soilTemperatureQuery, sizeof(soilTemperatureQuery));

    // Wait for response from the sensor
    delay(100);  // Adjust delay as needed

    // Check if there's data available to read
    if (SerialPort.available() >= 7)
    {  // Adjust the number based on the expected response length
        // Read the response
        uint8_t temperatureResponse[7];  // Adjust the size based on the expected response length
        SerialPort.readBytes(temperatureResponse, sizeof(temperatureResponse));

        // Extract soil temperature value from the received response
        uint16_t temperatureValue = (temperatureResponse[3] << 8) | temperatureResponse[4];
        float temperature = temperatureValue / 10.0;  // Convert to decimal

        // Print the value
        Serial.println("Soil Temperature:");
        Serial.println(temperature);
    }
}

void printConductivity()
{
    // Send query to the NPK sensor
    SerialPort.write(conductivityQuery, sizeof(conductivityQuery));

    // Wait for response from the sensor
    delay(100);  // Adjust delay as needed

    // Check if there's data available to read
    if (SerialPort.available() >= 7)
    {  // Adjust the number based on the expected response length
        // Read the response
        uint8_t conductivityResponse[7];  // Adjust the size based on the expected response length
        SerialPort.readBytes(conductivityResponse, sizeof(conductivityResponse));

        // Extract conductivity value from the received response
        uint16_t conductivityValue = (conductivityResponse[3] << 8) | conductivityResponse[4];

        // Print the value
        Serial.println("Conductivity:");
        Serial.println(conductivityValue);
    }
}
