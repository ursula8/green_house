
//#include <SoftwareSerial.h>
#include "DHT.h" // připojení knihovny DHT
#include "Servo.h" //knižnica servo motora
#include <Wire.h>
#include <RTClib.h>




//SoftwareSerial Serial1(2, 3); // RX, TX
#define pinDHT A0 // 
#define typDHT11 DHT11     // DHT 11
#define pinvlhkomer A1
#define pinvetranie 8
int pinCerpadlo=10;

DHT mojeDHT(pinDHT, typDHT11);
RTC_DS1307 rtc; 
Servo vetranie;

////////////////WIFI PRIHLASENIE///////////////
const char* ssid     ="";
const char* password = "";
const char* server = "example.com";
const char* cmd="AT+CIPSTART=\"TCP\",\"example.com\",80";

////////PREMENNE ODOSIELANÝCH ÚDAJOV//////////
float t;
int vv;
int vp;
int za=0;
int ku=0;
int ve=0;
char cas_zaznamu[9]; //time
char datum_zavlaha[12]; //date

///////////PREMENNE PRIJIMANÝCH ÚDAJOV///////
String mode;
String inzavlaha;
String inkurenie;
String invetranie;
String invlhkost_p;
String invlhkost_v;
String incas;
String intrvanie;
String inopakovanie;
String inteplota;

/////POMOCNE PREMENNE/////////
boolean connected = false;
String odpoved="";

int map_low=695;
int map_high=310;
int servo_poz=0;
String pom;
unsigned long startMillis; 
unsigned long currentMillis;
char numBuffer[16] = {'\0'};

/////////ZOZNAM FUNKCII/////////
void Reset(); //reset ESP8266
void connect_to_wifi(); ///pripojenie na WiFi
void PrijmiData(); //prijem dat od servera 
void PosliData(); //odoslanie dát na server
String return_premenne();
void VycistiESP();
void NastavSklenik() ;


void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial1.setTimeout(2000);
  Wire.begin();
  mojeDHT.begin();
  startMillis = 0; 
  vetranie.attach(pinvetranie);
  pinMode(pinvlhkomer, INPUT);
  pinMode(pinCerpadlo,OUTPUT);
  digitalWrite(pinCerpadlo,HIGH);
  vetranie.write(servo_poz);
  
  Reset();
  connect_to_wifi();
  
 //rtc.adjust(DateTime(__DATE__, __TIME__));
}
 
 
void loop() {         //ZACIATOK LOOP ***********************************************************************
 delay(1000);
  
 
 t=mojeDHT.readTemperature();
 vv=mojeDHT.readHumidity();
 int vlhpoda=analogRead(pinvlhkomer);
 vp=map(vlhpoda,map_low,map_high,0,100);  ////percenta z analogových udajov
 Serial.println("Vlhkost pôdy: ");
 Serial.println(vlhpoda);
 Serial.println(vp);
 DateTime now = rtc.now();

snprintf(cas_zaznamu, 9, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()); ///actual time
snprintf(datum_zavlaha, 12, "%02d-%02d-%d", now.day(), now.month(), now.year());    ///actual date

Serial.println("Current Date & Time: ");
Serial.println(cas_zaznamu);
Serial.println(datum_zavlaha);
  
if (isnan(t) || isnan(vv)) {
  Serial.println("Chyba pri čítaní z DHT senzoru!");
  } 
  else {
    Serial.print("Teplota: "); 
    Serial.println(t);      //Temperature
    Serial.print("vlhkost: "); 
    Serial.println(vv);     //Humidity
  }
 
 if(!connected) {
  Reset();
  connect_to_wifi(); 
 }
 
 else{ 
    PrijmiData();   //get data from server
    
 mode=return_premenne();   ///load data into single variables
 inteplota=return_premenne();
 invlhkost_v=return_premenne();
 incas=return_premenne();
 intrvanie=return_premenne();
 inopakovanie=return_premenne();
 invlhkost_p=return_premenne();
 inzavlaha=return_premenne();
 inkurenie=return_premenne();
 invetranie=return_premenne();
 
  NastavSklenik();  //change relay state etc.
  VycistiESP();     //clean ESP from any buffer residues
  delay(1000);      
  PosliData();      //send data to server
  
   
 }
 
 

 delay(3000);
 
}           // KONIEC LOOP ************************************************************************************
 
void ReadString(){
  while(Serial1.available()){
      String odpoved=Serial1.readString();
    Serial.print(odpoved);
    delay(1);
   }
}
void Reset (){                        // Reset ESP8266 chip
 
  Serial1.println("AT+RST");          // AT príkaz na reset
  delay(2000);                        // Čaká 1 sekundu
  if(Serial1.find("OK") )             // Ak ESP8266 po odoslaní príkazu na reset odošle naspäť OK vypíše Reset OK ak nie Reset neúspešný
  {
    Serial.println("ESP8266 Reset OK");
  }
  else
  {
    Serial.println("ESP8266 Reset neúspešný!");
  }
  Serial1.println("AT+CWQAP");
  delay(1000);
  if (Serial1.find("OK")){
    Serial.println("Uspesne Odpojena Wifi");
  }
  ReadString();
}
 
 
 
void connect_to_wifi(){
  
  Serial1.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd+=ssid;
  cmd+="\",\"";
  cmd+=password;
  cmd+="\"";
  Serial1.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if (Serial1.find("OK")){
    Serial.println("Uspesne pripojeny k internetu");
  connected=true;
  } else {
    Serial.println("Nepodarilo sa pripojit");
  connected=false;
  }
  
  
}

void VycistiESP(){
   Serial1.println("AT+CIPCLOSE");
   while(Serial1.available()){
      char odpoved=Serial1.read();
   }     
}
String return_premenne(){   ///slicing the string returned from server into different variables

int zac = pom.indexOf("=");  // from this Mode=manual#Teplota=30#Vlhv=20#Cas=0#Trvanie=0#Opakovanie=0#Vlhp=20#Zav=0#Kur=0#Vet=0#
int kon= pom.indexOf("#");
String slice=pom.substring(zac+1,kon);
pom.remove(0,kon+1);

return slice;
}
