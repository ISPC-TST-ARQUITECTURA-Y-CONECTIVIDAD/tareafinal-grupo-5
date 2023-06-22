#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

#define ANCHO_PANTALLA 128 // Ancho de la pantalla OLED
#define ALTO_PANTALLA 64 // Alto de la pantalla OLED

// En ESP32: GPIO-21(SDA), GPIO-22(SCL)
#define RESET_OLED -1 // Pin de reinicio # (o -1 si se comparte el pin de reinicio de Arduino)
#define DIRECCION_PANTALLA 0x3C 
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, RESET_OLED);

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);

TinyGPSPlus gps;

void setup() {
Serial.begin(115200);
// Iniciar comunicación serial

// Iniciar comunicación serial con Neo6mGPS
neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);

// SSD1306_SWITCHCAPVCC = generar voltaje de pantalla desde 3.3V 
if(!display.begin(SSD1306_SWITCHCAPVCC, DIRECCION_PANTALLA)) {
Serial.println(F("Fallo en la asignación de SSD1306"));
for(;;); // No continuar, bucle infinito
}

display.clearDisplay();
display.display();
delay(2000);
}

void loop() {

boolean nuevosDatos = false;
for (unsigned long inicio = millis(); millis() - inicio < 1000;)
{
while (neogps.available())
{
if (gps.encode(neogps.read()))
{
nuevosDatos = true;
}
}
}


if(nuevosDatos == true)
{
nuevosDatos = false;
Serial.println(gps.satellites.value());
imprimirVelocidad();
}
else
{
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.setTextSize(3);
display.print("Sin Datos");
display.display();
}
}

void imprimirVelocidad()
{
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);

if (gps.location.isValid() == 1)
{
//String gps_speed = String(gps.speed.kmph());
display.setTextSize(1);


display.setCursor(25, 5);
display.print("Lat: ");
display.setCursor(50, 5);
display.print(gps.location.lat(), 6);

display.setCursor(25, 20);
display.print("Lng: ");
display.setCursor(50, 20);
display.print(gps.location.lng(), 6);

display.setCursor(25, 35);
display.print("Velocidad: ");
display.setCursor(65, 35);
display.print(gps.speed.kmph());

display.setTextSize(1);
display.setCursor(0, 50);
display.print("SAT:");
display.setCursor(25, 50);
display.print(gps.satellites.value());

display.setTextSize(1);
display.setCursor(70, 50);
display.print("ALT:");
display.setCursor(95, 50);
display.print(gps.altitude.meters(), 0);

display.display();

}
else
{
display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setCursor(0, 0);
display.setTextSize(3);
display.print("Sin Datos");
display.display();
}
}
