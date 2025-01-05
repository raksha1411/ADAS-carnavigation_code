#include <WiFi.h>
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include < TinyGPS++.h>
#include <SoftwareSerial.h> 

staticconstuint32_tGPSBaud=9600; SoftwareSerial ss(27, 26); TinyGPSPlus gps;
const double HOME_LAT = 16.437716;
const double HOME_LNG = 74.610964;

#define WLAN_SSID "Galaxy M11" #define WLAN_PASS "sudeep 12345" #define AIO_SERVER "io.adafruit.com" #define AIO_SERVERPORT 1883
#define AIO_USERNAME "sudeep1"
#define AIO_KEY "aio_123456789" WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

const char gpslat_FEED[] PROGMEM = AIO_USERNAME "/feeds/gpslat";
Adafruit_MQTT_Publish gpslat = Adafruit_MQTT_Publish(&mqtt, gpslat_FEED);

const char gpslng_FEED[] PROGMEM = AIO_USERNAME "/feeds/gpslng";
Adafruit_MQTT_Publish gpslng = Adafruit_MQTT_Publish(&mqtt, gpslng_FEED);

const char gps_FEED[] PROGMEM = AIO_USERNAME "/feeds/gpslatlng/csv";
Adafruit_MQTT_Publish gpslatlng = Adafruit_MQTT_Publish(&mqtt, gps_FEED);
void MQTT_connect();
void setup()
{
    Serial.begin(115200);
    delay(100);
    ss.begin(GPSBaud);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");

    Serial.println(WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void loop()
{
    smartDelay(500);
    MQTT_connect();

    float Distance_To_Home;
    float GPSlat = (gps.location.lat()); // variable to store latitude float GPSlng = (gps.location.lng());	// variable to store longitude float GPSalt = (gps.altitude.feet());

    Distance_To_Home = (unsignedlong)TinyGPSPlus::distanceBetween(gps.location.lat(), gp s.location.lng(), HOME_LAT, HOME_LNG); // Query Tiny GPS to Calculate Distance to
    char gpsbuffer[40];                                                                                                         // Combine Latitude, Longitude, char *p = gpsbuffer;		// Create a buffer to store GPS

    dtostrf(Distance_To_Home, 3, 4, p);
    p += strlen(p);
    p[0] = ',';
    p++;

    dtostrf(GPSlat, 3, 6, p);
    p += strlen(p);
    p[0] = ',';
    p++;

    dtostrf(GPSlng, 3, 6, p); // Convert GPSlng(longitude) to a String variable and add it to the buffer
    p += strlen(p);
    p[0] = ',';
    p++;

    dtostrf(GPSalt, 2, 1, p);
    p += strlen(p);

    p[0] = 0;

    if ((GPSlng != 0) && (GPSlat != 0))
    {
        Serial.println("Sending GPS Data ");
        gpslatlng.publish(gpsbuffer); // publish Combined Data to Adafruit IO Serial.println(gpsbuffer);
    }

    {
        Serial.println("Sending GPS Data ");
        gpslatlng.publish(gpsbuffer); // publish Combined Data Serial.println(gpsbuffer);
    }
    delay(1000);

    Serial.print("Distance from home location : ");
    Serial.println(Distance_To_Home);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        Serial.println(F("No GPS data received: check wiring"));

    Serial.println("Pausing...");

    smartDelay(500);
    delay(1000);
}
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (ss.available())
            gps.encode(ss.read());
    } while (millis() - start < ms);
    void MQTT_connect()
    {
        int8_t ret;
        if (mqtt.connected())
        {
            return;
        }
        Serial.print("Connecting to MQTT... ");
        uint8_t retries = 3;
        while ((ret = mqtt.connect()) != 0)
        { // connect will return 0 for connected Serial.println(mqtt.connectErrorString(ret));
            Serial.println("Retrying MQTT connection in 5 seconds...");
            mqtt.disconnect();
            delay(5000); // wait 5 seconds retries--;
            if (retries == 0)
        }
        Serial.println("MQTT Connected!");
    }
}
