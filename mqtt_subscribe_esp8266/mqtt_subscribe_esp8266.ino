#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

const char *wifiSSID = "SOME_SSID";
const char *wifiPassword = "SOME_PASS";
const char *mqttServer = "SOME_IP";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    char value[100] = "";
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        value[i] = (char) payload[i];
    }

    Serial.println(value);

    if(strcmp ("ON",value) == 0){
        Serial.println("LED ON");
        digitalWrite(2, HIGH);
    }else{
      Serial.println("LED OFF");
        digitalWrite(2, LOW);
    }

    Serial.println();
    Serial.println("-----------------------");
}

void setup(){
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    WiFi.begin(wifiSSID, wifiPassword);

    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Connected to the WiFi network");

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()){
        Serial.println("Connecting to MQTT...");

        if (client.connect("test_client")){
            Serial.println("Connected.");
        }
        else{
            Serial.print("Failed with state: ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.subscribe("esp/test");
}

void loop(){
    client.loop();
}
