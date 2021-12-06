//VERSION 0.1
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial BTSerial(10, 11);


byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
String WorkingString="";
long DisplayValue;
long A;



void setup() {
  lcd.begin(20, 4);


  lcd.setCursor(0, 0);
  lcd.print("Enter Welcome text here");
  lcd.setCursor(0, 1);
  lcd.print("Enter Welcome text here, second row");
  delay(3000);
  lcd.clear();

//********************************************************************
  Retry:
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("Connessione...");  //connection message

  BTSerial.begin(38400);
  Serial.begin(9600);

  BTSerial.println("ATZ");	//BTSerial command
  lcd.setCursor(0, 0);
  lcd.print("Arduino ECU OBD");
  lcd.setCursor(0,1);
  lcd.print("Wooler97");
  lcd.setCursor(0,2);
  lcd.print("2017 Project");
  delay(2000);
  ReadData();

  if (BuildINString.substring(1,3)=="TZ")
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Benvenuto a bordo");
      lcd.setCursor(9, 0);
      //lcd.print(BuildINString);
      lcd.setCursor(0, 1);
      lcd.print("Connessione OK");
      delay(1500);
      lcd.clear();
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("--------------------");
      lcd.setCursor(3, 1);
      lcd.print("CONNESSIONE");
      lcd.setCursor(5, 2);
      lcd.print("FALLITA!")
      lcd.setCursor(0, 3);
      lcd.print("--------------------");
      delay(1500);
      lcd.clear();
      goto Retry;
    }

   BTSerial.println("0100");
   lcd.setCursor(0, 0);
   lcd.print("Inizializzazione...");
   delay(4000);
   ReadData();
   lcd.setCursor(0, 0);
   lcd.print("Inizializzato!      ");
   delay(1000);
   lcd.clear();
}

void loop()
{
  //"Temperatura Motore" on display
  lcd.setCursor(0, 0);
  lcd.print("--------FORD--------");
  lcd.setCursor(0,1);
  lcd.print("Temperatura Motore"); //"Engine Temperature"


  BuildINString = "";

  BTSerial.println("0105");  //Sends PID 0105 (Engine Coolant Temperature) to the ECU
  // BTSerial.flush();       //idk if it's useful, i'll try
  delay(1000);


  ReadData();
  WorkingString = BuildINString.substring(11,13);

  A = strtol(WorkingString.c_str(),NULL,16);  //hex to decimal conversion

  DisplayValue = A;
  DisplayString = String(DisplayValue - 40); //+ " C            ";  //wikipedia says that i have to subtract 40 from the decimal result to receive the correct temperature
  lcd.setCursor(0, 2);
  lcd.print(DisplayString);
  lcd.print(char(223));
  lcd.print("C");
  lcd.setCursor(0,3);
  lcd.print("-------FIESTA-------");
  delay(500);

//Control if it's over 100 C ******************************************************
  int B;
  B = DisplayString.toInt();  //string to integer


  if (B >= 100){     //control if the temperature goes up to 100°C

// ------- 3 blink of brightness  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();   //For I2C displays use lcd.backlight
    delay(250);
    lcd.noBacklight(); //For I2C displays use lcd.noBacklight
    delay(250);
  }
  lcd.backlight(); // END with backlight ON   //For I2C displays use lcd.backlight
   }

}

//Data Read function ***********************************************************
void ReadData()
{
  Serial.println("Lettura info da BT..");
  BuildINString="";
  while(BTSerial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = BTSerial.read();
    inChar=char(inData);
    Serial.print(inChar);
    BuildINString = BuildINString + inChar;
  }
}
