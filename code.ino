#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TMRpcm.h>
#include <SD.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET    4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SD_ChipSelectPin 10

TMRpcm tmrpcm;
const int flexPin0 = A0;
const int flexPin1 = A1;
const int flexPin2 = A2;
const int flexPin3 = A3;
const int flexPin4 = A4;

int temp = 0;
int LangV = 0;

void setup() {
  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(5);

  Serial.begin(9600);

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
    return;
  }

  pinMode(flexPin0, INPUT);
  pinMode(flexPin1, INPUT);
  pinMode(flexPin2, INPUT);
  pinMode(flexPin3, INPUT);
  pinMode(flexPin4, INPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void loop() {
  int thumbFinger = analogRead(flexPin0);
  int pointFinger = analogRead(flexPin1);
  int midFinger = analogRead(flexPin2);
  int ringFinger = analogRead(flexPin3);
  int smallFinger = analogRead(flexPin4);

  float angle1 = map(thumbFinger, 60, 120, 0, 1);
  float angle2 = map(pointFinger, 50, 70, 0, 1);
  float angle3 = map(midFinger, 200, 350, 0, 4);
  float angle4 = map(ringFinger, 100, 250, 0, 4);
  float angle5 = map(smallFinger, 60, 100, 0, 4);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Flex Sensor 1 (A0): ");
  display.println(thumbFinger);
  display.print("Flex Sensor 2 (A1): ");
  display.println(pointFinger);

  display.print(angle1); display.print("   ");
  display.print(angle2); display.print("   ");
  display.print(thumbFinger); display.print("   ");
  display.println(pointFinger); display.print("   ");

  if (LangV == 0) {
    if (angle1 == 0 && angle2 == 0) {
      printaudioEnglish(1);
    }
    if (angle1 == 1 && angle2 == 0) {
      printaudioEnglish(2);
    }
    if (angle1 == 0 && angle2 == 1) {
      printaudioEnglish(3);
    }
    if (angle1 == 1 && angle2 == 1) {
      printaudioEnglish(4);
    }

    delay(100);
  }

  display.display();
}

void printaudioEnglish(int i) {
  if (i != temp) {
    if (i == 3) {
      display.print(" VIMUN ");
      tmrpcm.play("V.wav");
      delay(2000);
    }
    if (i == 2) {
      display.print(" ENTE PEERU");
      tmrpcm.play("M.wav");
      delay(2000);
    }
    if (i == 4) {
      display.print(" HAI NAMASKARAM ");
      tmrpcm.play("M1.wav");
      delay(2000);
    }
    temp = i;
  } else {
    // Print playback status when not playing audio
    if (!tmrpcm.isPlaying()) {
      display.print("No audio playing");
    }
  }
}
