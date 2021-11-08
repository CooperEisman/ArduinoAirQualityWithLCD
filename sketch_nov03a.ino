#include <LiquidCrystal.h>
#include <Adafruit_PM25AQI.h>                               //Adafruit pm2.5 Detection Library
#include <Adafruit_BusIO_Register.h>                        //Adafruit Libs
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <SoftwareSerial.h>                                 //Serialization Library

LiquidCrystal lcd(3,4,5,6,7,8,9,10,11,12,13);               // Params: (rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7)

int air;                                                    //Value for Air Quality: 0-63
int filter;                                                 //Value for Filter Quality: 0-63
int state;                                                  //Value for State of Display... 0 for Air Quality, 1 for Filter Quality, 2 for Status View

void setup() {                                              //Function for Setup. Default Call
  lcd.begin(16,2);                                          //Sets up with 16 Colums and 2 Rows
  
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

  if(value == 63) {                                         //If Statments for Each Value. Value can be between 0-63, inclusive
    out+= "######";
  } else if(value > 31) {
     out+= "-#####";
  } else if(value >15) {
     out+= "--####";
  } else if(value > 7) {
     out+= "---###";
  } else if(value > 3) {
     out+= "----##";
  } else if(value > 1){
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
