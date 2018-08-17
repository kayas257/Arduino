//
//Author Kayas Ahmed
//
#include <SoftwareSerial.h>
#include <dht.h>
#define dht_apin A0 // Analog Pin sensor is connected to
#define RX 10
#define TX 11
String AP = "xxxxxx";       // CHANGE ME
String PASS = "xxxxxxx"; // CHANGE ME
String API = "xxxxxxxxx";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 
 dht DHT;
  
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
 // sendCommand("AT",5,"OK");
   Serial.print("OK");
  //sendCommand("AT+CWMODE=1",5,"OK");
 //sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void loop() {
   DHT.read11(dht_apin);
  valSensor=DHT.temperature;
    Serial.print(valSensor);
    Serial.print("C\n");
    
 String getData = "GET /update?api_key="+ API +"&"+ field+"1" +"="+String(valSensor);


sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 valSensor=DHT.humidity;
 Serial.print(valSensor);
    Serial.print("%\n");
   getData = "GET /update?api_key="+ API +"&"+ field+"2" +"="+String(valSensor);
   sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
    
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
