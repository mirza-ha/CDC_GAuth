//
//  main.cpp
//  CDC_GAuth
//
//  Created by Mirza Hadžiomerović on 24/12/2018.
//  Copyright © 2018 Mirza Hadžiomerović. All rights reserved.
//  Codecta Sarajevo
//

#include "SHA1.h"
#include <TimeLib.h>
#include "DS1302.h"

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

///////////////// OLED Properties ////////////////
// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


const unsigned char PROGMEM C_logo [] =
{
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xf8,0x00,0x03,0xff,0xff,0xff
,0xff,0xff,0xc0,0x00,0x00,0x3f,0xff,0xff
,0xff,0xff,0x00,0x00,0x00,0x07,0xff,0xff
,0xff,0xfe,0x00,0x00,0x00,0x07,0xff,0xff
,0xff,0xf8,0x00,0x00,0x00,0x01,0xff,0xff
,0xff,0xe0,0x00,0x00,0x00,0x00,0x7f,0xff
,0xff,0xc0,0x00,0x00,0x00,0x00,0x7f,0xff
,0xff,0x80,0x00,0x00,0x00,0x00,0x3f,0xff
,0xff,0x00,0x00,0x00,0x00,0x00,0x0f,0xff
,0xfe,0x00,0x00,0x00,0x00,0x00,0x07,0xff
,0xfc,0x00,0x00,0x00,0x00,0x00,0x07,0xff
,0xfc,0x00,0x00,0x00,0x00,0x00,0x01,0xff
,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0xff
,0xf0,0x00,0x00,0x7f,0xf0,0x00,0x01,0xff
,0xe0,0x00,0x03,0xff,0xfc,0x00,0x01,0xff
,0xe0,0x00,0x07,0xff,0xfe,0x00,0x07,0xff
,0xc0,0x00,0x1f,0xff,0xff,0x80,0x0f,0xff
,0xc0,0x00,0x3f,0xff,0xff,0x80,0x0f,0xff
,0xc0,0x00,0x3f,0xff,0xff,0xc0,0x1f,0xff
,0xc0,0x00,0x7f,0xff,0xff,0xe0,0x7f,0xff
,0x80,0x00,0xff,0xff,0xff,0xf0,0xff,0xff
,0x80,0x01,0xff,0xff,0xff,0xf8,0xff,0xff
,0x80,0x01,0xff,0xff,0xff,0xff,0xff,0xff
,0x80,0x03,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0x80,0x03,0xff,0xff,0xff,0xff,0xff,0xff
,0x80,0x03,0xff,0xff,0xff,0xff,0xff,0xff
,0x80,0x01,0xff,0xff,0xff,0xfb,0xff,0xff
,0x80,0x01,0xff,0xff,0xff,0xf0,0xff,0xff
,0x80,0x00,0xff,0xff,0xff,0xe0,0x7f,0xff
,0x80,0x00,0x7f,0xff,0xff,0xe0,0x7f,0xff
,0xc0,0x00,0x3f,0xff,0xff,0x80,0x1f,0xff
,0xc0,0x00,0x1f,0xff,0xff,0x00,0x0f,0xff
,0xc0,0x00,0x0f,0xff,0xff,0x00,0x0f,0xff
,0xe0,0x00,0x07,0xff,0xfc,0x00,0x07,0xff
,0xe0,0x00,0x00,0xff,0xf0,0x00,0x01,0xff
,0xf0,0x00,0x00,0x24,0x80,0x00,0x00,0xff
,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0xff
,0xfc,0x00,0x00,0x00,0x00,0x00,0x01,0xff
,0xfc,0x00,0x00,0x00,0x00,0x00,0x07,0xff
,0xfc,0x00,0x00,0x00,0x00,0x00,0x07,0xff
,0xfe,0x00,0x00,0x00,0x00,0x00,0x0f,0xff
,0xff,0x80,0x00,0x00,0x00,0x00,0x3e,0x07
,0xff,0xc0,0x00,0x00,0x00,0x00,0x7c,0x07
,0xff,0xe0,0x00,0x00,0x00,0x00,0x78,0x07
,0xff,0xf8,0x00,0x00,0x00,0x00,0xf8,0x01
,0xff,0xfc,0x00,0x00,0x00,0x07,0xf8,0x01
,0xff,0xff,0x00,0x00,0x00,0x1f,0xf8,0x03
,0xff,0xff,0x80,0x00,0x00,0x3f,0xf8,0x07
,0xff,0xff,0xf8,0x00,0x03,0xff,0xfe,0x07
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };

////////////// OLED Properties End ///////////////



