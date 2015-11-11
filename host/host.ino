#include <RFduinoGZLL.h>
device_t role = HOST;

char state = 0;

String Data;
String hstr;
String tstr;
String hestr;//declaring string
String uvstr;
String myData;

char mydata[16];

void setup() {
  Serial.begin(9600);
  RFduinoGZLL.begin(role);
}

void loop() {
}

void RFduinoGZLL_onReceive(device_t device1, int rssi, char *data, int len)
{
  //hdata = data[0,1] + " ";
  Data = data;
  hstr = Data.substring(0,2) + " ";
  tstr = Data.substring(2,4) + " ";
  hestr = Data.substring(4,6) + " ";
  uvstr = Data.substring(6,9) + " ";
  
  myData = hstr + tstr + hestr + uvstr;
  
  myData.toCharArray(mydata,16);
  
  Serial.println(mydata);
}
