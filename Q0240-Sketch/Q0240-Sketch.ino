#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <SPI.h>



#define TFT_DC 12 //A0
#define TFT_CS 13 //CS
#define TFT_MOSI 14 //SDA
#define TFT_CLK 15 //SCK
#define TFT_RST 0  
#define TFT_MISO 0 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define    ONE_WIRE_BUS     5

OneWire oneWire(ONE_WIRE_BUS);        
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

float tempMin = 999;   //armazena temperatura mínima
float tempMax = 0;     //armazena temperatura máxima

void mostra_endereco_sensor(DeviceAddress deviceAddress);  //Função para mostrar endereço do sensor


const char* ssid = "POCO";
const char* password = "12345678";

WiFiServer server(80); //Shield irá receber as requisições das páginas (o padrão WEB é a porta 80)

String HTTP_req; 
String URLValue;

void processaPorta(byte porta, byte posicao, WiFiClient cl);
void lePortaDigital(byte porta, byte posicao, WiFiClient cl);        
void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl);   
String getURLRequest(String *requisicao);
bool mainPageRequest(String *requisicao);

const byte qtdePinosDigitais = 2;
byte pinosDigitais[qtdePinosDigitais] = {2, 4};
byte modoPinos[qtdePinosDigitais]     = {OUTPUT, OUTPUT};

const byte qtdePinosAnalogicos = 1;
byte pinosAnalogicos[qtdePinosAnalogicos] = {A0};

void setup()
{         

  


    Serial.begin(115200);

    sensors.begin();

    // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
     
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  Serial.println();
 

    //Conexão na rede WiFi
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado!");

    // Inicia o servidor WEB
    server.begin();
    Serial.println("Server iniciado");

    // Mostra o endereco IP
    Serial.println(WiFi.localIP());

    //Configura o modo dos pinos
    for (int nP=0; nP < qtdePinosDigitais; nP++) {
        pinMode(pinosDigitais[nP], modoPinos[nP]);
    }
     tft.initR(INITR_BLACKTAB); 

   tft.setTextWrap(false);
   tft.fillScreen(0x408E2F);
   tft.setTextColor(0x5AEB);

   tft.setTextSize(1);
   tft.setCursor(12, 70);
   tft.print("IP: ");
   tft.println(WiFi.localIP());
   delay(5000);
   tft.fillScreen(0x408E2F);
}

void loop()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
 if(digitalRead(4) == LOW){
   tft.setTextSize(1);
   tft.setCursor(12, 70);
   tft.print("IP: ");
   tft.println(WiFi.localIP());
   delay(5000);
   tft.fillScreen(0x408E2F);
 }
  
 if(digitalRead(4) == HIGH){
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

    WiFiClient  client = server.available();

    if (client) { 
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { 
                char c = client.read(); 
                HTTP_req += c;  
                
                if (c == '\n' && currentLineIsBlank ) { 

                    if ( mainPageRequest(&HTTP_req) ) {
                        URLValue = getURLRequest(&HTTP_req);
                        Serial.println(HTTP_req);
                                                 
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");              //<------ ATENCAO
                        client.println();
                        
                        //Conteudo da Página HTML
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");

                        
                        client.println("<head>");
                        client.println("<meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Exercicio HTML</title>");
                        client.println("<link rel=\"stylesheet\" href=\"https://renan0eng.github.io/Site-Analise-De-Solo/style.css\">");
                        client.println("<link rel=\"shortcut icon\" href=\"https://renan0eng.github.io/Site-Analise-De-Solo/img/favicon.ico\" type=\"image/x-icon\">"); 
                        
                        client.println("<script>");
                        client.println("function LeDadosDoArduino() {");
                        client.println("nocache = \"&nocache=\" + Math.random() * 1000000;");
                        client.println("var request = new XMLHttpRequest();");
                        client.println("var posIni;");
                        client.println("var valPosIni;");
                        client.println("var valPosFim;");
                        client.println("request.onreadystatechange = function() {");
                        client.println("if (this.readyState == 4) {");
                        client.println("if (this.status == 200) {");
                        client.println("if (this.responseText != null) {");

                                                                       //<-------NOVO
                            client.print("posIni = this.responseText.indexOf(\"PD");
                            client.print(tempC);
                            client.println("\");");
                            client.println("if ( posIni > -1) {");
                            client.println("valPosIni = this.responseText.indexOf(\"#\", posIni) + 1;");
                            client.println("valPosFim = this.responseText.indexOf(\"|\", posIni);");
                            client.print("document.getElementById(\"pino");
                            client.print(tempC);
                            client.println("\").checked = Number(this.responseText.substring(valPosIni, valPosFim));");
                            client.println("}");
                        

                                                                            //<-------NOVO
                            
                            client.print("posIni = this.responseText.indexOf(\"PA");
                            client.print(tempC);
                            client.println("\");"); 
                            client.println("if ( posIni > -1) {");
                            client.println("valPosIni = this.responseText.indexOf(\"#\", posIni) + 1;");
                            client.println("valPosFim = this.responseText.indexOf(\"|\", posIni);");
                            client.print("document.getElementById(\"pino");
                            client.print(tempC);
                            client.print("\").innerHTML = \"Porta ");
                            client.print("Temperatura");
                            client.print(" - Valor: \" + this.responseText.substring(valPosIni, valPosFim);");
                            client.println("}");
                        
                          
                        client.println("}}}}");
                        client.println("request.open(\"GET\", \"solicitacao_via_ajax\" + nocache, true);");
                        client.println("request.send(null);");
                        client.println("setTimeout('LeDadosDoArduino()', 1000);");
                        client.println("}");
                        client.println("</script>");
                        
                        client.println("</head>");

                        client.println("<body onload=\"LeDadosDoArduino()\">");                      //<------ALTERADO                    
                        client.println("<h1>Temperatura:</h1>");

                        

                            client.print("<div id=\"pino");                         //<----- NOVO
                            client.print(tempC);
                            client.print("\">"); 
                                                         
                            client.print("Porta ");
                            client.print("Temperatura");
                            client.println(" - Valor:");
                               
                            client.print(tempC);
                            client.println("</div>");                               //<----- NOVO
                               
                            client.println("<br/>");                             
                        
                        
                        client.println("<br/>");                        
                        client.println("<h1>PORTAS EM FUN&Ccedil;&Atilde;O DIGITAL</h1>");
                        client.println("<form method=\"get\">");

                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            processaPorta(pinosDigitais[nL], nL, client);
                            client.println("<br/>");
                        }
                        
                        client.println("<br/>");
                        client.println("<button type=\"submit\">Envia para o ESP8266</button>");
                        client.println("</form>");                      
                        
                        client.println("</body>");
                        
                        client.println("</html>");

                    
                    } else if (HTTP_req.indexOf("solicitacao_via_ajax") > -1) {     //<----- NOVO

                        Serial.println(HTTP_req);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");      
                        client.println();                      

                        for (int nL=0; nL < qtdePinosAnalogicos; nL++) {
                            lePortaAnalogica(pinosAnalogicos[nL], nL, client);                            
                        }
                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            lePortaDigital(pinosDigitais[nL], nL, client);
                        }
                            
                    } else {

                        Serial.println(HTTP_req);
                        client.println("HTTP/1.1 200 OK");
                    }
                    HTTP_req = "";    
                    break;
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        } 
        delay(1);     
        client.stop(); 
    } 
}