#define HASH_SIZE 20
#define BLOCK_SIZE 64
#define COUNTER_SIZE 8
#define DIGITS 6
#define PERIOD 30

#define BUTTON_PIN 4

enum DeviceStatus {
  DEVICE_ON,
  DEVICE_OFF
};

enum ButtonEvent {
  BUTTON_NONE = 0,
  BUTTON_CLICK = 1,
  BUTTON_HOLD = 2
};



const char *seedHex = "56C931FD6E03344216459229C573CA336FE37149"; //secret
unsigned long valid = 0;
char OTP[DIGITS + 1];

ds1302_struct rtc;

DeviceStatus DEVICE_STATUS = DEVICE_ON;
ButtonEvent BUTTON_EVENT = BUTTON_NONE;
int BUTTON_STATUS_PREV = LOW;
int BUTTON_STATUS = LOW;
int buttonHoldCnt = 0;
int sequenceCnt = 0;
bool cLogoShown = false;
bool otpShown = false;
bool timestampShown = false;


static int32_t kPinModTable[] = {
  0,
  10,
  100,
  1000,
  10000,
  100000,
  1000000,
  10000000,
  100000000,
};



// Steve Leonard, https://gist.github.com/xsleonard
unsigned char* hexstr_to_char(const char* hexstr)
{
  size_t len = strlen(hexstr);
  if (len % 2 != 0) return NULL;
  size_t final_len = len / 2;
  unsigned char* chrs = (unsigned char*)malloc((final_len + 1) * sizeof(*chrs));
  for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
    chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i + 1] % 32 + 9) % 25;
  chrs[final_len] = '\0';
  return chrs;
}


int generateGOTP(const char *seedHex,  unsigned long *validity, char *OTP)
{
  SHA1 sha1;
  DS1302_clock_burst_read( (uint8_t *) &rtc);

  union {
    uint64_t value;
    unsigned char bytes[COUNTER_SIZE];
  } counter;

  uint8_t hashResult[HASH_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned char * hashKeyBytes;
  unsigned char hashDataBytes[8];

  setUtcTime();
  long seconds = now();

  unsigned long start = ((seconds / PERIOD)) * PERIOD;
  *validity = PERIOD - (seconds - start);

  counter.value = (seconds / PERIOD);

  for (int i = 1; i <= COUNTER_SIZE; ++i) {
    hashDataBytes[i - 1] = counter.bytes[COUNTER_SIZE - i];
  }

  hashKeyBytes = hexstr_to_char(seedHex);

  sha1.resetHMAC(hashKeyBytes, HASH_SIZE);
  sha1.update(hashDataBytes, COUNTER_SIZE);
  sha1.finalizeHMAC(hashKeyBytes, HASH_SIZE, hashResult, sizeof(hashResult));

  uint8_t offset = hashResult[HASH_SIZE - 1] & 0x0f;

  uint32_t truncHash1 = *((uint32_t *)&hashResult[offset]);
  uint32_t truncHash2 = __builtin_bswap32(truncHash1);
  uint32_t truncatedHash = truncHash2 & 0x7fffffff;

  uint32_t pinValue = truncatedHash % kPinModTable[DIGITS];

  //sprintf(OTP, "%0*u\0", DIGITS, pinValue);
  sprintf(OTP, "%06ld", pinValue);

  return 0;
}


void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT);
  
  DEVICE_STATUS = DEVICE_ON;
  BUTTON_EVENT = BUTTON_NONE;
  BUTTON_STATUS_PREV = LOW;
  BUTTON_STATUS = LOW;
  buttonHoldCnt = 0;
  sequenceCnt = 0;
  cLogoShown = false;
  otpShown = false;
  timestampShown = false;
  
  DS1302_write (DS1302_TRICKLE, 0x00);
  generateGOTP(seedHex, &valid, OTP);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); 
}


void displayCLogo()
{
  display.clearDisplay();
  display.fillScreen(WHITE);
  display.display();
  display.setCursor(0,0);
  display.drawBitmap( 30, 0, C_logo, 64, 64, 2);
  display.display();
  cLogoShown = true;
}


void displayOtp()
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.setCursor(35,10);
  display.print(now());
  display.setTextSize(3);
  display.setCursor(10,30);
  display.print(OTP);
  display.display();
  otpShown = true;
}


