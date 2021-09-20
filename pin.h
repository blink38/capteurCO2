/**
 * Définition des PINs
 */

// capteur de CO2 : MHZ-19B

#define PIN_MHZ19_RX 10                                          // Rx pin which the MHZ19 Tx pin is attached to
#define PIN_MHZ19_TX 12                                          // Tx pin which the MHZ19 Rx pin is attached to



// écran LCD Nokia 5110

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// PIN_LCD_DC = 5  - Data/Command select (D/C)
// pPIN_LCD_SCE =7 - LCD chip select (CS)
// PIN_LCD_RST = 6 - LCD reset (RST)

const int PIN_LCD_SCE = 7;   // SCE - Chip select, pin 3 on LCD.
const int PIN_LCD_RST = 6;   // RST - Reset, pin 4 on LCD.
const int PIN_LCD_DC = 5;    // DC - Data/Command, pin 5 on LCD.
const int PIN_LCD_MOSI = 11;  // DN(MOSI) - Serial data, pin 6 on LCD.
const int PIN_LCD_SCLK = 13;  // SCLK - Serial clock, pin 7 on LCD.
const int PIN_LCD_LIGHT = 9;    // LED - Backlight LED, pin 8 on LCD.


#define PIN_LED_OK 2
#define PIN_LED_WARN 3
#define PIN_LED_ALERT 4

const int PIN_BTN_1= 8;
