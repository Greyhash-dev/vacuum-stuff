/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <LiquidCrystal_I2C.h>
#include "esp_adc_cal.h"
#include "MathHelpers.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);

int a = 0;
int b = 0;
int LOWV_CAL = 159; // 1.5V
int HIGHV_CAL = 977; // 10.5V

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  Serial.begin(115200);
}

float po(float x, int y){
  return x*pow(10, y);
  }

float voltage_apg[47] = {2, 2.05, 2.1, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 5.8, 6, 6.2, 6.4, 6.6, 6.8, 7, 7.2, 7.4, 7.6, 7.8, 8, 8.2, 8.4, 8.6, 8.8, 9, 9.2, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9, 9.95, 10};
float preassure_apg[47] = {po(1, -6), po(8.36, -5), po(2.27, -4), po(5, -4), po(1.08, -3), po(1.68, -3), po(2.6, -3), po(3.84, -3), po(5.15, -3), po(6.87, -3), po(1.05, -2), po(1.56, -2),
po(2.1, -2), po(2.77, -2), po(3.45, -2), po(4.16, -2), po(5.04, -2), po(5.92, -2), po(8.74, -2), po(1.27, -1), po(1.71, -1), po(2.23, -1), po(2.9, -1), po(3.57, -1), po(4.35, -1), po(5.33, -1),
po(6.4, -1), po(7.67, -1), po(9.23, -1), po(1.14, 0), po(1.4, 0), po(1.66, 0), po(1.92, 0), po(2.38, 0), po(2.95, 0), po(3.51, 0), po(4.17, 0), po(5.4, 0), po(7.06, 0), 
po(9.69, 0), po(12.9, 0), po(16.6, 0), po(20.7, 0), po(33.9, 0), po(63.2, 0), po(144, 0), po(1000, 0)};
float voltage_aim[47] = {2, 2.5, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 5.8, 6, 6.2, 6.4, 6.6, 6.8, 7, 7.2, 7.4, 7.6, 7.8, 8, 8.2, 8.4, 8.6, 8.8, 9, 9.2, 9.4, 9.6, 9.8, 10};//, 9.2, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9, 9.95, 10};
float preassure_aim[47] = {po(1, -8), po(2.4, -8), po(5.8, -8), po(8.2, -8), po(1.2, -7), po(1.6, -7), po(2.3, -7), po(3.2, -7), po(4.4, -7), po(5.9, -7), po(7.9, -7), po(1, -6),
po(1.3, -6), po(1.6, -6), po(2, -6), po(2.5, -6), po(3.1, -6), po(3.9, -6), po(4.9, -6), po(6.1, -6), po(7.5, -6), po(9.3, -6), po(1.1, -5), po(1.4, -5), po(1.7, -5), po(2.1, -5),
po(2.6, -5), po(3.1, -5), po(3.9, -5), po(4.8, -5), po(6.1, -5), po(8.0, -5), po(1.1, -4), po(1.8, -4), po(2.9, -4), po(5.4, -4), po(1.5, -3), po(1, -2)};//, po(7.06, 0), 
//po(9.69, 0), po(12.9, 0), po(16.6, 0), po(20.7, 0), po(33.9, 0), po(63.2, 0), po(144, 0), po(1000, 0)};

