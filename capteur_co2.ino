#include <Arduino.h>
#include "MHZ19.h"            

#include "pin.h"
#include "config.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
                            
#include <SoftwareSerial.h>                                // Remove if using HardwareSerial

#include <arduino-timer.h>


MHZ19 MHZ19_sensor;                                             // Constructor for library
SoftwareSerial mhz19_serial(PIN_MHZ19_RX, PIN_MHZ19_TX);        // (Uno example) create device to MH-Z19 serial

unsigned long getDataTimer = 0;

Timer<1> timer_capteur; // 10 concurrent tasks, using millis as resolution
Timer<1> timer_lumiere;

// ------------------------------

// écran LCD nokia 5110
Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_LCD_SCLK, PIN_LCD_MOSI, PIN_LCD_DC, PIN_LCD_SCE, PIN_LCD_RST);
byte lcd_light_status = LCD_LIGHT_ON;


void setup()
{
    Serial.begin(SERIAL_BAUDRATE);              // Device to serial monitor feedback

    pinMode(PIN_LED_OK, OUTPUT);
    pinMode(PIN_LED_WARN, OUTPUT);
    pinMode(PIN_LED_ALERT, OUTPUT);
    pinMode(PIN_BTN_1, INPUT);
    
    
    mhz19_serial.begin(MHZ19_BAUDRATE);           // (Uno example) device to MH-Z19 serial start   
    MHZ19_sensor.begin(mhz19_serial);           // *Serial(Stream) refence must be passed to library begin(). 

    MHZ19_sensor.autoCalibration();             // Turn auto calibration ON (OFF autoCalibration(false))

    // écran lcd
    display.begin();
    display.setContrast(LCD_DEFAULT_CONTRAST);
//    lcd_lightOn();

    Serial.println("--------------------");
    Serial.println("  setup done");
    Serial.println("--------------------");
    
}

void lcd_show_temperature(float temperature){

  // https://forum.arduino.cc/t/only-one-decimal-for-a-floating-variable/170722/2
  char strTemp[5];
  dtostrf(temperature, 4,1, strTemp);
  
  display.setTextSize(2);
  display.setCursor(0, 27);
  display.print("        ");
  display.display();
  display.setCursor(0, 27);
  display.print(strTemp);
  display.print((char)167);
  display.print("C");
  display.display();  
}

void lcd_show_co2(int co2){

  display.setTextSize(2);
  display.setCursor(0, 4);
  display.print("        ");
  display.display();
  display.setCursor(0, 4);
  display.print(co2);
  display.print("ppm");
  display.display();  
}

int etat;

void loop()
{

    timer_lumiere.tick();
    timer_capteur.tick();

    timer_capteur.in(CO2_DELAY_MEASURE, prendre_les_mesures);
    
   /* if (millis() - getDataTimer >= CO2_DELAY_MEASURE )
    {
        

        getDataTimer = millis();

        
            
    //    delay(5000);
    } */

    int ancien_etat = etat;
    etat = digitalRead(PIN_BTN_1); // Rappel : bouton = 2

    if(ancien_etat != etat && etat != HIGH){
      Serial.print("appui button");
      lcd_lightOn();
      timer_lumiere.in(LIGHT_DELAY_ON, eteindre_lumiere);
    }
}

bool prendre_les_mesures(void *argument){

  int CO2; 

  /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even 
  if below background CO2 levels or above range (useful to validate sensor). You can use the 
  usual documented command with getCO2(false) */

  CO2 = MHZ19_sensor.getCO2();                             // Request CO2 (as ppm)
  
  Serial.print("CO2 (ppm): ");                      
  Serial.println(CO2);                                

  co2_led(CO2);
  
  float Temp;
  Temp = MHZ19_sensor.getTemperature(true,false);                     // Request Temperature (as Celsius)
  Serial.print("Temperature (C): ");                  
  Serial.println(Temp);          

  display.clearDisplay();
  lcd_show_co2(CO2);
  lcd_show_temperature(Temp);
  
  return true;
}


bool eteindre_lumiere(void *argument){
  lcd_lightOff();
  return false; // true to repeat the action, false to stop
}


void co2_led(int CO2){

  if (CO2 < CO2_SEUIL_WARN){
    //digitalWrite(PIN_LED_OK, HIGH);
    analogWrite(PIN_LED_OK, 250);
    digitalWrite(PIN_LED_WARN, LOW);
    digitalWrite(PIN_LED_ALERT, LOW);
  }
  if (CO2 >= CO2_SEUIL_WARN && CO2 < CO2_SEUIL_ALERT){
    digitalWrite(PIN_LED_OK, LOW);
    analogWrite(PIN_LED_OK, 250);
    digitalWrite(PIN_LED_ALERT, LOW);
  }
  if (CO2 >= CO2_SEUIL_ALERT){
    digitalWrite(PIN_LED_OK, LOW);
    digitalWrite(PIN_LED_WARN, LOW);
    digitalWrite(PIN_LED_ALERT, HIGH);
  }

  
}

void lcd_lightOn(){

  lcd_light_status = LCD_LIGHT_ON;
  analogWrite(PIN_LCD_LIGHT, LCD_LIGHT_ON_VALUE);

}

void lcd_lightOff(){

  lcd_light_status = LCD_LIGHT_OFF;
  analogWrite(PIN_LCD_LIGHT, LCD_LIGHT_OFF_VALUE);
}
