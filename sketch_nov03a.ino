#include <LiquidCrystal.h>
#include <Adafruit_PM25AQI.h>                               //Adafruit pm2.5 Detection Library
#include <Adafruit_BusIO_Register.h>                        //Adafruit Libs
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <SoftwareSerial.h>                                 //Serialization Library

LiquidCrystal lcd(3,4,5,6,7,8,9,10,11,12,13);               // Params: (rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7)
SoftwareSerial pmSerial(2, 0);                              //Creates a Serial w/ Input Pin 2 and Output pin 0
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();                  //New Instance of Adafruit Tech

int air;                                                    //Value for Air Quality: 0-63
int filter;                                                 //Value for Filter Quality: 0-63
int state;                                                  //Value for State of Display... 0 for Air Quality, 1 for Filter Quality, 2 for Status View

void setup() {                                              //Function for Setup. Default Call
  lcd.begin(16,2);                                          //Sets up with 16 Colums and 2 Rows

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
  
  air = 13;                                                 //Initial Value for Air Quality
  filter = 44;                                              //Initial Value for Filter Quality
  state = 0;                                                //Initial State
  updateDisplay();                                          //Call Function to Update the Display. Initializes the system.
  while(1==1) {
    loop();
   }
}

void loop() {                                               //Loop Function, Defualt Call
  updateDisplay();                                          //Calls to Update Display.
  delay(5000);                                              //Delay 5000 millis (5 Seconds) before loop.

  PM25_AQI_Data data;                                       //New Datastream
  
  if (! aqi.read(&data)) {                                  //Test if AQI Stream is Being Read Properly
    Serial.println("Could not read from AQI");              //When Failed, Print This
    delay(500);                                             //Try again in 500 Millis (1/2 Second)
    return;
  }
  updateData(data);                                         //Updata Air and Filter vars When Data can be read

  
}

void updateDisplay() {                                      //Function to update display with pertinent information. Called from loop()
  lcd.clear();                                              // Clears the LCD screen
  if (state == 0) {                                         //If statement, determines which data to display
    state = 1;                                              //Switch State next Cycle;
    twoLinePrint("Air Quality:", printValue(air));          //Print Output Using twoLinePrint()
    
   } else if(state == 1) {
    if(air <= 15 || filter <= 15) {                         //If Values Reach an Extreme, next screen will be the status screen.
      state = 2;                                            //Switch State to Status View Next Cycle
    } else {
      state = 0;                                            //Switch State to Air Quality View Next Cycle
     }

    twoLinePrint("Filter Quality:", printValue(filter));    //Print Output Using twoLinePrint()
    
   } else {                                                 //Status View
      state = 0;                                            //Switch State to Air Quality View Next Cycle
      if(air <= 15 && filter <= 15) {                       //If Both Air and Filter are in bad shape
        
        twoLinePrint("Poor Air Quality", "Replace Filter"); //Print Output Using twoLinePrint()
        
      } else if(air <= 15) {                                //If it is just Air Quality
        twoLinePrint("Extremely Poor", "Air Quality");      //Print Output Using twoLinePrint()
        
       } else {                                             //If it is just the Filter Quality
        twoLinePrint("Filter Degraded", "Replace Soon");    //Print Output Using twoLinePrint()
       }
   }
}

String printValue(int value) {                              //Prints Values for Input with Graphical Output
  String out = "Good ";                                     //Initialize Output, Set Left Label Text

  if(value > 60) {                                         //If Statments for Each Value. Value can be between 0-63, inclusive
    out+= "######";
  } else if(value > 50) {
     out+= "-#####";
  } else if(value >40) {
     out+= "--####";
  } else if(value > 30) {
     out+= "---###";
  } else if(value > 20) {
     out+= "----##";
  } else if(value > 10){
     out+= "-----#";
  } else {
     out+= "------";
  }      

  out+= " Poor";                                            //Appends Right Label Text
  return out;
}

void twoLinePrint(String one, String two) {                 //Function to hellp eliminate clutter
        lcd.setCursor(0, 0);                                //Sets Cursor to Begining of First Line
        lcd.print(one);                                     //Prints Line One
        lcd.setCursor(0, 1);                                //Sets Cursor to Begining of Second Lines
        lcd.print(two);                                     //Prints Line Two
}

void updateData(PM25_AQI_Data data) {
  air = 10;
  filter = 10;
}
