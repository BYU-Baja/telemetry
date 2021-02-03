#include "SdCard.h"
#include <stdio.h>
#include <iostream>
#using namespace std;

#include <SPI.h>
#include <SD.h>

File myFile;

//from: https://www.arduino.cc/en/Tutorial/LibraryExamples/ReadWrite
void setup(){
    // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void update(){
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
}

File * openFile(string filename){
    if(!SD.exists(filename)){
        SD.mkdir(filename);
    }
    myFile = SD.open(filename, FILE_WRITE);
    File * fileptr = &myFile;
    return fileptr;
}

void write(FILE * fileptr){
    *fileptr.write("here is data"); //what to write? takes data or buf, len
}

void read(FILE * fileptr){
    *fileptr.read();
}

void close(FILE * fileptr){
    *fileptr.close();
}
