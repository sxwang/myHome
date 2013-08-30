#include <SPI.h>         
#include <WiFi.h>
#include <HttpClient.h>
#include <Xively.h>
#include <dht.h>

dht DHT;

#define DHT22_INSIDE 5
#define DHT22_OUTSIDE 6
int led = 9;  // status led pin

int status = WL_IDLE_STATUS;
int attempts = 0;
char ssid[] = "SSID-HERE";  //  your network SSID (name)
char pass[] = "PASS-HERE";       // your network password

// Your Xively key to let you upload data
char xivelyKey[] = "XIVELY_KEY_HERE";
// Define the strings for our datastream IDs
char tempIn[] = "tempIn";
char humIn[] = "humIn";
char tempOut[] = "tempOut";
char humOut[] = "humOut";
XivelyDatastream datastreams[] = {
  XivelyDatastream(tempIn, strlen(tempIn), DATASTREAM_FLOAT),
  XivelyDatastream(humIn, strlen(humIn), DATASTREAM_FLOAT),
  XivelyDatastream(tempOut, strlen(tempOut), DATASTREAM_FLOAT),
  XivelyDatastream(humOut, strlen(humOut), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(XIVELY_FEED_HERE, datastreams, 4);

WiFiClient client;
XivelyClient xivelyclient(client);


void setup()
{
  Serial.begin(9600);
  Serial.println("Starting up");
  pinMode(led, OUTPUT);

  // attempt to connect to Wifi network:
  while(!Serial) ;
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECT_FAILED) {
      Serial.println("connection failed, retrying in 10 seconds");
    }
      // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  Serial.flush();
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
  float tIn = DHT.temperature;
  float hIn = DHT.humidity;

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
  float tOut = DHT.temperature;
  float hOut = DHT.humidity;

  delay(1000);
  
  // send data to Xively
  datastreams[0].setFloat(tIn);
  datastreams[1].setFloat(hIn);
  datastreams[2].setFloat(tOut);
  datastreams[3].setFloat(hOut);

  Serial.println("Uploading data to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);
  
  // blink the LED so we know it's alive
  digitalWrite(led, LOW); 
  delay(30000);  
  digitalWrite(led, HIGH);  
  
  Serial.flush();
  
}
//
// END OF FILE
//
