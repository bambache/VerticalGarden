#include <DHT.h>

//Constants
const int RELAYPIN = 2;
const int SPKPIN   = 9;
const int DHTPIN   = 7;

const int beepDelay      = 200;  // microseconds
const int humThreshold   = 85;   // percent
const long debounceDelay = 10*60000;// milliseconds

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// DHT Variables
//int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

//Relay
volatile byte relayState = LOW;

// Timer Variables
long lastDebounceTime = 0;  

void setup()
{
  // Pin for relay module set as output
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH);

  // speaker
  pinMode(SPKPIN, OUTPUT);
  
  Serial.begin(9600);
  dht.begin();
}

//void beep()
//{
//  for (int i=0; i<500; i++) {
//    digitalWrite(SPKPIN, HIGH);
//    delayMicroseconds(beepDelay);
//    digitalWrite(SPKPIN, LOW);
//    delayMicroseconds(beepDelay);
//  }
//}

void loop()
{
    //delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    if (hum < humThreshold){
      tone(SPKPIN, 1000);
      delay(beepDelay);
      noTone(SPKPIN);
      Serial.println("SPK ON");
    }
    else {
      Serial.println("SPK OFF");
    }

    if(hum < humThreshold){
      if(relayState == LOW){
        digitalWrite(RELAYPIN, LOW);
        relayState = HIGH;
        Serial.println("ON");
      }

      // update time here so we debounce after getting below threshold
      lastDebounceTime = millis();+
      Serial.print("Update debouce time: ");
      Serial.println(lastDebounceTime);
    }

    // If x seconds have passed, the relay is turned off
    if((millis() - lastDebounceTime) > debounceDelay && relayState == HIGH){
      digitalWrite(RELAYPIN, HIGH);
      relayState = LOW;
      Serial.println("OFF");
    }
    
    delay(5000);
}
