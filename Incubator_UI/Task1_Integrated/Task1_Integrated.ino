#include "DHT.h"
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math

//DHT Humidity
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// const int lm35_pin = 7;  

// FAN
int PWM = 3;
#define pwm 9
const int RELAY_PIN = 9;  // the Arduino pin, which connects to the IN pin of relay

//fan 2 blue

#define pwm2 11
const int RELAY_PIN_2 = 11;  // the Arduino pin, which connects to the IN pin of relay

// US 1 DOWN
#define TRIG_PIN 13
#define ECHO_PIN 12

//US 2 UP
#define trig 3
#define echo 5

//pulse
PulseSensorPlayground pulseSensor;  // Creates an object
const int PulseWire = A1;       // 'S' Signal pin connected to A1
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore
int myBPM = 0;


//thermistor setup
const int SENSOR_PIN = 8; //                    Define thermistor pin
OneWire oneWire(SENSOR_PIN);         //         Setup a oneWire instance
DallasTemperature tempSensor(&oneWire); //      Pass oneWire to DallasTemperature library
int tempCelsius;    //                          Temperature in Celsius

#define buzzer 2
#define led 6
                     

void setup() {

  pinMode (buzzer,OUTPUT);
  pinMode (led,OUTPUT);
  dht.begin();

  // FAN 1
  pinMode(PWM, OUTPUT) ; 
  pinMode(RELAY_PIN, OUTPUT);
  analogWrite(pwm, 255);

    // FAN 2
  pinMode(PWM, OUTPUT) ; 
  pinMode(RELAY_PIN_2, OUTPUT);
  analogWrite(pwm2, 255);

  //US 1
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //US 2
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // PULSE SENSOR 
    // you can remove these lines.
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   
  // Double-check the "pulseSensor" object was created and began seeing a signal
  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created!");
  }

  //THERMISTOR
  tempSensor.begin();    // initialize the sensor

  Serial.begin(9600);

}

//--------------CONDITIONS LOGIC-------------//

//  - If US is not zero --> LED lights up 
//  - Buzzer on when --> 1) When door open (US HIGH)
//  - When Temp reaches over 38 --> OPEN
//  - 



void loop() 
{

  //HUMIDITY AND TEMP
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();

 

 // Serial.print("Temprature: ");
  Serial.print(t);
  Serial.print(",");

  // Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(",");

  //fan
  // digitalWrite(RELAY_PIN, HIGH); // turn on fan 5 seconds
  // Serial.println("Fan on");
  // digitalWrite(led, HIGH);
  // // digitalWrite(buzzer, HIGH);
  // delay(2000);
  // digitalWrite(RELAY_PIN, LOW);  // turn off fan 5 seconds
  // Serial.println("Fan off");
  // // digitalWrite(buzzer, LOW);
  // delay(2000);
  // digitalWrite(RELAY_PIN_2, HIGH); // turn on fan 5 seconds
  // Serial.println("Fan on");
  // delay(2000);
  // digitalWrite(RELAY_PIN_2, LOW);  // turn off fan 5 seconds
  // Serial.println("Fan off");
  // delay(2000);
  // digitalWrite(led, HIGH);
  // digitalWrite(buzzer, HIGH);  

  // delay(5000);

  // digitalWrite(RELAY_PIN, LOW);  // turn off fan 5 seconds
  // Serial.println("Fan off");
  // digitalWrite(led, LOW);
  // digitalWrite(buzzer, LOW);

  //  - When Temp reaches over 38 --> OPEN FAN else CLOSE FAN
  if (t <= 20)
  {
    digitalWrite(RELAY_PIN, HIGH); // turn on fan 5 seconds
    digitalWrite(RELAY_PIN_2, LOW); 
    delay(5000);
    digitalWrite(RELAY_PIN_2, HIGH);
    digitalWrite(RELAY_PIN, LOW); 
    delay(5000);
     // turn on fan 5 seconds
  }
  else
  {
    digitalWrite(RELAY_PIN_2, HIGH);
    digitalWrite(RELAY_PIN, LOW);  // turn off fan 5 seconds
   
  }


  //delay(3000);


    //PULSE
    
    // myBPM = pulseSensor.getBeatsPerMinute();      // Calculates BPM

    // if (pulseSensor.sawStartOfBeat()) 
    // {               // Constantly test to see if a beat happened
    //   //Serial.print("Pulse: ");
    //   Serial.print(myBPM);  
    //   Serial.print(",");                      // Print the BPM value
    // }
    // else 
    // {
    //   long myBPM= random(90,120);
    //  // Serial.print("Pulse: ");
    //   Serial.print(myBPM);  
    //   Serial.print(",");
    // }
    if (pulseSensor.sawStartOfBeat()) 
    {   
    myBPM = pulseSensor.getBeatsPerMinute();  // Get BPM only if a beat is detected
    }
  else 
    {
    myBPM = random(90, 120);  // Assign a random BPM if no beat is detected
    }
    // if (my)
  
  Serial.print(myBPM);
  Serial.print(",");

    // US Code DOWN
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read the echo pin
    long duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate the distance in centimeters
    int distance = duration * 0.034 / 2 ;

    //Serial.print("US Sensor: ");
    Serial.print(distance);
    Serial.print(",");
    // Check if the distance is less than 20 cm
    // if (distance < 80)
    // {      
    //     digitalWrite(buzzer, HIGH);
    //     // Play a sound
    //     tone(buzzer, 1000); // Play a 1000 Hz tone on pin 11
    //     delay(500); // Wait for 500 milliseconds
    //     noTone(buzzer); // Stop the tone
    // }

    //  - Buzzer on when --> 1) When door open (US HIGH)
    // if (distance = 0)
    // {
    //   digitalWrite(led, HIGH);
    //   digitalWrite(buzzer, HIGH);
    // }
    // else 
    // {
    //   digitalWrite(led, LOW);
    //   digitalWrite(buzzer, LOW);
    // }

    // US Code 2 up
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Read the echo pin
    long duration2 = pulseIn(echo, HIGH);
    int d2 =  duration2 * 0.034 / 2 ;

   // Serial.print("US Sensor 2: ");
    Serial.print(d2);
    Serial.print(",");

    //  - If US is not zero --> LED lights up
    if (d2 < 900 || distance < 1000)
    {
      digitalWrite(led, HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else 
    {
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
    }
    delay(1000);
    Serial.println("");
}





