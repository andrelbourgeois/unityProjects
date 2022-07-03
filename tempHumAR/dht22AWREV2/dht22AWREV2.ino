// #include <ESP8266WiFi.h> // import the ESP8266 wifi library
#include <ArduinoMqttClient.h> // enables arduino to send and recieve messages over mqtt 
#include <WiFiNINA.h> // enables wifi connection for arduino wifi rev 2 
// #include <WiFimqttClient.h> // import WifimqttClient Library
// #include <PubSubmqttClient.h> // import PubSubmqttClient Library
#include <DHT.h> // import DHT22 library
#include <ezTime.h> // import ezTime library
#include "arduino_secrets.h"

const char* ssid = SECRET_SSID; // wifi network name here
const char* password = SECRET_PASS; // wifi password here
const char* mqtt_server = "test.mosquitto.org"; // declare the mqtt broker to use
const char* topic = "temperature and humidity";

#define DHTPIN 2 // set anaalog pin for the sensor at pin A0
#define DHTTYPE DHT22 // set the sensor type
#define DELAYVAL 1000 // define a delay value (makes it easier to make changes in larger code files)

Timezone GB; // declare timezone for ezTime

// WiFimqttClient espmqttClient;
// PubSubmqttClient mqttClient(espmqttClient);
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temp; // declare temp variable as type float
float hum; // declare hum variable as type float

DHT dht(DHTPIN, DHTTYPE); // initialize the DHT sensor

void setup() { // initialize variables’ values,
 // setup communications (ex: Serial),
 // setup modes for digital pins (input/output),
 // initialize any hardware component (sensor/actuator) plugged to the microcontroller
 
  Serial.begin(9600); // initiate serial connection
  delay(100); // to give time for the serial connection to open

  startWifi(); // start a wifi access point, and try to connect to some given access points

  pinMode(DHTPIN, INPUT); // sets the DHTPIN (pin D4) as the input pin
  dht.begin(); // starts the DHT sensor

  // get real date an time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

  // once connected to wifi, establish mqtt connection
  mqttClient.connect(mqtt_server, 1883);

}

void startWifi() {
  delay(100);
  // connect to the wifi network
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("...");
    }
   Serial.println("");
   Serial.println("wifi connected");
   Serial.println("ip address: ");
   Serial.println(WiFi.localIP());
  }

void reconnect(){
  while(!mqttClient.connected()){
    // loop until reconnected
    Serial.print("attempting mqtt connection...");
    // create random mqttClient id
    String mqttClientId = "arduinomqttClient-";
    mqttClientId += String(random(0xffff), HEX);
    // attempt to connect
    // if mqtt broker has mqttClientId, username, and password
    // change following line to; if (mqttClient.connect(mqttClientId,userName,passWord))
    if (mqttClient.connect(mqttClientId.c_str())){
      Serial.println("connected");
      // once connected, subscribe commenad if any
      mqttClient.subscribe("OsoyooCommand");
    } else {
        Serial.print("failed, rc=");
        // Serial.print(mqttClient.state());
        Serial.println("try again in 5 seconds");
        // wait 5 seconds before trying
        delay(5000);
      }
    }
  }

void loop() {
  if(!mqttClient.connected()){
    reconnect();
  }
  
  Serial.print("connected to mqtt client");
  Serial.println();

  // read temperature and humidity
  temp = dht.readTemperature(); // get the temperature value from DHT sensor, assign it to temp
  hum = dht.readHumidity(); // get humidity value from DHT sesnor, assign it to hum

  Serial.print("Temperature: "); // prints temp value in serial monitor for debugging
  Serial.println(temp);

  Serial.print("Humidity: "); // prints hum value in the serial monitor for debugging
  Serial.println(hum);

   String msg = "{\"temperature\":";
   msg = msg + temp;
   msg = msg + ",\"humidity\":";
   msg = msg + hum;
   msg = msg + "}";
   char message[58];
   mqttClient.beginMessage(topic);
   mqttClient.print(msg);
   mqttClient.endMessage();

  delay(30*DELAYVAL);

}
