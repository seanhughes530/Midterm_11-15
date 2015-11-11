//temp and hum sensor info
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


//uv sensor info
int UVOUT = 4; 
int REF_3V3 = 6;

//rfduino info
#include <RFduinoGZLL.h>
device_t role = DEVICE1;

void setup() {
  Serial.begin(9600);

  //begin temp and hum sensor
  dht.begin();

  //uv sensor
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

  //rfgzll
  RFduinoGZLL.txPowerLevel = 0;
  RFduinoGZLL.begin(role);
}

void loop() {
  
  /////////////temp and hum sensor////////////
  delay(2000);

  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hif);
  Serial.println(" *F");


  /////////////////uv sensor/////////////////////////
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level

  Serial.print("output: ");
  Serial.print(refLevel);

  Serial.print("ML8511 output: ");
  Serial.print(uvLevel);

  Serial.print(" / ML8511 voltage: ");
  Serial.print(outputVoltage);

  Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.print(uvIntensity);
  
  Serial.println();
  
  delay(100);



  ///////////////////info for sending/////////////////////
  char hdata[3];   //declaring character array
  char tdata[3];
  char hedata[3];
  char uvdata[4];
  char data[16];

  String hstr;
  String tstr;
  String hestr;//declaring string
  String uvstr;
  String mydata;

  int hum;
  int temp;
  int hein;
  int uvin;

  hum = h;
  temp = f;
  hein = hif;
  uvin = uvIntensity;

  hstr = String (hum);
  tstr = String(temp);
  hestr = String(hein);
  uvstr = String(uvin);

  mydata = hstr+tstr+hestr+uvstr;
  mydata.toCharArray(data,16);
  
  Serial.println(data);

  RFduinoGZLL.sendToHost(data,16);
  delay(250);
}


int averageAnalogRead(int pinToRead){
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}  

