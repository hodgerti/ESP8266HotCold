#ifndef ProxSensor_h
#define ProxSensor_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const char HTTP_PAGE[] = "<!DOCTYPE html><head></head></html>";

class WiFiProxSensor
{
  public:
    WiFiProxSensor();
    WiFiProxSensor( char const *, char const * = NULL );
    ~WiFiProxSensor();

    boolean startProxPortal();
    boolean think();
    
  protected:

  private:
    std::unique_ptr<DNSServer> dns_server;
    std::unique_ptr<ESP8266WebServer> server;

    char const *_ssid = "";
    char const *_pass = "";
    // DNS server
    const char DNS_PORT = 53;

    void handleRoot();

    bool _DEBUG = true;
    template <typename T>
    void DEBUG_MSG(T text)
    {
      if( _DEBUG )
      {
        Serial.print( "DEBUG: " );
        Serial.println( text );
      }
    }
  
};

#endif
