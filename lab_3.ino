#include <Arduino.h>
#include <LedControl.h>
#include <MD_TCS230.h>

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

const int displaysCount = 1;
const int dataPin = 26;
const int clkPin = 22;
const int csPin = 24;
LedControl lc = LedControl(dataPin, clkPin, csPin, displaysCount);

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

    lc.shutdown(0, false);
    lc.setIntensity(0, 16);
    lc.clearDisplay(0);

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 120060;
    whiteCalibration.value[TCS230_RGB_G] = 117520;
    whiteCalibration.value[TCS230_RGB_B] = 157590;
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 11280;
    blackCalibration.value[TCS230_RGB_G] = 10270;
    blackCalibration.value[TCS230_RGB_B] = 13230;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);

}

void loop() 
{
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available())
        ;
    ColorSensor.getRGB(&rgb);
    print_rgb(rgb);
    set_led(rgb);
}

void print_rgb(colorData rgb)
{
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println();
}

void set_led(colorData rgb)
{
    int R_part = (int)rgb.value[TCS230_RGB_R]/8;
    int G_part = (int)rgb.value[TCS230_RGB_G]/8;
    int B_part = (int)rgb.value[TCS230_RGB_B]/8;
    lc.clearDisplay(0);
    for (int i=0; i<R_part; i++)
    {
      lc.setLed(0, 1, i, true);
      lc.setLed(0, 2, i, true);
    }
    for (int i=0; i<G_part; i++)
    {
      lc.setLed(0, 3, i, true);
      lc.setLed(0, 4, i, true);
    }
    for (int i=0; i<B_part; i++)
    {
      lc.setLed(0, 5, i, true);
      lc.setLed(0, 6, i, true);
    }
}
