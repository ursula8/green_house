void NastavSklenik(){

  
if(mode=="manual"){
  
  if((inzavlaha=="1")&&(za!=1)){   ///controlling water pump
    digitalWrite(pinCerpadlo,LOW);
    Serial.println("Pustena zavlaha");
    za=1;
  }
  else if((inzavlaha=="0")&&(za!=0)){
    digitalWrite(pinCerpadlo,HIGH);
    Serial.println("Zavlaha vypnuta");
    za=0;
  }
  
  if((invetranie=="1")&&(ve!=1)){   ///controlling servo position
    ve=1;
    for (servo_poz; servo_poz<=90;servo_poz++){
    vetranie.write(servo_poz);
    delay(15);        
      }   
      Serial.println("Okno otvorené");
  }
  else if ((invetranie=="0")&&(ve!=0)){ 
    ve=0;
    for (servo_poz; servo_poz>=0;servo_poz--){
    vetranie.write(servo_poz); 
    delay(15);          
      }  
      Serial.println("Okno zavrete"); 
  }
  
  if((inkurenie=="1")&&(ku!=1)){  ///controlling relay (lightbulb)
    ku=1;
  }
  else if ((inkurenie=="0")&&(ku!=0)){
    ku=0;
  }
}
else if(mode=="automat"){
  
}
}
