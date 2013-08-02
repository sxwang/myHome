// 
//   FILE:  dht_test.pde
// PURPOSE: DHT library test sketch for Arduino
//

#include <dht.h>

dht DHT;

#define DHT22_INSIDE 5
#define DHT22_OUTSIDE 7

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("status, \tTemp1 (C), \tHum1 (%), \tTemp2 (C), \tHum2 (%)");
}

void loop()
{
  // READ DATA
  //Serial.print("DHT22, \t");
  int chk = DHT.read22(DHT22_INSIDE);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print(DHT.temperature, 1);
  Serial.print(",\t");
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");

  //delay(1000);
  
  
  int chk2 = DHT.read22(DHT22_OUTSIDE);
  switch (chk2)
  {
    case DHTLIB_OK:  
		//Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  Serial.print(DHT.temperature, 1);
  Serial.print(",\t");
  Serial.println(DHT.humidity, 1);

  delay(1000);
}
//
// END OF FILE
//
