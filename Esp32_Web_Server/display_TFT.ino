void Dispaly (void* pvParameters)
{
  
  
  //IMPORTANTE: A tarefa não pode terminar, deve ficar presa em um loop infinito
  while(true){
    sensors.requestTemperatures();
    tempC = sensors.getTempC(sensor1);
    if(digitalRead(4) == LOW){
      
      tft.fillScreen(0x408E2F);
      tft.setTextSize(1);
      tft.setCursor(12, 70);
      tft.print("IP: ");
      tft.println(WiFi.localIP());
      TickType_t taskDelay = 500 / portTICK_PERIOD_MS;
      vTaskDelay(taskDelay);
      tft.fillScreen(0x408E2F);
    
    }else if(digitalRead(4) == HIGH){
      
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
      TickType_t taskDelay = 2 / portTICK_PERIOD_MS;
    vTaskDelay(taskDelay);
      
    }
  }
}
