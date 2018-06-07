//time keeper Written by Ruben Marc Speybrouck
//used to write motion detection to an sdcard without the need for an external RTC module

#include <SPI.h>
#include <SD.h>

unsigned long timeNow = 0;
unsigned long timeLast = 0;

//Time start Settings:

int startingHour = 12; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time

int seconds = 0;
int minutes = 51;
int hours = startingHour;
int days = 0;

//Accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  

int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

File myFile;
void setup() {
  
   pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
Serial.begin(9600);

}


void loop() {
  // put your main code here, to run repeatedly:
timeNow = millis()/1000; // the number of milliseconds that have passed since boot
seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.

  

if (seconds == 60) {
  timeLast = timeNow;
  minutes = minutes + 1;
}

//if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

if (minutes == 60){ 
  minutes = 0;
  hours = hours + 1;
}

// if one hour has passed, start counting minutes from zero and add one hour to the clock

if (hours == 24){
  hours = 0;
  days = days + 1;
  }

  //if 24 hours have passed , add one day

if (hours ==(24 - startingHour) && correctedToday == 0){
  delay(dailyErrorFast*1000);
  seconds = seconds + dailyErrorBehind;
  correctedToday = 1;
}

//every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
//Change these varialbes according to the error of your board. 
// The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
// and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

if (hours == 24 - startingHour + 2) { 
  correctedToday = 0;
}

//let the sketch know that a new day has started for what concerns correction, if this line was not here the arduiono
// would continue to correct for an entire hour that is 24 - startingHour. 

val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on

       myFile = SD.open("capture.txt", FILE_WRITE);
       if (myFile) {
        Serial.print("Writing to test.txt...");
     myFile.println("Motion detected at: ");
      myFile.print(days);
      myFile.print(":");
      myFile.print(hours);
      myFile.print(":");
      myFile.print(minutes);
      myFile.print(":"); 
      myFile.println(seconds); 
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
      Serial.println("Motion detected at: ");
      Serial.print(days);
      Serial.print(":");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(":"); 
      Serial.println(seconds); 
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }


 

}
