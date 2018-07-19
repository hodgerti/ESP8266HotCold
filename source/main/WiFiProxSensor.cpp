#include "WiFiProxSensor.h"

WiFiProxSensor::WiFiProxSensor()
{
  _ssid = "PROXDIMMERDEFAULT";
  _pass = NULL;
}
WiFiProxSensor::WiFiProxSensor( char const *ssid, char const *pass )
{
  _ssid = ssid;
  _pass = pass;
}
WiFiProxSensor::~WiFiProxSensor()
{

  
}

boolean WiFiProxSensor::startProxPortal()
{
  WiFi.mode(WIFI_AP_STA);
  
  dns_server.reset( new DNSServer() );
  server.reset( new ESP8266WebServer(80) );
  DEBUG_MSG( F("Starting Prox Sensor") );

  // check if password is valid
  if( _pass != NULL &&
      strlen(_pass) > 7 &&
      strlen(_pass) < 64 )
  {
    DEBUG_MSG( F("Invalid Password. Ignoring") );
    _pass = NULL;
  }

  if( _pass != NULL )
  {
    WiFi.softAP( _ssid, _pass );
  }
  else
  {
    WiFi.softAP( _ssid );
  }

  // Let IP be set
  delay(500);
  DEBUG_MSG( F( "AP IP address: " ) );
  DEBUG_MSG( WiFi.softAPIP() );
  
  // Setup the DNS server redirecting all the domains to the apIP
  dns_server->setErrorReplyCode( DNSReplyCode::NoError );
  dns_server->start(DNS_PORT, "*", WiFi.softAPIP() );

  // setup webpage handling
  server->on( String(F("/") ), std::bind( &WiFiProxSensor::handleRoot, this ) );
  server->onNotFound( std::bind( &WiFiProxSensor::handleNotFound, this ) );

  DEBUG_MSG( F( "HTTP server started" ) );
  server->begin();
}

boolean WiFiProxSensor::think()
{
  dns_server->processNextRequest();
  server->handleClient();  
}

/***************************************************
- servers root
****************************************************/
void WiFiProxSensor::handleRoot()
{
  DEBUG_MSG( F("Handling Root" ) );

  String page = FPSTR( HTTP_PAGE );

  server->sendHeader( "Content-Length", String( page.length() ) );
  server->send( 200, "text/html", page );
}

/***************************************************
- servers 404
****************************************************/
void WiFiProxSensor::handleNotFound()
{
  DEBUG_MSG( F("404: NOT FOUND" ) );

  String page = FPSTR( HTTP_404 );

  server->sendHeader( "Content-Length", String( page.length() ) );
  server->send( 200, "text/html", page );
}

