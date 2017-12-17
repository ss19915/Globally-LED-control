//This program can change state of LED_BUILTIN of the nodemcu Globally.  
//use html file to control the led
//set baud rate to 115200
#include<ESP8266WiFi.h>
#include<PubSubClient.h>

// fill this according to your network
char ssid[32]="-----",key[64]="--------";
char* mqtt_server= "broker.mqtt-dashboard.com",*topicPub="AD",*topicSub="SAD",*clientID="ss199155";
WiFiClient esp;
PubSubClient client(esp);


void setup() {
  Serial.begin(115200);
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
  pinMode(LED_BUILTIN,OUTPUT);
}

int x=0;
void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectwifi(ssid,key);
    connectMQTT();
  }

  if(!client.connected())
      connectMQTT();
  client.loop();
}



void connectwifi(char * ssid,char * key){
  int i;
  WiFi.begin(ssid,key);
  Serial.print("Connecting: ");
  Serial.print(ssid); 
  for(i=0;i<100;i++){//wait for establishing connection
    if(WiFi.status() == WL_CONNECTED){
        Serial.println();
        Serial.print("Connected To : ");
        Serial.println(ssid);
        Serial.println(WiFi.localIP());
        break;
      }
      Serial.print(".");
      delay(500);
  }
  if(WiFi.status() != WL_CONNECTED)
    connectwifi(ssid,key);
}

void connectMQTT(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...    ");
    if(client.connect(clientID)){
        Serial.println("Connected");
        client.publish(topicPub,"System Online");
        if(client.subscribe(topicSub)){
        Serial.print("Subscribed to topic: ");
        Serial.println(topicSub);}
        
    }
    else{
      Serial.println("failed");
      Serial.println("Retrying in 5 Sec..");
      delay(5000);
    }
  }
}


void callback(char * topic,byte* payload,unsigned int len){
    if(payload[0]=='o'){//o stands on
      digitalWrite(LED_BUILTIN,LOW);
    }
    else if(payload[0]=='c'){//c stands for off
      digitalWrite(LED_BUILTIN,HIGH);
    }
    else 
    Serial.println(payload[0]);
  }
