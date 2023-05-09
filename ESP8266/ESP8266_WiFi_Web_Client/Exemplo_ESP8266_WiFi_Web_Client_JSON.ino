#include <ESP8266WiFi.h>        // Incluindo a Biblioteca
//#include <WiFi.h>        // Incluindo a Biblioteca
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
//#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "ssssssssssssssssssss";      // WiFi SSID
const char* password = "ppppppppppppppppppppp";     // WiFi password
WiFiClient client;
HTTPClient http;
int contaFalhas=0;
const char* host = "http://192.168.1.65:8080//CadastroPrimario/CadastroInicial";   // Endereço do servidor e caminho, pode ser ipaddress 

StaticJsonDocument<1024> doc;

void setup() {
  Serial.begin(115200);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  

  
  while (WiFi.status() != WL_CONNECTED) {    
    Serial.print(".");
    delay(1000);
    contaFalhas++;
    if (contaFalhas>5){
          Serial.println("--> Mais de 5 falhas consecutivas!\n");
          contaFalhas=0;
          ESP.restart();
          while(1);
     }

  }
  
  Serial.print("Conectado á rede WiFi network com IP Address: ");
  Serial.println(WiFi.localIP());
}
void loop(){
  if(WiFi.status()== WL_CONNECTED){
        
      http.begin(client,host); 

      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "nome=Maria&cpf=1234&senha=12&confirmaSenha=1234";
      int httpResponseCode = http.POST(httpRequestData);

      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());

     DeserializationError error = deserializeJson(doc,http.getString());
     if (error) {
      Serial.println("JSON parsing failed!");
      
    }
    
  const char* msg = doc["msg"];
  boolean erro = doc["erro"];
  
      
  Serial.print("Erro = ");
  Serial.println(erro);
  Serial.print("Msg = ");
  Serial.println(msg);

  Serial.print("Capacidade da Memoria: ");
  Serial.println(doc.capacity());
  
  Serial.print("Memoria utilizada: ");
  Serial.println(doc.memoryUsage());


  
      

   http.end();
}

delay(10000);
}

