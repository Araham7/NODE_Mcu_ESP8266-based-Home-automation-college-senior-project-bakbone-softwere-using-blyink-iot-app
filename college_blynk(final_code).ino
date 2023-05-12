// NOTE : homlog D5 aur D6 me ultrasonic sensor laga rahen hai--//
// NOTE  : Humidity_Sensor Pin D8 se juda huwa hai of
// garden light is "D3"
// Moter_Pin is "D4"

#define BLYNK_PRINT Serial

// #include <SimpleDHT.h> // This is the header file for DHT11 sensor

#define BLYNK_TEMPLATE_ID "TMPL3T5UxkrUx"
#define BLYNK_TEMPLATE_NAME "NODEBHAIYA"
#define BLYNK_AUTH_TOKEN "3zPhKVnwiFN7ZauEZRpKvBryH3d9RdkC"

//--------------------------I2C librry---------------------------//---------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//------------------------// defining DTH11 sensor pin //----------------------------------
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define Type DHT11

int sensePin = D8; // Temperature and Humidity sense_Pin :
DHT HT(sensePin, Type);
int humidity;
int tempC;
//-------------------------------------------------------------------------------------------//

// ----------------//------------Ultrasonic ssendor-------------//---------------//

#include <HCSR04.h>

HCSR04 hc(D5, D6); // initialisation class HCSR04 (trig pin , echo pin)

//-------------------------------------------------------------------------------//

//--------------Defining the digital pins that we want to use //----------------------------
#define led1 D0         // led
#define Garden_light D3 // led
#define Moter_Pin D4    //PUMPmoter
#define LDRpin D7

// NOTE :-- "led4" ka istamal jab "LDR" HIGH_hoga(Jab raat hoga) tab bagiche me light on karnekeliye kiya jayega

//---------------------------------//--------------------------------------------------------

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "NANDI BHABAN"; // write the "name of your WiFi".
char pass[] = "susamay0303";  // write your wifi "password".

// ---------------------------------- // "tank_water_persent()" naam ka timer function banarahen hai //
BlynkTimer timer;
void tank_water_persent()
{
    Blynk.virtualWrite(V9, (100-map(hc.dist(), 0, 11, 0, 100))); // blynk virtual pin v9 command
    delay(20);
    Blynk.virtualWrite(V7, HT.readTemperature());
    delay(20);
    Blynk.virtualWrite(V8, HT.readHumidity());
    delay(20);
}

//------------Blynk_Vartual_Write--------------//---------------------------------------------
BLYNK_WRITE(V1) // Widget WRITEs to Virtual Pin V1
{
    int value = param.asInt(); // To get value as integer
    digitalWrite(led1, value);
}

//--------------------------------------//------------------------------------------------------------

void setup()
{
    Serial.begin(9600);

    lcd.init();      // to initialise the liquidrystl_I2C
    lcd.backlight(); // to on the backlight of liquidrystl_I2C

    Blynk.begin(auth, ssid, pass);
    HT.begin();
    delay(60);

    timer.setInterval(200L, tank_water_persent); // 200L stand for 200 miliSecond ke bad "tank_water_persent()" ko call karega

    //-------------Purpes of the pins , Weather it is used to take an input or to give an output //---------------
    pinMode(led1, OUTPUT);
    pinMode(Garden_light, OUTPUT);
    pinMode(Moter_Pin, OUTPUT);
    pinMode(LDRpin, INPUT);

    //------------------------//------------------------------------------------------------------------
}

void loop()
{
    Blynk.run();

    timer.run(); // to run the "tank_water_persent()" timer function

    //------ for ultrasonic sensor -------//
    Serial.print("Distance in c.m : ");
    Serial.println(hc.dist()); // return curent distance in serial              // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.

    
    //----------------------DTH11_sensor_reading--------------------------//

    humidity = HT.readHumidity();
    tempC = HT.readTemperature();
    Serial.print("Humidity : ");
    Serial.println(humidity);
    Serial.print("Temperture C : ");
    Serial.println(tempC);

    lcd.setCursor(0, 0);
    lcd.print("TempC: ");
    lcd.setCursor(7, 0);
    lcd.print(tempC);
    lcd.setCursor(10, 0);
    lcd.print("*C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.setCursor(10, 1);
    lcd.print(humidity);
    lcd.setCursor(13, 1);
    lcd.print("%");

    int LDRpin_val = digitalRead(LDRpin);
    //----Garden light programm-----//
    if (LDRpin_val == 1)
    {
        digitalWrite(Garden_light, HIGH);
    }
    if(LDRpin_val == 0)
    {
        digitalWrite(Garden_light, LOW);
    }

    int TakWater_persentage = (100 - map(hc.dist(), 0, 11, 0, 100));
    if (TakWater_persentage <= 20)
    {
        digitalWrite(Moter_Pin, HIGH);
    }

    if (TakWater_persentage >= 90)
    {
        digitalWrite(Moter_Pin, LOW);
    }

}