void displayTimestamp()
{
  display.fillScreen(BLACK);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.setCursor(35,15);
  display.print(now());

  // mini goes wild when using sprintf to format text
  display.setCursor(5,40);
  display.print(rtc.Date10); display.print(rtc.Date); display.print(".");
  display.print(rtc.Month10); display.print(rtc.Month); display.print(".");
  display.print(2000 + bcd2bin( rtc.Year10, rtc.Year)); display.print(" ");  
  display.print(rtc.h24.Hour10); display.print(rtc.h24.Hour); display.print(":");
  display.print(rtc.Minutes10); display.print(rtc.Minutes); display.print(":");
  display.print(rtc.Seconds10); display.print(rtc.Seconds);
  display.display();  
    
  timestampShown = true;
}


void turnOffDevice()
{
  DEVICE_STATUS = DEVICE_OFF;
  BUTTON_EVENT = BUTTON_NONE;
  BUTTON_STATUS_PREV = LOW;
  BUTTON_STATUS = LOW;
  buttonHoldCnt = 0;
  sequenceCnt = 0;
  cLogoShown = false;
  otpShown = false;
  timestampShown = false;

  display.clearDisplay();
  display.fillScreen(BLACK);
  display.display();
  //display.ssd1306_command(SSD1306_DISPLAYOFF);
}


void turnOnDevice()
{
  DEVICE_STATUS = DEVICE_ON;
  BUTTON_EVENT = BUTTON_NONE;
  BUTTON_STATUS_PREV = LOW;
  BUTTON_STATUS = LOW;
  sequenceCnt = 0;
  cLogoShown = false;
  otpShown = false;
  timestampShown = false;
  
  //display.ssd1306_command(SSD1306_DISPLAYON);
}


void loop()
{
  delay(100);
  
  BUTTON_STATUS_PREV = BUTTON_STATUS;

  BUTTON_STATUS = digitalRead(BUTTON_PIN);

  if ((BUTTON_STATUS_PREV == HIGH) && (BUTTON_STATUS == LOW))
  {
    if (buttonHoldCnt>5)
    {
      BUTTON_EVENT = BUTTON_NONE;
      buttonHoldCnt = 0;
    } else
    {
      BUTTON_EVENT = BUTTON_CLICK;
      buttonHoldCnt = 0;
    }
  }
  else if ((BUTTON_STATUS_PREV == HIGH) && (BUTTON_STATUS == HIGH))
  {
    buttonHoldCnt += 1;
    if (buttonHoldCnt >= 30) BUTTON_EVENT = BUTTON_HOLD;
  } else
  {
    BUTTON_EVENT = BUTTON_NONE;
    buttonHoldCnt = 0;
  }


  if (BUTTON_EVENT == BUTTON_CLICK)
  {
    if (DEVICE_STATUS == DEVICE_ON)
    {
      // Turn device off. Go to pawer save mode.
      turnOffDevice();
    } else
    {
      // Turn device on. Start display sequence.
      turnOnDevice();
    }
  } else if (BUTTON_EVENT == BUTTON_HOLD)
  {
    turnOnDevice();
    displayTimestamp();
  }



  if ((DEVICE_STATUS == DEVICE_ON) && (!timestampShown))
  {
    sequenceCnt += 1;
    if ((sequenceCnt<30) && (!cLogoShown)) displayCLogo();
    if ((sequenceCnt>30) && (!otpShown)) displayOtp();
    if (sequenceCnt>330) turnOffDevice();
  }
  
  
}


void setUtcTime()
{
  //void    setTime(int hr,int min,int sec,int day, int month, int yr);
  setTime(
    bcd2bin( rtc.h24.Hour10, rtc.h24.Hour),
    bcd2bin( rtc.Minutes10, rtc.Minutes),
    bcd2bin( rtc.Seconds10, rtc.Seconds),
    bcd2bin( rtc.Date10, rtc.Date), 
    bcd2bin( rtc.Month10, rtc.Month),
    2000 + bcd2bin( rtc.Year10, rtc.Year));  
}


// --------------------------------------------------------
// DS1302_clock_burst_read
//
// This function reads 8 bytes clock data in burst mode
// from the DS1302.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
void DS1302_clock_burst_read( uint8_t *p)
{
  int i;

  _DS1302_start();

  // Instead of the address, 
  // the CLOCK_BURST_READ command is issued
  // the I/O-line is released for the data
  _DS1302_togglewrite( DS1302_CLOCK_BURST_READ, true);  

  for( i=0; i<8; i++)
  {
    *p++ = _DS1302_toggleread();
  }
  _DS1302_stop();
}

