#ifndef ProxSensor_h
#define ProxSensor_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const char HTTP_PAGE[] PROGMEM = "<!DOCTYPE html><html><head></head><style>.btn{background-color: red;color: black;padding: 16px 16px;margin-bottom: 5px;font-size: 20px;border: 1px solid black;cursor: pointer;width: 50%;height: 3em;}.btn:hover, .btn:focus{color: white;}.btn_analog:hover, .btn_analog:focus{color: white;}.btn-header{color: black;font-family: Verdana;font-size: 14px;margin: 0px auto;padding: 1em;width: 50%;}input[type=\"text\"]{width: 30%;padding-right: 50px;font-family: Verdana;font-size: 22px;}input[type=\"submit\"]{width: 12%;height: 3em;padding-right: 50px;background-color: blue;color: black;border: 1px solid black;}input[type=\"submit\"]:hover{color: white;}input[type=\"submit\"]:focus{color: white;}</style><body><h5 class=\"btn-header\"> A BUTTON </h5><div align=\"center\"><input type=\"text\" id=\"a_button\" ><input type=\"submit\" id=\"dim_level_button\" value=\"button\"></div></body>";
const char HTTP_404[]  PROGMEM = "<!DOCTYPE html><html><h1>404</h1></html>";
class WiFiProxSensor
{
  public:
    WiFiProxSensor();
    WiFiProxSensor( char const *, char const * = NULL );
    ~WiFiProxSensor();

    boolean setupProxPortal();
    boolean think();
    
  protected:

  private:
    std::unique_ptr<DNSServer> dns_server;
    std::unique_ptr<ESP8266WebServer> server;

    char const *_ssid = "";
    char const *_pass = "";
    // DNS server
    const char DNS_PORT = 53;
    int serverStartTime;

    void handleRoot();
    void handleNotFound();

    boolean isIp(String str);
    boolean captivePortal();
    String toStringIp(IPAddress ip);

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
