#include <LiquidCrystal_I2C.h>
#include <Wire.h>



#define KEY_BACKSPACE 0x08
int sensor;
char thelist[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',KEY_BACKSPACE};
char val;
String navn = "";
int buttonPin = 6;
int buttonstate = 0;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int cursorPlace;

void setup() {
  lcd.init();       
  lcd.backlight();
  Serial.begin(9600);
  lcd.begin(20, 4);  
  lcd.print("Indtast navn");
  delay(1000);
}

void loop() {
  sensor = analogRead(A0)/38;
  if(digitalRead(D6) == 1){
    lcd.setCursor(0,0);
    val = thelist[sensor];
    
    if (val != KEY_BACKSPACE){
      navn = navn + val;
    }
    else if (val == KEY_BACKSPACE){
      navn.remove(navn.length()-1);
    }
    lcd.clear();
    lcd.print(navn);
    
    if (navn.length() > 15){
      
      for(int i = 0; navn.length() - 15 > i; i++){
        lcd.scrollDisplayLeft();
      }
    }
    delay(200);
  }
   lcd.setCursor(0, 1);
    lcd.write(thelist[sensor]);
}
