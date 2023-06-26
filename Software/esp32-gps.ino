#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

static const int RXPin = 4, TXPin = 5;   // GPIO 4=D2(conectar Tx del GPS) y GPIO 5=D1(conectar Rx del GPS)
static const uint32_t GPSBaud = 9600; //si la velocidad de baudios de 9600 no funciona en tu caso, usa 4800

TinyGPSPlus gps;      // El objeto TinyGPS++
WidgetMap myMap(V0);  // V0 para el pin virtual del widget de mapa

SoftwareSerial mygps(RXPin, TXPin);  // Conexión serie al dispositivo GPS

BlynkTimer timer;

float latitude;     // Almacena la latitud
float longitude;    // Almacena la longitud
float velocity;     // Variable para almacenar la velocidad
float sats;         // Variable para almacenar el número de satélites
String bearing;     // Variable para almacenar la orientación o dirección del GPS

char auth[] = "********************";            // Token de autenticación de Blynk
char ssid[] = "********************";            // SSID de la red WiFi
char pass[] = "********************";            // Contraseña de la red WiFi

unsigned int move_index = 1;       // Ubicación fija por ahora

void setup()
{
  Serial.begin(115200);
  Serial.println();
  mygps.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); // cada 5 segundos verifica si el GPS está conectado, solo es necesario hacerlo una vez
}

void checkGPS()
{
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No se detecta GPS: verifica la conexión."));
    Blynk.virtualWrite(V3, "ERROR DE GPS");  // Widget de valor en V3 si no se detecta el GPS
  }
}

void loop()
{
  while (mygps.available() > 0)
  {
    // el boceto muestra información cada vez que una nueva sentencia se codifica correctamente.
    if (gps.encode(mygps.read()))
      displayInfo();
  }
  Blynk.run();
  timer.run();
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    sats = gps.satellites.value();       // obtiene el número de satélites
    latitude = (gps.location.lat());     // almacena la latitud y longitud
    longitude = (gps.location.lng());
    velocity = gps.speed.kmph();         // obtiene la velocidad
    bearing = TinyGPSPlus::cardinal(gps.course.value());     // obtiene la dirección

    Serial.print("SATÉLITES:  ");
    Serial.println(sats);  // muestra el número de satélites
    Serial.print("LATITUD:  ");
    Serial.println(latitude, 6);  // muestra la latitud con 6 decimales
    Serial.print("LONGITUD: ");
    Serial.println(longitude, 6);
    Serial.print("VELOCIDAD: ");
    Serial.print(velocity);
    Serial.println("km/h");
    Serial.print("DIRECCIÓN: ");
    Serial.println(bearing);

    Blynk.virtualWrite(V1, String(latitude, 6));
    Blynk.virtualWrite(V2, String(longitude, 6));
    Blynk.virtualWrite(V3, sats);
    Blynk.virtualWrite(V4, velocity);
    Blynk.virtualWrite(V5, bearing);
    myMap.location(move_index, latitude, longitude, "GPS_Location");
  }
}

