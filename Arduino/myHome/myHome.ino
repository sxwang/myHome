
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
char ssid[] = "SSID";  //  your network SSID (name)
char pass[] = "PASS";       // your network password

// Your Xively key to let you upload data
char xivelyKey[] = "XIVELYKEY";
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
XivelyFeed feed(XIVELYFEED, datastreams, 4);

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
    attempts++;
    if (attempts>5) {
      Serial.println("max 5 attempts. Maybe already connected?");
      break;
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
  int chk = DHT.read22(DHT22_INSIDE);
  if (chk==DHTLIB_OK) {
    Serial.print("OK,\t"); 
  }
  else{
    Serial.print("Error,\t"); 
  }
  float tIn = DHT.temperature;
  float hIn = DHT.humidity;
  int chk2 = DHT.read22(DHT22_OUTSIDE);
  if (chk2!=DHTLIB_OK){
    Serial.print("Error,\t"); 
  }
  float tOut = DHT.temperature;
  float hOut = DHT.humidity;
  Serial.println(String(tIn)+"\t"+String(tOut)+"\t"+String(hIn)+"\t"+String(hOut));
  delay(1000);
  
  // send data to Xively
  datastreams[0].setFloat(tIn);
  datastreams[1].setFloat(hIn);
  datastreams[2].setFloat(tOut);
  datastreams[3].setFloat(hOut);

  if (chk==DHTLIB_OK && chk2==DHTLIB_OK) {
    Serial.println("Uploading data to Xively");
    int ret = xivelyclient.put(feed, xivelyKey);
    Serial.print("xivelyclient.put returned ");
    Serial.println(ret);
  }
  else {
    Serial.println("bad data, not uploaded");
  }
  
  // blink the LED so we know it's alive
  digitalWrite(led, LOW); 
  delay(30000);  
  digitalWrite(led, HIGH);  
  
  Serial.flush();
  
}
//
// END OF FILE
//
