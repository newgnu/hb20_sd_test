
//Setup TFT display
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_CS   19
#define TFT_DC   22
#define TFT_MOSI 23
#define TFT_CLK  26
#define TFT_RST  21
#define TFT_MISO 25
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

//Setup SD card
#include <mySD.h>
#define SD_CS    17
#define SD_MOSI  16
#define SD_MISO   4
#define SD_CLK    0
File root;


void setup() {
  tft.begin();
  tft.setRotation(3); //

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println("Waiting for SD init");
  tft.println("Hold 'flash' key now.");

  while (!SD.begin(SD_CS, SD_MOSI, SD_MISO, SD_CLK)){
    delay(1000);
    tft.print(".");
  }
  tft.setTextColor(ILI9341_GREEN);
  tft.println("OK");
  tft.setTextColor(ILI9341_YELLOW);
  delay(1000);


  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);

  root = SD.open("/");
  if (root) {
    printDirectory(root, 0);
    root.close();
  } else {
    tft.println("error opening root FS.. we can likely recover");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void printDirectory(File dir, int numTabs) {
  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       tft.print('\t');   // we'll have a nice indentation
     }
     // Print the name
     tft.print(entry.name());
     /* Recurse for directories, otherwise print the file size */
     if (entry.isDirectory()) {
       tft.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       /* files have sizes, directories do not */
       tft.print("\t\t");
       tft.println(entry.size());
     }
     entry.close();
   }
}
