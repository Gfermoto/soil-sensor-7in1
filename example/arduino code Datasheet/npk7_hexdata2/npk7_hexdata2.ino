#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(4800); // NPK communicates at 4800 bps
}

void loop()
{
  byte queryData[] {0x01,0x03,0x00,0x00,0x00,0x07,0x04,0x08};
  byte receivedData[19];
  mySerial.write(queryData,sizeof(queryData)); // send query data to NPK
  delay(1000);
 if (mySerial.available() >= sizeof(receivedData)) {  // Check if there are enough bytes available to read
    mySerial.readBytes(receivedData, sizeof(receivedData));  // Read the received data into the receivedData array

  // Print the received data in HEX format
for (int i = 0; i < sizeof(receivedData); i++) {
      Serial.print(receivedData[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
