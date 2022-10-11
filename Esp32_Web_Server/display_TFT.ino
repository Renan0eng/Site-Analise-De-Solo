void Dispaly () {
  if(digitalRead(4) == LOW){
   tft.setTextSize(1);
   tft.setCursor(12, 70);
   tft.print("IP: ");
   tft.println(WiFi.localIP());
   tft.fillScreen(0x408E2F);
   Serial.print("4");
   
 }else if(digitalRead(4) == HIGH){
  Serial.print("5");
  tft.setTextSize(3);
  tft.setCursor(30, 10);
  tft.println("TEMP");
 
  tft.setTextSize(1);
  tft.setCursor(12, 70);
  tft.println("-- Temperatura --");
  tft.setTextSize(2);
  tft.setCursor(20, 120);

  tft.fillRect(42,120, 100 ,20,0x408E2F);
  tft.print("C:");
  tft.print(tempC);
  }
}
