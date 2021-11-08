#include <Adafruit_PM25AQI.h>                               //Adafruit Libs
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <SoftwareSerial.h>                                 //Serialization Library

SoftwareSerial pmSerial(2, 0);                              //Creates a Serial w/ Input Pin 2 and Output pin 0

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();                  //New Instance of Adafruit Tech

void setup() {
  Serial.begin(9600);                                       // Wait for serial monitor to open at 9600 Baud
  while (!Serial) delay(10);

  Serial.println("Adafruit PMSA003I Air Quality Sensor");   //Print When Discovered
  delay(1000);                                              //Wait one second for sensor to boot up
  pmSerial.begin(9600);                                     //Software Serial w/ Baudrate of 9600

  if (! aqi.begin_UART(&pmSerial)) {                        // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");                            //Print when Device Communication is Set Up
}

void loop() {
  PM25_AQI_Data data;                                       //New Datastream
  
  if (! aqi.read(&data)) {                                  //Test if AQI Stream is Being Read Properly
    Serial.println("Could not read from AQI");              //When Failed, Print This
    delay(500);                                             //Try again in 500 Millis (1/2 Second)
    return;
  }
  Serial.println("AQI reading success");                    //Print when Expected

  //Following Is Output Sequence                            <----------------------------

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  

  delay(1000);
}
