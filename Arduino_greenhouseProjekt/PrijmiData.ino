void PrijmiData(){
ReadString();
Serial1.println(cmd);
Serial.println(cmd);

 if(Serial1.find("OK"))
  {
    Serial.println("\r\nPripravené na príjem dát!");
  }
const char* getStart="GET /getdata.php HTTP/1.1\r\nHost:example.com\r\n\r\n";

int dlzka=strlen(getStart);
Serial1.print("AT+CIPSEND=");
Serial1.println(dlzka);
Serial.print("AT+CIPSEND=");
Serial.println(dlzka);

if (Serial1.find(">")){
  Serial.println("Posielam data...");
} else {
  Serial1.println("AT+CIPCLOSE");
  Serial.println("Ukoncene pripojenie");
  return;
}
Serial1.println(getStart);
Serial.println(getStart);

while (Serial1.available()){
  
  if(Serial1.find("Mode")){
   
    pom=Serial1.readString();
    Serial.print(pom);
  }  
}



Serial.println("---end");

}
 
