#include "WiFiProxSensor.h"

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

int baud_rate = 9600;

static const char *ssid = "ProxSensor";
static const char *pass = NULL;

WiFiProxSensor proxSensor( ssid );

void setup()
{
  Serial.begin(baud_rate);
  Serial.println();

  // start softap
  proxSensor.startProxPortal();
}

void loop() 
{
  proxSensor.think();
}
