#include <DHT.h> // import DHT22 library

#define DHTPIN 2 // set digital pin for the sensor at pin 2
#define DHTTYPE DHT22 // set the sensor type
#define DELAYVAL 2000 // define a delay value (makes it easier to make changes in larger code files)

float temp; // declare temp variable as type float
float hum; // decalre hum variable as type float

DHT dht(DHTPIN, DHTTYPE); // initialize the DHT sensor

void setup() {
  Serial.begin(9600); // initiate serial connection

  pinMode(DHTPIN, INPUT); // sets the DHTPIN (pin 2) as the input pin
  dht.begin(); // starts the DHT sensor

}

void loop() {
  temp = dht.readTemperature(); // gets temperature value from DHT sensor, assigns it to temp
  hum = dht.readHumidity(); // get humidity value from DHT sesnor, assign it to hum

  Serial.print("Temperature: "); // prints temp value in serial monitor for debugging
  Serial.println(temp);

  Serial.print("Humidity: "); // prints hum value in the serial monitor for debugging
  Serial.println(hum);

  delay(DELAYVAL); // delays loop by DELAYVAL (500ms)

}