// --------------------------------------------------------
// DS1302_read
//
// This function reads a byte from the DS1302 
// (clock or ram).
//
// The address could be like "0x80" or "0x81", 
// the lowest bit is set anyway.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
uint8_t DS1302_read(int address)
{
  uint8_t data;

  // set lowest bit (read bit) in address
  bitSet( address, DS1302_READBIT);  

  _DS1302_start();
  // the I/O-line is released for the data
  _DS1302_togglewrite( address, true);  
  data = _DS1302_toggleread();
  _DS1302_stop();

  return (data);
}

// --------------------------------------------------------
// DS1302_write
//
// This function writes a byte to the DS1302 (clock or ram).
//
// The address could be like "0x80" or "0x81", 
// the lowest bit is cleared anyway.
//
// This function may be called as the first function, 
// also the pinMode is set.
//
void DS1302_write( int address, uint8_t data)
{
  // clear lowest bit (read bit) in address
  bitClear( address, DS1302_READBIT);   

  _DS1302_start();
  // don't release the I/O-line
  _DS1302_togglewrite( address, false); 
  // don't release the I/O-line
  _DS1302_togglewrite( data, false); 
  _DS1302_stop();  
}


// --------------------------------------------------------
// _DS1302_start
//
// A helper function to setup the start condition.
//
// An 'init' function is not used.
// But now the pinMode is set every time.
// That's not a big deal, and it's valid.
// At startup, the pins of the Arduino are high impedance.
// Since the DS1302 has pull-down resistors, 
// the signals are low (inactive) until the DS1302 is used.
void _DS1302_start( void)
{
  digitalWrite( DS1302_CE_PIN, LOW); // default, not enabled
  pinMode( DS1302_CE_PIN, OUTPUT);  

  digitalWrite( DS1302_SCLK_PIN, LOW); // default, clock low
  pinMode( DS1302_SCLK_PIN, OUTPUT);

  pinMode( DS1302_IO_PIN, OUTPUT);

  digitalWrite( DS1302_CE_PIN, HIGH); // start the session
  delayMicroseconds( 4);           // tCC = 4us
}

// --------------------------------------------------------
// _DS1302_stop
//
// A helper function to finish the communication.
//
void _DS1302_stop(void)
{
  // Set CE low
  digitalWrite( DS1302_CE_PIN, LOW);

  delayMicroseconds( 4);           // tCWH = 4us
}

// --------------------------------------------------------
// _DS1302_toggleread
//
// A helper function for reading a byte with bit toggle
//
// This function assumes that the SCLK is still high.
//
uint8_t _DS1302_toggleread( void)
{
  uint8_t i, data;

  data = 0;
  for( i = 0; i <= 7; i++)
  {
    // Issue a clock pulse for the next databit.
    // If the 'togglewrite' function was used before 
    // this function, the SCLK is already high.
    digitalWrite( DS1302_SCLK_PIN, HIGH);
    delayMicroseconds( 1);

    // Clock down, data is ready after some time.
    digitalWrite( DS1302_SCLK_PIN, LOW);
    delayMicroseconds( 1);        // tCL=1000ns, tCDD=800ns

    // read bit, and set it in place in 'data' variable
    bitWrite( data, i, digitalRead( DS1302_IO_PIN)); 
  }
  return( data);
}


// --------------------------------------------------------
// _DS1302_togglewrite
//
// A helper function for writing a byte with bit toggle
//
// The 'release' parameter is for a read after this write.
// It will release the I/O-line and will keep the SCLK high.
//
void _DS1302_togglewrite( uint8_t data, uint8_t release)
{
  int i;

  for( i = 0; i <= 7; i++)
  { 
    // set a bit of the data on the I/O-line
    digitalWrite( DS1302_IO_PIN, bitRead(data, i));  
    delayMicroseconds( 1);     // tDC = 200ns

    // clock up, data is read by DS1302
    digitalWrite( DS1302_SCLK_PIN, HIGH);     
    delayMicroseconds( 1);     // tCH = 1000ns, tCDH = 800ns

    if( release && i == 7)
    {
      // If this write is followed by a read, 
      // the I/O-line should be released after 
      // the last bit, before the clock line is made low.
      // This is according the datasheet.
      // I have seen other programs that don't release 
      // the I/O-line at this moment,
      // and that could cause a shortcut spike 
      // on the I/O-line.
      pinMode( DS1302_IO_PIN, INPUT);

      // For Arduino 1.0.3, removing the pull-up is no longer needed.
      // Setting the pin as 'INPUT' will already remove the pull-up.
      // digitalWrite (DS1302_IO, LOW); // remove any pull-up  
    }
    else
    {
      digitalWrite( DS1302_SCLK_PIN, LOW);
      delayMicroseconds( 1);       // tCL=1000ns, tCDD=800ns
    }
  }
}



