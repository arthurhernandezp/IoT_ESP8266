#include <ESP8266WiFi.h>        // Incluindo a Biblioteca
//#include <WiFi.h>        // Incluindo a Biblioteca
#include <WiFiClient.h> 

const char* ssid = "sssssssssssssssssss";      // WiFi SSID
const char* password = "pppppppppppppppppp";     // WiFi password
WiFiClient client;

int contaFalhas=0;
const char* host = "192.168.1.65";   // Endereço do servidor e caminho, pode ser ipaddress 

String dados ="?nome=Maria&cpf=1234&senha=12&confirmaSenha=1234";

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
  if (client.connect(host,8080)) {  // Tenta conectar-se ao servidor pela porta 8080   
      client.println("POST /CadastroPrimario/CadastroInicial" + dados);  // Manda os dados
      client.println("Cache-Control: no-cache");  
      client.println("Content-Type: application/x-www-form-urlencoded");
      Serial.println("[Resposta:]");          // Usar 
         while (client.connected()){            // se 
             if (client.available()){         // interessa 
              String line = client.readStringUntil('\n');   // ver 
                Serial.println(line);         // a 
             }                // resposta 
          }                 //  do servidor 
  }      
client.stop();
delay(10000);
}

