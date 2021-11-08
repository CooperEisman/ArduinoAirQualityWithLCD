#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);      // Params: (rs, enable, d4, d5, d6, d7)

int air;                                    //Value for Air Quality: 0-63
int filter;                                 //Value for Filter Quality: 0-63
int state;                                  //Value for State of Display... 0 for Air Quality, 1 for Filter Quality, 2 for Status View

void setup() {                              //Function for Setup. Default Call
  lcd.begin(16,2);                          //Sets up with 16 Colums and 2 Rows
  
  air = 23;                                  //Initial Value for Air Quality
  filter = 44;                               //Initial Value for Filter Quality
  state = 0;                                //Initial State
  updateDisplay();                          //Call Function to Update the Display. Initializes the system.
  while(1==1) {
    loop();
   }
}

void loop() {                               //Loop Function, Defualt Call
  updateDisplay();                          //Calls to Update Display.
  delay(5000);                              //Delay 5000 millis (5 Seconds) before loop.
}

void updateDisplay() {                      //Function to update display with pertinent information. Called from loop()
  lcd.clear();                              // Clears the LCD screen
  if (state == 0) {                         //If statement, determines which data to display
    state = 1;                              //Switch State next Cycle;
    lcd.setCursor(0, 0);                    //Sets Cursor to Begining of First Line
    lcd.print("Air Quality:");              //Prints Title
    lcd.setCursor(0, 1);                    //Sets Cursor to Begining of Second Lines
    printValue(air);                        //Calls printValue() to print graphical view.
    
   } else if(state == 1) {
    if(air <= 15 || filter <= 15) {         //If Values Reach an Extreme, next screen will be the status screen.
      state = 2;                            //Switch State to Status View Next Cycle
    } else {
      state = 0;                            //Switch State to Air Quality View Next Cycle
     }
    
    lcd.setCursor(0, 0);                    //Sets Cursor to Begining of First Line
    lcd.print("Filter Quality:");           //Prints Title
    lcd.setCursor(0, 1);                    //Sets Cursor to Begining of Second Lines
    printValue(filter);                     //Calls printValue() to print graphical view.
    
   } else {                                 //Status View
      state = 0;                                //Switch State to Air Quality View Next Cycle
      if(air <= 15 && filter <= 15) {       //If Both Air and Filter are in bad shape
        lcd.setCursor(0, 0);                //Sets Cursor to Begining of First Line
        lcd.print("Poor Air Quality");      //Prints
        lcd.setCursor(0, 1);                //Sets Cursor to Begining of Second Lines
        lcd.print("Replace Filter");        //Prints.
      } else if(air <= 15) {                //If it is just Air Quality
        lcd.setCursor(0, 0);                //Sets Cursor to Begining of First Line
        lcd.print("Extremely Poor");        //Prints
        lcd.setCursor(0, 1);                //Sets Cursor to Begining of Second Lines
        lcd.print("Air Quality");           //Prints.
       } else {                             //If it is just the Filter Quality
        lcd.setCursor(0, 0);                //Sets Cursor to Begining of First Line
        lcd.print("Filter Degraded");       //Prints
        lcd.setCursor(0, 1);                //Sets Cursor to Begining of Second Lines
        lcd.print("Replace Soon");          //Prints.
       }
   }
}

void printValue(int value) {                //Prints Values for Input with Graphical Output
  lcd.print("Good ");                       //Left Label Text

  if(value == 63) {                         //If Statments for Each Value. Value can be between 0-63, inclusive
    lcd.print("######");
  } else if(value > 31) {
     lcd.print("-#####");
  } else if(value >15) {
     lcd.print("--####");
  } else if(value > 7) {
     lcd.print("---###");
  } else if(value > 3) {
     lcd.print("----##");
  } else if(value > 1){
     lcd.print("-----#");
  } else {
     lcd.print("------");
  }      

  lcd.print(" Poor");                       //Right Label Text
}
