//includes
#include "pitches.h"
#include <vector>
#include <LiquidCrystal_I2C.h>
//1 blue
//2 green
//3 red
//4 yellow

//pins
constexpr int blueSwitch = 2;
constexpr int blueLed = 3;
constexpr int greenSwitch = 4;
constexpr int greenLed = 5;
constexpr int redSwitch = 6;
constexpr int redLed = 7;
constexpr int yellowSwitch = 8;
constexpr int yellowLed = 9;
constexpr int buzzer = 10;

//stuff
std::vector<int> colors;
std::vector<int> colorsInput;
int score = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    lcd.init();
    lcd.backlight();
    lcd.clear();

    //start sequence
    lcd.setCursor(0, 0);
    lcd.print("nuclear reactor");
    lcd.setCursor(0, 1);
    lcd.print("starting...");
    delay(2000);
    lcd.clear();

    //pinmodes
    pinMode(blueSwitch, INPUT);
    pinMode(greenSwitch, INPUT);
    pinMode(redSwitch, INPUT);
    pinMode(yellowSwitch, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
}

void color_tone_do_f(auto note, int ledPin) {
    tone(buzzer, note);
    digitalWrite(ledPin, HIGH);
    delay(300);
    noTone(buzzer);
    digitalWrite(ledPin, LOW);
}

void color_tone_f(int color) {
    //1 blue
    //2 green
    //3 red
    //4 yellow
    //5 win
    //6 lose
    switch(color) {
        case 1:
            color_tone_do_f(NOTE_G3, blueLed);
            break;
        case 2:
            color_tone_do_f(NOTE_C4, greenLed);
            break;
        case 3:
            color_tone_do_f(NOTE_E4, redLed);
            break;
        case 4:
            color_tone_do_f(NOTE_G5, yellowLed);
            break;
        case 5:
            tone(buzzer, NOTE_E4);
            delay(150);
            tone(buzzer, NOTE_G4);
            delay(150);
            tone(buzzer, NOTE_E5);
            delay(150);
            tone(buzzer, NOTE_C5);
            delay(150);
            tone(buzzer, NOTE_D5);
            delay(150);
            tone(buzzer, NOTE_G5);
            delay(150);
            noTone(buzzer);
            break;
        case 6:
            tone(buzzer, NOTE_DS5);
            delay(300);
            tone(buzzer, NOTE_D5);
            delay(300);
            tone(buzzer, NOTE_CS5);
            delay(300);
            for (byte i = 0; i < 10; i++) {
                for (int pitch = -10; pitch <= 10; pitch++) {
                    tone(buzzer, NOTE_C5 + pitch);
                    delay(5);
                }
            }
            noTone(buzzer);
            break;
        default:
            //error
            while(true) {
                digitalWrite(blueLed, HIGH);
                digitalWrite(greenLed, HIGH);
                digitalWrite(redLed, HIGH);
                digitalWrite(yellowLed, HIGH);
                delay(500);
                digitalWrite(blueLed, LOW);
                digitalWrite(greenLed, LOW);
                digitalWrite(redLed, LOW);
                digitalWrite(yellowLed, LOW);
                delay(500);
            }
    }
}

void display_f() {//shows the Specified sequence of the led's
    for(auto i = colors.begin(); i != colors.end(); ++i) {
        color_tone_f(*i);
        delay(500);
    }
}

int button_f() {//reads the button inputs
    while(true) {
        if(digitalRead(blueSwitch)) {
            color_tone_f(1);
            return 1;
        }
        else if(digitalRead(greenSwitch)) {
            color_tone_f(2);
            return 2;
        }
        else if(digitalRead(redSwitch)) {
            color_tone_f(3);
            return 3;
        }
        else if(digitalRead(yellowSwitch)) {
            color_tone_f(4);
            return 4;
        }
        else {
            Serial.println("waiting for input");
            delay(50);
        }
    }
}

bool vector_comparison_f(){
    size_t minLength = std::min(colors.size(), colorsInput.size());
    for (size_t i = 0; i < minLength; ++i) {
        if (colors[i] != colorsInput[i]) {
            return false;
        }
    }
    return true;
}

void game_f() {
    int input;
    bool state;
    while(colorsInput.size() < colors.size()) {//plays the full sequenz and tests it
        input = button_f();
        colorsInput.push_back(input);
        state = vector_comparison_f();
        if(!state) {
            colorsInput.clear();
            colors.clear();
            score = 0;
            color_tone_f(6);
            return;
        }
    }
    score++;
    colorsInput.clear();
}

void loop() {
    color_tone_f(5);
    delay(1000);
    int randomNumber = random(1, 5);
    colors.push_back(randomNumber);
    display_f();
    game_f();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("score:");
    lcd.setCursor(0, 1);
    lcd.print(score);
}