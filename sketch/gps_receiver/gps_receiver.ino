#include <NMEAGPS.h>
#include <GPSport.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

static NMEAGPS  gps; // This parses the GPS characters

// Home Wifi definitions

static const char* ssid = "WifiSam-2G";
static const char* password = "Apollo1122";

//Campus Wifi definitions

/*static const char* ssid = "EHU-Visitor";
static const char* username = "23441445";
static const char* password = "FuenteSecreto11"; */

static HTTPClient http;
static String postData;

void setup() {
  DEBUG_PORT.begin(9600);
  gpsPort.begin(9600);
//  Home Wifi Connection
  WiFi.begin(ssid, password);
//  Campus Wifi Connection
/*  wifi_set_opmode(STATION_MODE);
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  wifi_station_set_config(&wifi_config);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));
  wifi_station_connect(); */
}

void loop() {
  while (gps.available( gpsPort ))
    doSomeWork( gps.read() );
}

static void doSomeWork( const gps_fix & fix )
{
  if (fix.valid.location) {
    postData = "lat=" + String(fix.latitudeL()) + "&long=" + String(fix.longitudeL());
    //IP Address of Web Server
    http.begin("http://192.168.0.10:8000/api/v0/location/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postData);   //Send the request
    String payload = http.getString();    //Get the response payload  
    DEBUG_PORT.println("code:");
    DEBUG_PORT.println(httpCode);   //Print HTTP return code
    DEBUG_PORT.println("data:");    //Print request response payload
    DEBUG_PORT.println(payload);    //Print request response payload
    http.end();  //Close connection
  }
  //Sleep for 60 seconds
  ESP.deepSleep(60e6);
}