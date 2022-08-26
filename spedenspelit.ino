#include <TM1637Display.h>

// lights
#define LYELLOW 7
#define LRED 6
#define LGREEN 5
#define LWHITE 4

// buttons
#define BYELLOW 2
#define BRED 3
#define BGREEN 8
#define BWHITE 9

//button states
int yState = 0;
int rState = 0;
int gState = 0;
int wState = 0;

// Module connection pins (Digital Pins)
#define CLK 11
#define DIO 10

TM1637Display display(CLK, DIO);

// new lights here
int lightArray[100];
// button presses here
int buttonArray[100];
int points = 0;
int pressed = 0;
int prevPressed = 0; // keep this also

float timepassed = 0;
float lightTimePassed = 0;

bool gameOn = false;
int last = 0;

int buttonIndex = 0;
int lightIndex = 0;
// when ever light is light, we add here the accumulating number
float addition = 0;
int burningLight = 0; // what light is burning
int lightIsOnRotations = 12; // how many rotations light is burning
int delayOfRound = 20; // ms

void allOn()
{
  digitalWrite(LYELLOW, HIGH);
  digitalWrite(LRED, HIGH);
  digitalWrite(LGREEN, HIGH);
  digitalWrite(LWHITE, HIGH);
}

void allOff()
{
  digitalWrite(LYELLOW, LOW);
  digitalWrite(LRED, LOW);
  digitalWrite(LGREEN, LOW);
  digitalWrite(LWHITE, LOW);
}

void setup()
{
  pinMode(LYELLOW, OUTPUT);
  pinMode(LRED, OUTPUT);
  pinMode(LGREEN, OUTPUT);
  pinMode(LWHITE, OUTPUT);

  pinMode(BYELLOW, INPUT);
  pinMode(BRED, INPUT);
  pinMode(BGREEN, INPUT);
  pinMode(BWHITE, INPUT);
  Serial.begin(9600);

  display.setBrightness(0x0f);
  for (int i = 0; i <= 2; i++) {
    allOn();
    delay(200);
    allOff();
    delay(200);
  }

  digitalWrite(LYELLOW, HIGH);
  delay(200);
  digitalWrite(LRED, HIGH);
  delay(200);
  digitalWrite(LGREEN, HIGH);
  delay(200);
  digitalWrite(LWHITE, HIGH);
  delay(200);
  allOff();
}

void loop()
{
  pressed = whatIsPressed();
  if (pressed == LYELLOW) {
    playgame(400);
    runEndGame();
  } else if (pressed == LRED) {
    playgame(500);
    runEndGame();
  } else if (pressed == LGREEN) {
    playgame(600);
    runEndGame();
  } else if (pressed == LWHITE) {
    playgame(700);
    runEndGame();
  }
  delay(50);
}

void runEndGame()
{

  for (int i = 0; i <= 2; i++) {
    allOn();
    delay(200);
    allOff();
    delay(200);
  }
  for (int i = 0; i <= 9; i++) {
    display.showNumberDec(points, false);
    delay(100);
    display.clear();
    delay(100);
  }
  display.showNumberDec(points, false);
}

/**
   float howHard - bigger the easier this is ms
*/
void playgame(float howHard)
{
  delay(1000);
  points = 0;
  timepassed = 0;
  lightTimePassed = 0;
  gameOn = true;

  last = 0;
  randomLightArray(100, last);
  // keep track o clicks here
  memset(buttonArray, 0, sizeof(buttonArray));
  memcpy( buttonArray, lightArray, sizeof(buttonArray));
  buttonIndex = 0;
  lightIndex = 0;
  // when ever light is light, we add here the accumulating number
  lightTimePassed = 0;
  addition = 0;
  burningLight = 4; // what light is burning
  display.showNumberDec(points, false);

  do {
    delay(delayOfRound);
    timepassed += delayOfRound;
    // button listener
    pressed = whatIsPressed();
    if (prevPressed != pressed && pressed > 0) {
      Serial.println("-------------");
      Serial.println(pressed);
      Serial.println(lightArray[buttonIndex]);
      Serial.println(buttonArray[buttonIndex]);
      if (pressed == buttonArray[buttonIndex]) {
        points++;
        display.showNumberDec(points, false);
        buttonIndex++;
        prevPressed = pressed;
      } else if (pressed != buttonArray[buttonIndex]) {
        gameOn = false;
      }
    }

    // -------------------------------- lights off and on
    if (lightIsOnRotations == 0) {
      digitalWrite(burningLight, LOW);
    }
    lightIsOnRotations--;

    // is it time to light up
    addition = getAddition(lightIndex, howHard);
    if (timepassed > (lightTimePassed + addition)) {
      lightTimePassed = lightTimePassed + addition;
      digitalWrite(burningLight, LOW);
      burningLight = lightArray[lightIndex];
      digitalWrite(burningLight, HIGH);
      lightIsOnRotations = 12;
      lightIndex++;
    }

    // -------------------------------- populating more lights and buttons
    // get more random items when lights have been lit
    if (lightIndex == 100) {
      last = lightArray[99];
      randomLightArray(100, last);
      lightIndex = 0;
    }
    // when 100 buttons have been pressed
    if (buttonIndex == 100) {
      memcpy( buttonArray, lightArray, sizeof(buttonArray));
      buttonIndex = 0;
    }
  } while (gameOn);
}

/**
   1000 passed light something with howHard
   lightIndex = 0 first
   timepassed 1000
   howHard is time in millis which we multiply
*/
float getAddition(int lightIndex, float howHard)
{
  // 2 - ln(index) / 2
  float addition = round((2 - log(lightIndex) / 2) * howHard);
  if (addition < howHard) {
    return howHard;
  }
  return addition;
}

int whatIsPressed()
{
  yState = digitalRead(BYELLOW);
  rState = digitalRead(BRED);
  gState = digitalRead(BGREEN);
  wState = digitalRead(BWHITE);
  if (yState == HIGH) {
    return LYELLOW;
  }
  if (rState == HIGH) {
    return LRED;
  }
  if (gState == HIGH) {
    return LGREEN;
  }
  if (wState == HIGH) {
    return LWHITE;
  }
  return 0;
}

/**
   returns an array sizeOf, where first is notThis
*/
void randomLightArray(int sizeOf, int notThis)
{
  memset(lightArray, 0, sizeof(lightArray));
  int nextLight = notThis;
  for (int i = 0; i < sizeOf; i++) {
    do {
      nextLight = random(4, 8);
    } while (nextLight == notThis);
    lightArray[i] = nextLight;
    notThis = nextLight;
  }
}
