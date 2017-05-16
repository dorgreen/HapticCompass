#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <LowPower.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
sensors_event_t event;
float heading;
float headingDegrees;

/*
 * Parameters
 */
float declinationAngle = 7.7;
int actions[] = {8,9,10,11};
int actionTime = 500; // milliseconds
boolean hardwareTest = false;
boolean debug = false;
boolean serial_singleton = false;

void serialSingleton(){
  if(!serial_singleton){
    Serial.begin(9600);
    serial_singleton = true; 
  } 
}

void displayReadings(){
  serialSingleton();
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
}

void act(float headingDegrees){
  int spot = (int) (headingDegrees * ((sizeof(actions)/sizeof(int))) / 360);
  
  if(debug){
    serialSingleton();
    Serial.println(spot);
    Serial.println(headingDegrees);
    Serial.println();
    delay(100);  
  }
 
   
  digitalWrite(actions[spot], HIGH);
  delay(actionTime);
  digitalWrite(actions[spot], LOW);
  
}

void emergency(){
  pinMode(13, OUTPUT);
  while(true){
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
  }
  
  }

void setup() {

  pinMode(13, OUTPUT);
  for(int i=0 ; i < 3 ; ++i){
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(40);
  }
    
  if(!mag.begin()){
    serialSingleton();
    Serial.println("Check Wiring!");
    emergency();
  }

  for(int i=0 ; i < (sizeof(actions)/sizeof(int)) ; ++i){
    pinMode(actions[i], OUTPUT);
  }
}

void loop() {
  mag.getEvent(&event);
  
  if(hardwareTest){
    displayReadings();
  }

  heading = atan2(event.magnetic.y, event.magnetic.x) + declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  headingDegrees = heading * 180/M_PI; 

  //Serial.println(headingDegrees);
  act(headingDegrees);

  LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF); 
  LowPower.idle(SLEEP_500MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  // "TWI_OFF" turns off Wire\I2C. could mess with magnometer...

}
