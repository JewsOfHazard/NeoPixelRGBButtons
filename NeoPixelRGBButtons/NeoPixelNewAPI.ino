

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

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

//int resultsNum;
int stepDelay = 10;
int resetDelay = 500;

int bActiveModeButtons = 9;
int bPreviousMode = 10;
int bNextMode = 11;

uint8_t mode = 0;

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

    pinMode(bActiveModeButtons, INPUT_PULLUP);
    pinMode(bNextMode, INPUT_PULLUP);
    pinMode(bPreviousMode, INPUT_PULLUP);
}

bool bNextModeOn = false;
bool bPreviousModeOn = false;

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


  //NOTE: While this button is pressed, the RGB strip will not update until it is depressed
  //TODO: Add the digital display to the mix so we can show the mode is changing (or what mode it is)
  while(digitalRead(bActiveModeButtons) == LOW) //We are holding the button that lets us use the other buttons
  {
    if(digitalRead(bNextMode) == LOW && !bNextModeOn)
    {
      mode = mode + 1;
      bNextModeOn = true;
    }
    else if(digitalRead(bNextMode) == HIGH)
    {
      bNextModeOn = false;
    }

    if(digitalRead(bPreviousMode) == LOW && !bPreviousModeOn)
    {
      if(mode > 0) mode = mode - 1;
      bNextModeOn = true;
    }
    else if(digitalRead(bPreviousMode) == HIGH)
    {
      bNextModeOn = false;
    }

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
