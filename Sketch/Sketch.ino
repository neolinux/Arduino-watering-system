/*
 * Name: Atomatic watering System
 */

// Definitions
//#define SERIAL_DEBUG                // Enable serial debug for the Moisture Sensor
//#define SERIAL_DEBUG_WATERLEVEL     // Enable serial debug for the Water level Sensor
#define WATERLEVEL                    // Enable the Water level Sensor

#ifdef WATERLEVEL
#define echoPin 2             // Echo Pin of the Water level Sensor
#define trigPin 3             // Trigger Pin of the Water level Sensor
const int waterLevelThreshold = 10;    // This is the minimal distance between Water and Water Sensor espressed in centimeters (full water recipient)
int emptywaterlevel = 25;     // Measure from the border to the bottom of the water recipient (empty)
#endif

int soilPin = A3;             // Soil Moisture Sensor Pin
int redPin = 11;              // RGB LED RED Pin
int greenPin = 10;            // RGB LED GREEN Pin
int bluePin = 9;              // RGB LED BLUE Pin
int relais = 7;               // Relais Pin - on boot = OFF
int dryValue = 1023;          // Moisture Sensor calibration for dry value
int wetValue = 350;           // Moisture Sensor calibration for wet value
int friendlyDryValue = 0;     // Moisture Sensor conversion for dry value
int friendlyWetValue = 100;   // Moisture Sensor conversion for wet value
//int Pump = 13;              // 5v Water Pump - For testing, pin 13 (onboard LED of the arduino)
int pump_time = 3000;         // How long the Pump needs to be on
int after_pump_time = 5000;    // The wating Time after the Pump is stopped
int minimaldryofmoisture = 50; // How much the Moisture has to be dried out (in %) before activating the relais

void setup() {
  Serial.begin(9600);         // Open the serial with 9600 baud
  pinMode(relais, OUTPUT);    // Declare the relay as an output Pin
  //pinMode(Pump, OUTPUT);    // Declare the Pump as an output Pin
  pinMode(redPin, OUTPUT);    // Declare the red LED Pin as an output Pin
  pinMode(greenPin, OUTPUT);  // Declare the green LED Pin as an output Pin
  pinMode(bluePin, OUTPUT);   // Declare the blue LED Pin as an output Pin
  digitalWrite(relais, HIGH); // Turn off the relais after boot

  #ifdef WATERLEVEL
  pinMode(trigPin, OUTPUT);   // Declare the Trigger Pin of the Water level Sensor as an output Pin
  pinMode(echoPin, INPUT);    // Declare the Echo Pin of the Water level Sensor as an input Pin
  #endif
}

void loop() {

  #ifdef WATERLEVEL
  long waterLevel;
  waterLevel = WaterLevel();
  #endif
  int rawValue = analogRead(soilPin);    // Read the moisture sensor value
  
  #ifdef SERIAL_DEBUG
  Serial.print("Raw: ");            // Print to the Serial (for debugging)
  Serial.print(rawValue);           // Print to the Serial (for debugging)
  Serial.print(" | ");              // Print to the Serial (for debugging)
  #endif
  
  int friendlyValue = map(rawValue, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
  
  #ifdef SERIAL_DEBUG
  Serial.print("Friendly: ");       // Print to the Serial (for debugging)
  Serial.print(friendlyValue);      // Print to the Serial (for debugging)
  Serial.println("%");              // Print to the Serial (for debugging)
  #endif
  
  setColor(0, 0, 0); // Shut down the LED

  #ifdef WATERLEVEL
  if (friendlyValue < minimaldryofmoisture && waterLevel < waterLevelThreshold) { // If the moisture is dried out and water is inside the recipient
  #else
  if (friendlyValue < minimaldryofmoisture) { // If the moisture is dried out
  #endif
  
//    digitalWrite(Pump, HIGH);
    digitalWrite(relais, LOW);      // Activate the Relais
    
    for (int i=0; i<3; i=i+1){      // Blink 3 times
    setColor(0, 0, 255);            // Blue Color
    delay(250);
    setColor(0, 0, 0);              // Shut down the LED
    delay(250);
    }
    
    delay(pump_time);               // Activate the Relais for the pump_time defined at the beginning of the sketch
//    digitalWrite(Pump, LOW);
    digitalWrite(relais, HIGH);     // Disable the Relais
    //setColor(0, 255, 0);          // Green Color
    //delay(after_pump_time);       // Wait more seconds after the Relais ist shutted down
    }

    if (waterLevel < emptywaterlevel && friendlyValue < minimaldryofmoisture) {
    for (int i=0; i<3; i=i+1){      // Blink 3 times
    setColor(255, 0, 0);            // Red Color
    delay(250);
    setColor(0, 0, 0);              // Shut down the LED
    delay(250);
    }
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

#ifdef WATERLEVEL
long WaterLevel()
{
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
  long duration, distance; // Duration used to calculate distance
  /* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  if (distance >= maximumRange || distance <= minimumRange){
    //Serial.println("Error: Out of range");
  }
  else {
    #ifdef SERIAL_DEBUG_WATERLEVEL
    Serial.print("Water level: ");
    Serial.print(distance);
    Serial.println("cm");
    #endif
  }
  return distance;
}
#endif
