// NeoPixelTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// Included but commented out are examples of configuring a NeoPixelBus for
// different color order including an extra white channel, different data speeds, and
// for Esp8266 different methods to send the data.
// NOTE: You will need to make sure to pick the one for your platform 
//
//
// There is serial output of the current state so you can confirm and follow along
//

//#include <IRremote.h>
//#include <IRremoteInt.h>

#include <NeoPixelBus.h>

/*
int receiver = 6; // Signal Pin of IR receiver to Arduino Digital Pin 6

IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'
*/
const uint16_t PixelCount = 120; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 3;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
//NeoPixelBus<NeoRgbFeature, Neo400KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

//int resultsNum;
int stepDelay = 10;
int resetDelay = 500;

int buttonTogglePin = 9;

uint8_t mode = 0;

int buttonToggleIsPressed = false;

void setup()
{
    
    Serial.begin(9600);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();


    Serial.println();
    Serial.println("Running...");
   // irrecv.enableIRIn(); // Start the receiver

    pinMode(buttonTogglePin, INPUT_PULLUP);
}

void loop()
{
  //clearStrip();
  //Serial.println(resultsNum);
  /*
  if (irrecv.decode(&results) && results.decode_type == UNKNOWN){ // have we received an IR signal?
    
    resultsNum = results.value;
    if(results.value != 0) Serial.println(results.value, HEX);
    irrecv.resume();
  }
  */
  
  if(digitalRead(buttonTogglePin) == LOW && !buttonToggleIsPressed)
  {
    mode = mode + 1;
    buttonToggleIsPressed = true;
  }
  else if(digitalRead(buttonTogglePin) == HIGH)
  {
    buttonToggleIsPressed = false;
  }
  

  switch(mode)
  {
    case 0:
      clearStrip();
      break;

    case 1:
      rgbChase(resetDelay, stepDelay);
      break;

    case 2:
      singleColorChase(resetDelay, stepDelay);
      break;

    case 3:
      singleColorSnake(resetDelay, stepDelay, red);
      break;
      
    case 4:
      singleColorSnake(resetDelay, stepDelay, green);
      break;

    case 5:
      singleColorSnake(resetDelay, stepDelay, blue);
      break;
      
    default:
      clearStrip();
      mode = 0;
      break;
  }
  

  
/*
  switch(resultsNum)
  {
    case 0xFFA25D: //power button
      clearStrip();
      break;

    case 0xFF6897: //numpad 0
      rgbChase(resetDelay, stepDelay);
      break;

    case 0xFF629D: // Volume up
      stepDelay = stepDelay + 1;
      break;

    case 0xFFA857: // Volume down
      if(stepDelay == 0) break;
      stepDelay = stepDelay - 1;
      break;

    case 0xFFC23D: // Right Skip
      resetDelay = resetDelay + 25;
      break;

    case 0xFF22DD: // Left skip
      if(resetDelay == 0) break;
      resetDelay = resetDelay - 25;
      break;

    default:
      break;
  }
  */
}

void clearStrip()
{
  for(int i = 0; i < strip.PixelCount(); i++)
  {
    strip.SetPixelColor(i, black);
  }
  strip.Show();
}

void rgbChase(int resetDelay, int stepDelay)
{

  for(int i = 0; i < strip.PixelCount(); i++)
  {
    strip.SetPixelColor(i, getRedGreenBlue(i));
    strip.Show();
    delay(stepDelay);
  }
  delay(resetDelay);
  clearStrip();

}

RgbColor getRedGreenBlue(uint8_t index)
{
  switch(index % 3)
  {
    case 0:
      return red;
      break;
    case 1:
      return blue;
      break;
    case 2:
      return green;
      break;
  }
}

void singleColorChase(int resetDelay, int stepDelay)
{
  for(int x = 0; x < 3; x++)
  {
    for(int i = 0; i < strip.PixelCount(); i++)
    {
      strip.SetPixelColor(i, getRedGreenBlue(x));
      strip.Show();
      delay(stepDelay);
    }
  }
  
  //delay(resetDelay);
}

void singleColorSnake(int resetDelay, int stepDelay, RgbColor color)
{
  for(int x = 0; x < strip.PixelCount() + 3; x++)
  {
    for(int i = 0; i < strip.PixelCount() + 3; i++)
    {
      if(i <= 120) strip.SetPixelColor(i, black);
      if(x < 120) strip.SetPixelColor(x, color);
      if(x > 0 && x < 121) strip.SetPixelColor(x-1, color);
      if(x > 1 && x < 122) strip.SetPixelColor(x-2, color);
      
    }
    delay(stepDelay);
    strip.Show();
  }
}

