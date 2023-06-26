#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int LED_PIN = 2; // Número de pin correspondiente al LED integrado

float latitude;
float longitude;
float altitude;
float velocity;
int satellites;


void setup() {
  // Inicializar la comunicación I2C
  Wire.begin();

  // Inicializar la pantalla OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Configurar el tamaño y el color del texto
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Configurar el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Generar coordenadas GPS aleatorias
  randomSeed(analogRead(0));
  latitude = random(-90, 90);
  longitude = random(-180, 180);
  altitude = random(0, 5500);
  velocity = random(0, 300);
  satellites = random(0, 10);

  // Mostrar mensaje "Conectando" mientras se establece la conexión
  displayMessage("Conectando", (1000,3000), true);

  // Mostrar mensaje "Buscando Satélites" mientras se busca la señal
  displayMessage("Buscando Satelites", (1000,4000), true);


  // Encender el LED antes de mostrar los datos
  digitalWrite(LED_PIN, HIGH);

  // Mostrar mensaje "Conectado" y las coordenadas GPS
  displayMessage("Conectado", 2000, false);
  displayCoordinates();
 // Configurar la comunicación serie con una velocidad de 9600 bps
  Serial.begin(9600);
}

void loop() {


  // Generar nuevas coordenadas GPS aleatorias
  latitude += random(-1, 2) * 0.001;
  longitude += random(-1, 2) * 0.001;
  altitude = constrain(altitude + random(-10, 11), 0, 5500);
  velocity = constrain(velocity + random(-10, 11), 0, 300);
  satellites = random(0, 10);

  // Actualizar la pantalla con las nuevas coordenadas
  displayCoordinates();

  delay(2000); // Esperar 2 segundos antes de generar nuevas coordenadas
}

void displayCoordinates() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("GPS Data:");
  display.print("Lat: ");
  display.println(latitude, 6);
  display.print("Lon: ");
  display.println(longitude, 6);
  display.print("Alt: ");
  display.print(altitude, 2);
  display.println(" m");
  display.print("Vel: ");
  display.print(velocity, 2);
  display.println(" km/h");
  display.print("Sat: ");
  display.println(satellites);
  display.display();

  // Mostrar los datos en la consola serial
  Serial.println("GPS Data:");
  Serial.print("Lat: ");
  Serial.println(latitude, 6);
  Serial.print("Lon: ");
  Serial.println(longitude, 6);
  Serial.print("Alt: ");
  Serial.print(altitude, 2);
  Serial.println(" m");
  Serial.print("Vel: ");
  Serial.print(velocity, 2);
  Serial.println(" km/h");
  Serial.print("Sat: ");
  Serial.println(satellites);
  Serial.println();
}

void displayMessage(const char *message, unsigned long duration, bool showDots) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  int dotCount = 0;

  while (elapsedTime < duration) {
    display.clearDisplay();
    display.setCursor((SCREEN_WIDTH - strlen(message) * 6) / 2, (SCREEN_HEIGHT - 8) / 2);

    display.print(message);

    if (showDots) {
      display.setCursor((SCREEN_WIDTH - strlen(message) * 6) / 2 + strlen(message) * 6, (SCREEN_HEIGHT - 8) / 2);
      for (int i = 0; i < dotCount; i++) {
        display.print('.');
      }
    }

    display.display();

    // Actualizar los puntos en cada iteración
    if (showDots) {
      dotCount = (dotCount + 1) % 4;
    }

    elapsedTime = millis() - startTime;
  }

  display.clearDisplay();
  display.display();
}
