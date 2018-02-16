/*
 * Name: Atomatic watering System
 */
//#define SERIAL_DEBUG


int redPin= 11;               //RGB LED RED Pin
int greenPin = 10;            //RGB LED GREEN Pin
int bluePin = 9;              //RGB LED BLUE Pin
int relais = 7;               //Relais Pin - on boot = OFF
int dryValue = 1023;          //Moisture Sensor calibration for dry value
int wetValue = 350;           //Moisture Sensor calibration for wet value
int friendlyDryValue = 0;     //Moisture Sensor conversion for dry value
int friendlyWetValue = 100;   //Moisture Sensor conversion for wet value
int Pump = 13;                //5v Water Pump - For testing, pin 13 (onboard LED of the arduino)

void setup() {
  Serial.begin(9600);         //Open the serial with 9600 baud
  pinMode(relais, OUTPUT);    //Declare the relay as an output Pin
  pinMode(Pump, OUTPUT);      //Declare the Pump as an output Pin
  pinMode(redPin, OUTPUT);    //Declare the red LED Pin as an output Pin
  pinMode(greenPin, OUTPUT);  //Declare the green LED Pin as an output Pin
  pinMode(bluePin, OUTPUT);   //Declare the blue LED Pin as an output Pin
  digitalWrite(relais, HIGH); //turn off the relais after boot
}

void loop() {
  int rawValue = analogRead(A0);    //Read the moisture sensor value
  
  #ifdef SERIAL_DEBUG
  Serial.print("Raw: ");            //Print to the Serial (for debugging)
  Serial.print(rawValue);           //Print to the Serial (for debugging)
  Serial.print(" | ");              //Print to the Serial (for debugging)
  #endif
  
  int friendlyValue = map(rawValue, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
  
  #ifdef SERIAL_DEBUG
  Serial.print("Friendly: ");       //Print to the Serial (for debugging)
  Serial.print(friendlyValue);      //Print to the Serial (for debugging)
  Serial.println("%");              //Print to the Serial (for debugging)
  #endif
  
setColor(0, 0, 0); // Green Color

  if (friendlyValue < 50) {
    digitalWrite(Pump, HIGH);
    digitalWrite(relais, LOW);
    setColor(255, 0, 0); // Red Color
    delay(3000);
    digitalWrite(Pump, LOW);
    digitalWrite(relais, HIGH);
    setColor(0, 255, 0); // Green Color
    delay(1000);
    }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
