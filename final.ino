#include <LiquidCrystal.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <uRTCLib.h>
#include <Stepper.h>

#define RDA 0x80
#define TBE 0x20

volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1; 
volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
volatile unsigned int  *myUBRR0 = (unsigned int *) 0x00C4;
volatile unsigned char *myUDR0 = (unsigned char *)0x00C6; 

unsigned char *portDDRB = (unsigned char *) 0x24;
unsigned char *portB = (unsigned char *) 0x25;

volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1 = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 = (unsigned char *) 0x36;

volatile unsigned char *portF = (unsigned char*) 0x31;
unsigned char *portDDRF = (unsigned char*) 0x30;
volatile unsigned char* pinF = (unsigned char*) 0x2F;

uRTCLib rtc(0x68);
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);
DHT dht(PIN, TYPE);
Stepper myStepper(steps, 18, 19, 20, 21); 
void setup(){
    dht.begin();
    myStepper.setSpeed(10);
    U0init(9600);
    serial.begin(9600);
    delay(3000);
    lcd.begin(16, 2);
    *portDDRB | = 0x40;     //set to output
    *portB & = 0xBF;        // set low
    U0init(9600);
    lcd.print("working");
    adc_init();
    Serial.begin(9600);
}

void loop() {
    Serial.println(getTime());
    lcd.setCursor(0,1); 
    *myTCCR1A = 0x00;
    *myTCCR1B = 0x00;
    *myTCCR1C = 0x00;
    double humidity = dht.readHumidity();
    float temperature = dht.readTemperature(true);
    *myTIFR1 |= 0x01;
    lcd.print(millis() / 1000); // number of time since reset

}
void u0init(unsigned long U0baud) {
    unsigned int tbaud;
    tbaud = (16000000 / 16 / U0baud - 1);
    *myUCSR0A = 0x20;
    *myUCSR0B = 0x18;
    *myUCSR0C = 0x06;
    *myUBRR0 = tbaud;
}
unsigned char U0kbhit() 
{
    return (*myUCSR0A & RDA) ? true : false;
}

unsigned char U0getchar() {
    return *myUDR0;
}

void U0putchar(unsigned char U0pdata)
{
    while((*myUCSR0A & TBE) ==0);
    *myUDR0 = U0pdata;
}
void adc_init(){

    *my_ADCSRA | = 0b1000000;
}
