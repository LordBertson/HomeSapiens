#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ArduinoJson.h>

const char *wifiSSID = "";
const char *wifiPassword = "";
const char *mqttServer = "";
const char *mqttUser = "";
const char *mqttPass = "";
const int mqttPort = 1883;

WiFiClient WIFI_CLIENT;
PubSubClient MQTT_CLIENT;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(wifiSSID, wifiPassword);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// This function connects to the MQTT broker
void reconnect()
{
  // Set our MQTT broker address and port
  MQTT_CLIENT.setServer(mqttServer, mqttPort);
  MQTT_CLIENT.setClient(WIFI_CLIENT);

  // Loop until we're reconnected
  while (!MQTT_CLIENT.connected())
  {
    // Attempt to connect
    Serial.println("Attempt to connect to MQTT broker");
    MQTT_CLIENT.connect("garage_temp", mqttUser, mqttPass);

    // Wait some time to space out connection requests
    delay(3000);
  }

  Serial.println("MQTT connected");
}

void loop()
{
  if (!MQTT_CLIENT.connected())
  {
    reconnect();
  }

  while (Serial.available())
  {
    StaticJsonDocument<200> doc;

    DeserializationError error = deserializeJson(doc, Serial.readString());

    float temp_kotol = doc["temp_kotol"];
    float temp_boiler = doc["temp_boiler"];

    Serial.println(temp_kotol);

    Serial.println(temp_boiler);

    char temp_kotol_str[64];
    snprintf(temp_kotol_str, sizeof temp_kotol_str, "%f", temp_kotol);

    char temp_boiler_str[64];
    snprintf(temp_boiler_str, sizeof temp_boiler_str, "%f", temp_boiler);

    MQTT_CLIENT.publish("gladys/master/device/mqtt:senzor_garaz/feature/mqtt:kotol/state", temp_kotol_str);
    MQTT_CLIENT.publish("gladys/master/device/mqtt:senzor_garaz/feature/mqtt:boiler/state", temp_boiler_str);
  }

  delay(5000);
}
