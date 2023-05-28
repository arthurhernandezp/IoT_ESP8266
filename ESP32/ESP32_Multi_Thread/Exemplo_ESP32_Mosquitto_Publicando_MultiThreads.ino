#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "INOVUERJ_LACIPI";
const char* password =  "feudodoinov";
const char* mqtt_server = "test.mosquitto.org";      // Endereço do Mosquitto - Broker
char msg[50];
char topico[50];

TaskHandle_t Thread_1;
TaskHandle_t Thread_2;

 
WiFiClient espClient;
PubSubClient client(espClient);

int contafalhas = 0;

void setup(){  
      Serial.begin(115200);  
      Serial.print("Conectando a Rede WiFi ");       
      WiFi.begin(ssid, password);     
      while (WiFi.status() != WL_CONNECTED) {
         Serial.print(".");  
         if(contafalhas >= 5) {
            ESP.restart();
            while(1);
         }
         contafalhas++;
         delay(500);   
      }
       Serial.println("Conectado ");       
       client.setServer(mqtt_server, 1883);   // Atribui endereço do Broker Mosquitto e a porta        
       xTaskCreatePinnedToCore(Thread_Sensor_01,"Thread_Sensor_01",10000,NULL,1,&Thread_1,0);                
       delay(500);       
       xTaskCreatePinnedToCore(Thread_Sensor_02,"Thread_Sensor_02",10000,NULL,1,&Thread_2,1);          
       delay(500); 
} 

void publica(char* msg, String local){  
  if (!client.connected()){    
     conecta();
  }     
     local.toCharArray(topico,local.length()+1);
     client.publish(topico,msg,true); 
     Serial.println("Mensagem publicada");     
   
}

void conecta(){  
  while (!client.connected()) {
    if (client.connect("ESP32_Deep_Sleep","","","Controle_Ambiente_01",1,true,"Fora do Ar")){
       Serial.println("Cliente Conectado ao Mosquitto");  
    }
    else{
        Serial.print("Estado do cliente: "); 
        Serial.println(client.state());     
        delay(2000);
    }
  }
}

void Thread_Sensor_01( void * pvParameters ){
  while(true){
    String temperatura =(String) random(10,45);        // Valor arbitrado randomicamente(É um exemplo!)
    temperatura.toCharArray(msg,temperatura.length()+1); 
    publica(msg,"Temperatura"); 
    delay(5000);
  }
}
void Thread_Sensor_02( void * pvParameters ){
  while(true){  
    String umidade =(String) random(0,100);        // Valor arbitrado randomicamente(É um exemplo!)
    umidade.toCharArray(msg,umidade.length()+1);
    publica(msg,"Umidade");
   delay(5000);
  }
}

void loop(){}
