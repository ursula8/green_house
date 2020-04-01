void PosliData(){
ReadString();

Serial1.println(cmd);
Serial.println(cmd);

 if(Serial1.find("OK"))
  {
    Serial.println("\r\nPripravené na odoslanie dát!");
  }
const char* postStart = "POST /postdata.php HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection:Close\r\nContent-Length: ";
const char* postEnd = "\r\n\r\n";  
char retazec[256] = {'\0'}; 
char post[512] = {'\0'}; 
char numBuffer[16] = {'\0'}; 

strcpy (retazec, "cz=");
strcat(retazec,cas_zaznamu);
strcat(retazec,"&tep=");
dtostrf(t, 3, 2, numBuffer);
strcat(retazec,numBuffer);
strcat(retazec,"&vlhv=");
itoa(vv,numBuffer,10);
strcat(retazec,numBuffer);
strcat(retazec,"&vlhp=");
itoa(vp,numBuffer,10);
strcat(retazec,numBuffer);
strcat(retazec,"&zav=");
itoa(za,numBuffer,2);
strcat(retazec,numBuffer);
strcat(retazec,"&kur=");
itoa(ku,numBuffer,2);
strcat(retazec,numBuffer);
strcat(retazec,"&vet=");
itoa(ve,numBuffer,2);
strcat(retazec,numBuffer);
strcat(retazec,"&pz=");
strcat(retazec,datum_zavlaha);
int retazec_len=strlen(retazec);

strcpy(post, postStart);
itoa(retazec_len, numBuffer,10);
strcat(post, numBuffer);
strcat(post, postEnd);

int dlzka=strlen(post)+retazec_len;
strcat(post,retazec);

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
Serial.println(post);
Serial1.print(post);



if(Serial1.find("OK"))
    {
      Serial.println("Balík dát bol úspešne odoslaný!");
    }
 ReadString();  



Serial.println("-----end");

}