void processaPorta(byte porta, byte posicao, WiFiClient cl)
{
static boolean LED_status = 0;
String cHTML;

    cHTML = "P";
    cHTML += porta;
    cHTML += "=";
    cHTML += porta;

    if (modoPinos[posicao] == OUTPUT) { 
        
        if (URLValue.indexOf(cHTML) > -1) { 
           LED_status = HIGH;
        } else {
           LED_status = LOW;
        }
        digitalWrite(porta, LED_status);
    } else {

        LED_status = digitalRead(porta);
    }
        
    cl.print("<input type=\"checkbox\" name=\"P");
    cl.print(porta);
    cl.print("\" value=\"");
    cl.print(porta);
    
    cl.print("\"");

    cl.print(" id=\"pino");           //<------NOVO
    cl.print(porta);
    cl.print("\"");
    
    if (LED_status) { 
        cl.print(" checked ");
    }

    if (modoPinos[posicao] != OUTPUT) { 
        cl.print(" disabled ");
    }
    
    cl.print(">Porta ");
    cl.print(porta);

    cl.println();
}

void lePortaDigital(byte porta, byte posicao, WiFiClient cl)
{
    if (modoPinos[posicao] != OUTPUT) { 
       cl.print("PD");
       cl.print(porta);
       cl.print("#");
       
       if (digitalRead(porta)) {
          cl.print("1");
       } else {
          cl.print("0");
       }
       cl.println("|");
    }
}


void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl)
{
   cl.print("PA");
   cl.print(porta);
   cl.print("#");
   
   cl.print(analogRead(porta));

   //especifico para formatar o valor da porta analogica A0
   if (porta == A0) { 
      cl.print(" (");
      cl.print(map(analogRead(A0),0,1023,0,179)); 
      cl.print("&deg;)");
   }
   
   cl.println("|");   
}


String getURLRequest(String *requisicao) {
int inicio, fim;
String retorno;

  inicio = requisicao->indexOf("GET") + 3;
  fim = requisicao->indexOf("HTTP/") - 1;
  retorno = requisicao->substring(inicio, fim);
  retorno.trim();

  return retorno;
}

bool mainPageRequest(String *requisicao) {
String valor;
bool retorno = false;

  valor = getURLRequest(requisicao);
  valor.toLowerCase();

  if (valor == "/") {
     retorno = true;
  }

  if (valor.substring(0,2) == "/?") {
     retorno = true;
  }  

  if (valor.substring(0,10) == "/index.htm") {
     retorno = true;
  }  

  return retorno;
}
void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
