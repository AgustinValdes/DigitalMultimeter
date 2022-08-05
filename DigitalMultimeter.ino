#include <SevSeg.h>
SevSeg digiDisplay;

const int redPin = 14; //voltage
const int greenPin = 15; //amperage
const int bluePin = 16; //resistance
//User Inputs
const int redButton = 19;
const int greenButton = 18;
const int blueButton = 17;
//State of Inputs
int redState = 0;
int greenState = 0;
int blueState = 0;
const int scaleFactor = 0.868399;

//Voltage Variables
float vin = 0.0;
float vout_volts = 0.0;
float r1 = 461000.0;
float r2 = 88400.0;

//Resistance Variables
float resistance = 0.0;
float vout_ohms = 0.0;
float rknown = 10000.0;

//Amperage Variables
float amperage = 0.0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  byte digitPins[] = {10,11,12,13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool updateWithDelaysIn = true;
  digiDisplay.begin(COMMON_CATHODE, 4, digitPins, segmentPins, true);
  digiDisplay.setBrightness(200); //ranges from -200 to 200
  
  pinMode(redPin, OUTPUT);
  pinMode(redButton, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(greenButton, INPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(blueButton, INPUT);
}

void loop() {
  int rawReading_Volts = analogRead(A6);
  vout_volts = (rawReading_Volts * 5.0) / 1024.0;
  vin = ((vout_volts * (r1 + r2)) / r2)*scaleFactor;//With adjusted scale factor

  int rawReading_Ohms = analogRead(A7);
  vout_ohms = (rawReading_Ohms * 5.0) / 1024.0;
  resistance = int(round(rknown * ((5.0/vout_ohms)-1)));

  amperage = (vin/(resistance))*1000; // Amperage in milliamps
  
  Serial.println(amperage);
 
  if (digitalRead(redButton) == HIGH){ //If voltage mode is selected
    digiDisplay.setNumber(vin, 2);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
  else if (digitalRead(blueButton) == HIGH){ //If resistance mode is selected
    digiDisplay.setNumber(resistance/1000, 2);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    }
  else if (digitalRead(greenButton) ==  HIGH){// If amperage mode is selected
    digiDisplay.setNumber(amperage, 2);
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  } 
  digiDisplay.refreshDisplay();
}
