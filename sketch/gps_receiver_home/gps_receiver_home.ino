#include <NMEAGPS.h>
#include <GPSport.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

static NMEAGPS  gps; // This parses the GPS characters
static const char* ssid = "SKY52760";
static const char* password = "VCCRYTVTLN";
static HTTPClient http;
static String postData;

void setup() {
  DEBUG_PORT.begin(9600);
  gpsPort.begin(9600);
  WiFi.begin(ssid, password);
}

void loop() {
  while (gps.available( gpsPort ))
    doSomeWork( gps.read() );
}

static void doSomeWork( const gps_fix & fix )
{
  if (fix.valid.location) {
    postData = "lat=" + String(fix.latitudeL()) + "&long=" + String(fix.longitudeL());
    http.begin("http://192.168.0.19:8000/api/v0/location/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postData);   //Send the request
    String payload = http.getString();    //Get the response payload  
    DEBUG_PORT.println("code:");
    DEBUG_PORT.println(httpCode);   //Print HTTP return code
    DEBUG_PORT.println("data:");    //Print request response payload
    DEBUG_PORT.println(payload);    //Print request response payload
    http.end();  //Close connection
  }
  ESP.deepSleep(120e6);
}

static void printL( Print & outs, int32_t degE7 )
{
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    outs.print( '-' );
  }

  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  outs.print( deg );
  outs.print( '.' );

  // Get fractional degrees
  degE7 -= deg * 10000000L;

  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)) {
    outs.print( '0' );
    factor /= 10L;
  }

  // Print fractional degrees
  outs.print( degE7 );
}
