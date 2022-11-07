#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdio.h>
#include <ArduinoJson.h>

#define ONE_WIRE_BUS_1 2

#define ONE_WIRE_BUS_2 3

OneWire oneWire1(ONE_WIRE_BUS_1);

OneWire oneWire2(ONE_WIRE_BUS_2);

DallasTemperature sensors1(&oneWire1);

DallasTemperature sensors2(&oneWire2);

float temp_kotol = 0;
float temp_boiler = 0;

void setup(void)
{

  Serial.begin(115200);
  sensors1.begin();
  sensors2.begin();
}

void loop(void)
{
  DynamicJsonDocument doc(1024);

  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  temp_kotol = sensors1.getTempCByIndex(0);
  temp_boiler = sensors2.getTempCByIndex(0);

  doc["temp_kotol"] = temp_kotol;
  doc["temp_boiler"] = temp_boiler;

  serializeJson(doc, Serial);

  delay(5000);
}
