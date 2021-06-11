#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

const char *ssid = "SOME_SSID";
const char *password = "SOME_PASS";
const char *mqttServer = "SOME_IP";
const int mqttPort = 1883;

WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT;

void append_str(char str[] , char c){
     char arr[2] = {c , '\0'};
     strcat(str , arr);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }
 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// This function connects to the MQTT broker
void reconnect() {
  // Set our MQTT broker address and port
  MQTT_CLIENT.setServer(mqttServer, mqttPort);
  MQTT_CLIENT.setClient(WIFI_CLIENT);

  // Loop until we're reconnected
  while (!MQTT_CLIENT.connected()) {
    // Attempt to connect
    Serial.println("Attempt to connect to MQTT broker");
    MQTT_CLIENT.connect("garage_temp");

    // Wait some time to space out connection requests
    delay(3000);
  }

  Serial.println("MQTT connected");
}

void loop() {
  if (!MQTT_CLIENT.connected()) {
    reconnect();
  }

  while (Serial.available()){
    char serial_data[100];
    int availableBytes = Serial.available();
    char delimiter[] = "}";
    
    for(int i=0; i<availableBytes; i++)
    {
      serial_data[i] = Serial.read();
    }

    char* first = strtok(serial_data,delimiter);

    Serial.println(first);

    append_str(first,'}');

    MQTT_CLIENT.publish("garage/temperatures", first);
  }

  delay(5000);
}