void pr(float volts, int line, bool apg, bool disp){
  if(disp)lcd.setCursor(0, line);
  if(disp)lcd.print("                ");
  if(disp)lcd.setCursor(0, line);
  int num = 0;
  if (volts > 0){
    if(disp)lcd.print(volts+1.5);
    if(apg){
    for (byte i = 0; i < 47; i = i + 1) {
      float diff = 0;
      if(apg){diff = (volts+1.5) - voltage_apg[i];}
      else{diff = (volts+1.5) - voltage_aim[i];}
      num = i;
      if(diff < 0){break;}
      num = num + 1;
  }
    }
    else{
      for (byte i = 0; i < 47-9; i = i + 1) {
      float diff = 0;
      if(apg){diff = (volts+1.5) - voltage_apg[i];}
      else{diff = (volts+1.5) - voltage_aim[i];}
      num = i;
      if(diff < 0){break;}
      num = num + 1;
      }
    }
  float preassure = 0;
  if(disp)lcd.setCursor(6, line);
  if (0 != num and num < 47){
  float absvdiff = 0;
  float vdiff = 0;
  if(apg){
    absvdiff = (voltage_apg[num] - voltage_apg[num-1]);
    vdiff = ((volts+1.5) - voltage_apg[num-1]) / absvdiff;
    preassure = (preassure_apg[num] - preassure_apg[num-1])*vdiff + preassure_apg[num-1];
  }
  else{
    absvdiff = (voltage_aim[num] - voltage_aim[num-1]);
    vdiff = ((volts+1.5) - voltage_aim[num-1]) / absvdiff;
    preassure = (preassure_aim[num] - preassure_aim[num-1])*vdiff + preassure_aim[num-1];
  }
  if(disp)lcd.printf("%.1e",preassure);
  if(line == 1)
    {
      Serial.print("APG:ok:");
      Serial.print(sci(preassure, 4));
      Serial.print("\r\n");
    }
  if(line == 0)
    {
      Serial.print("AIM:ok:");
      Serial.print(sci(preassure, 4));
      Serial.print("\r\n");
    }
  }
  else{
    if(apg){
      if(num==0)
        {
        if(disp)lcd.printf("<%.1e",preassure_apg[0]);
        Serial.print("APG:lo:");
        Serial.print(sci(preassure_apg[0],4));
        Serial.print("\r\n");
        }
      if(num==47)
        {
        if(disp)lcd.printf(">%.1e",preassure_apg[46]);
        Serial.print("APG:hi:");
        Serial.print(sci(preassure_apg[46],4));
        Serial.print("\r\n");
        }
    }
    else{
      if(num==0)
        {
        if(disp)lcd.printf("<%.1e",preassure_aim[0]);
        Serial.print("AIM:lo:");
        Serial.print(sci(preassure_aim[0],4));
        Serial.print("\r\n");
        }
      if(num==47)
        {
        if(disp)lcd.printf(">%.1e",preassure_aim[46]);
        Serial.print("AIM:hi:");
        Serial.print(sci(preassure_aim[46],4));
        Serial.print("\r\n");
        }
      }
    }
  }
  else {
    if(disp)lcd.print("Disabled!");
    if(apg)
      {
      Serial.print("APG:of:");
      Serial.print(sci(0.0,4));
      Serial.print("\r\n");
      }
    else
      {
      Serial.print("AIM:of:");
      Serial.print(sci(0.0,4));
      Serial.print("\r\n");
      }
  }
  }

void loop(){
  for(int i=0; i<10; i++)
    {
    a = analogRead(32);
    a = readADC_Cal(a);
    b = analogRead(33);
    b = readADC_Cal(b);
    lcd.setCursor(0, 0);
    // print message
    float volts = float(float(a-LOWV_CAL)/float(HIGHV_CAL-LOWV_CAL))*9;
    pr(volts, 0, 0, 0);
    lcd.setCursor(0, 1);
    volts = float(float(b-LOWV_CAL)/float(HIGHV_CAL-LOWV_CAL))*9;
    pr(volts, 1, 1, 0);
    }
  a = analogRead(32);
  a = readADC_Cal(a);
  b = analogRead(33);
  b = readADC_Cal(b);
  lcd.setCursor(0, 0);
  // print message
  float volts = float(float(a-LOWV_CAL)/float(HIGHV_CAL-LOWV_CAL))*9;
  pr(volts, 0, 0, 1);
  lcd.setCursor(0, 1);
  volts = float(float(b-LOWV_CAL)/float(HIGHV_CAL-LOWV_CAL))*9;
  pr(volts, 1, 1, 1);
}

uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
