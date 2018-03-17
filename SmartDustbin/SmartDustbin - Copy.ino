#include <stdlib.h>

/*-----------------ESP8266 Serial WiFi Module---------------*/
#define SSID "Mohit"     // "SSID-WiFiname" 
#define PASS "Mohit@12"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "GET /update?key=#######################"; //change it with your key...
/*-----------------------------------------------------------*/



//Variables

const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 8;

long duration;
int distance;

float temp;
int hum;
String tempC;
int error;
void setup()
{
  Serial.begin(115200); //or use default 115200.
  
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
  
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(115200);
}

void loop(){

  start: //label 
  error=0;

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
   digitalWrite(trigPin, LOW);

   duration = pulseIn(echoPin, HIGH);

   distance= duration*0.034/2;

   distance=distance/2;

   Serial.print("Distance: ");
   Serial.println(distance);
 








  char buffer[10];
  // there is a useful c function called dtostrf() which will convert a float to a char array 
  //so it can then be printed easily.  The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  tempC = dtostrf(distance, 4, 1, buffer); 

  updateTemp();
  //Resend if transmission is not completed 
  if (error==1){
    goto start; //go to label "start"
  }


  if(distance<=2 || distance >=40)  //Assuming dustbin depth is less than 40 and distance 2 means that the dustbin is about to fill
  {

    tone(buzzer,1000);
    delay(9000);
    noTone(buzzer);
    
  }
  delay(3600); 
}

void updateTemp(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += tempC;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }
  else{
    Serial.println("AT+CIPCLOSE");
    //Resend...
    error=1;
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}
