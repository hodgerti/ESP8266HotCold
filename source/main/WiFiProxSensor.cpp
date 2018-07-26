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

boolean WiFiProxSensor::setupProxPortal()
{
  WiFi.mode(WIFI_AP_STA);
  
  dns_server.reset( new DNSServer() );
  server.reset( new ESP8266WebServer(80) );
  DEBUG_MSG( F("Starting Prox Sensor") );
  serverStartTime = millis();

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
  yield();
}

/** Is this an IP? */
boolean WiFiProxSensor::isIp(String str) {
  for (int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

/** IP to String? */
String WiFiProxSensor::toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
    
/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean WiFiProxSensor::captivePortal()
{
  if(!isIp(server->hostHeader()))
  {
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    return true;  
  }
  return false;
}

/***************************************************
- servers root
****************************************************/
void WiFiProxSensor::handleRoot()
{
  if(captivePortal())
  {
    return;
  }
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
  if(captivePortal())
  {
    return;
  }
  DEBUG_MSG( F("404: NOT FOUND" ) );

  String page = FPSTR( HTTP_404 );

  server->sendHeader( "Content-Length", String( page.length() ) );
  server->send( 200, "text/html", page );
}

