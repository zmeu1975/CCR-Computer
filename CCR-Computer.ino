#include <LiquidCrystal.h>

/*******************************************************

  This program is used to test the LCD module display and 5 buttons.

********************************************************/
bool AlmostEqualRelative(float A, float B, float maxRelDiff);
// Select the pin used on LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define the button
int lcd_key     = 0;
int adc_key_in  = 0;

//Define veribles.
int state = 1;
int SENSORSelect = 0;
float voltagePPo1 = 1.50;
float voltagePPo2 = 1.51;
float voltagePPo3 = 1.52;

// constants won't change. Used here to set a pin number :
const int Dangerled =  11;      // Set a name for the number of the LED pins
const int OKled = 12;           //
const int SYSONled = 13;       //

#define NextSENS  0
#define ValueUP   1
#define ValueDOWN 2
#define PrevSENS  3
#define Reset     4
#define btnNONE   5

//read the button value
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);          // read analog A0 value
  // when read the 5 key values in the vicinity of the following：0,144,329,504,741
  // By setting different threshold, you can read the one button
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return NextSENS;
  if (adc_key_in < 250)  return ValueUP;
  if (adc_key_in < 450)  return ValueDOWN;
  if (adc_key_in < 650)  return PrevSENS;
  if (adc_key_in < 850)  return Reset;

  return btnNONE;

}

void setup()
{
  Serial.begin(9600);  // initialize serial communication at 9600 bits per second:for debugging.
  lcd.begin(16, 2);              // star
  lcd.setCursor(3, 0);
  lcd.print("Booting..."); // display“Push the buttons”
  pinMode(Dangerled, OUTPUT);  //Sets the 11 pin as output
  pinMode(OKled, OUTPUT);  //Sets the 12 pin as output
  pinMode(SYSONled, OUTPUT); //Sets the 13 pin as output
  delay(2000);
}

void CCRMain()// the main routine for the program,
{
 digitalWrite(Dangerled, HIGH); //Turn off "Danger" led so it flases in the HUD.
  lcd.setCursor(1, 0); //Clean the LCD screen
  lcd.print("CCR O2 Sensors "); //Write the phrase "CCR O2 Sensors"
  //lcd.print(SENSORSelect); //TESTING ONLY
  //lcd.print(state); //MORE TESTING
  
  //PPo2 Sensor 1
  lcd.setCursor(0, 1);  //set screen position.
  lcd.print(voltagePPo1);// print out the value you read to lc

  //PPo2 Sensor 2
  lcd.setCursor(6, 1);  //set screen position.
  lcd.print(voltagePPo2);// print out the value you read to lcd:

  //PPo2 Sensor 3
  lcd.setCursor(12, 1);  //set screen position.
  lcd.print(voltagePPo3);// print out the value you read to lcd:

  delay(1000); 

  state = AlmostEqualRelative(voltagePPo1, voltagePPo2, 0.03);
  state = state && AlmostEqualRelative(voltagePPo2, voltagePPo3, 0.03);
  lcd.print(state);


}


void Danger()  //Routine if PP0 Dangerous.
{
  digitalWrite(Dangerled, LOW); //Turn on "Danger" led so it flases in the HUD.
  digitalWrite(OKled, HIGH); //Turn off "OK" led so it flases in the HUD.
  lcd.clear();  //Clear the lcd
  lcd.setCursor(0, 0);
  lcd.print("WARNING! BAILOUT");
  lcd.setCursor(2, 1);
  lcd.print("AND ASSEND!!");
  delay(1000);
  lcd.clear();  //Clear the lcd
//TEST CODE
  digitalWrite(Dangerled, HIGH); //Turn off "Danger" led so it flases in the HUD.
  digitalWrite(OKled, LOW); //Turn on "OK" led so it flases in the HUD.
  lcd.setCursor(1, 0); //Clean the LCD screen
  lcd.print("Sensor Problem"); //Write the phrase "CCR O2 Sensors"
  //lcd.print(SENSORSelect); //TESTING ONLY
  //lcd.print(state); //MORE TESTING
  
  //PPo2 Sensor 1
  lcd.setCursor(0, 1);  //set screen position.
  lcd.print(voltagePPo1);// print out the value you read to lc

  //PPo2 Sensor 2
  lcd.setCursor(6, 1);  //set screen position.
  lcd.print(voltagePPo2);// print out the value you read to lcd:

  //PPo2 Sensor 3
  lcd.setCursor(12, 1);  //set screen position.
  lcd.print(voltagePPo3);// print out the value you read to lcd:

  delay(1000);  
  
//END TEST CODE  
  
  
  
  state = AlmostEqualRelative(voltagePPo1, voltagePPo2, 0.03);
  state = state && AlmostEqualRelative(voltagePPo2, voltagePPo3, 0.03);
  lcd.print(state);
}


bool AlmostEqualRelative(float A, float B, float maxRelDiff)
{
  // Calculate the difference.
  float diff = fabs(A - B);
  A = fabs(A);
  B = fabs(B);
  // Find the largest
  float largest = (B > A) ? B : A;

  if (diff <= largest * maxRelDiff) return true;
  return false;
}


void buttons()
{
  lcd_key = read_LCD_buttons();  // read key

  switch (lcd_key)               // display key
  {
    case NextSENS:
      {
        if (SENSORSelect > 2) {
          SENSORSelect = 0;
        } else {
          SENSORSelect ++;
        }
        break;
      }
    case PrevSENS:
      {
        if (SENSORSelect < 1) {
          SENSORSelect = 3;
        } else {
          SENSORSelect --;
        }
        break;
      }
    case ValueUP:
      {
        lcd.print("UP    ");
        break;
      }
    case ValueDOWN:
      {
        lcd.print("DOWN  ");
        break;
      }
    case Reset:
      {
        lcd.print("Reset  ");
        voltagePPo1 = 1.50;
        voltagePPo2 = 1.50;
        voltagePPo3 = 1.55;
        break;
      }
    case btnNONE:
      {
        lcd.print("NONE  ");
        break;
      }
  }
}  
void loop()
{
  buttons();
  if (state == 0) {
    Danger();
  } else {
    digitalWrite(Dangerled, HIGH); //Turn off "Danger" led so it flases in the HUD.
    digitalWrite(OKled, LOW); //Turn on "OK" led so it flases in the HUD.
    CCRMain();
 }

}
