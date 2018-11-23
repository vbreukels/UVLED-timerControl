#include <millisDelay.h>

/* This code reads serial monitor input to control the UV-LED in a simple and timed way,
 * written by Vincent Breukels (November 2018) using two sources given below. It requires the millisDelay 
 * library, a convience library written around the millis Arduino statement.
 * 
 * This code is not warranted to be fit for any purpose. You may only use it at your own risk.
 * This generated code may be freely used for both private and commercial use
 * provided this copyright is maintained, as well as the copyright of others described below.
 *  
 * 
 * For the delays involving millisDelay I used the examples in the library as basis. The copyright is:
 * 
 * (c)2018 Forward Computing and Control Pty. Ltd.
 * NSW Australia, www.forward.com.au
 * This code is not warranted to be fit for any purpose. You may only use it at your own risk.
 * This generated code may be freely used for both private and commercial use
 * provided this copyright is maintained.
 * 
 * 
 * For the serial monitor reading I used the examples found on the arduino forum:
 * 
 * http://forum.arduino.cc/index.php?topic=396450 (retreived 20 November 2018)
 * from a forum member called Robin2. No copyright statement was found in her/his examples
 */
 

int led = 13; 
// Pin 13 has an LED connected on most Arduino boards.
// if using Arduino IDE 1.5 or above you can use pre-defined
// LED_BUILTIN  instead of 'led'
//

// Unlike the examples from millisDelay, we want a variable delayTime, that can be set from monitor
unsigned long delayTime =  0; // start with 0 - means light will not turn on
unsigned long dataNumber = 0; // is the obtained time when asked at serial monitor

const unsigned long printDelayTime = 2000; // the rep time of the print timer statement in ms (2sec)

millisDelay printDelay; // the print delay object
millisDelay ledDelay;   // the delay object

// we need some more consts and variables for the reading of serial and asking the right question through serial
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
boolean askedForDelayTime = false;
boolean askedForStart = false;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  Serial.begin(9600);
  Serial.println("!! Please make sure serial monitor is on new line !!");
  Serial.println("");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

// The main loop
void loop() {
    recvWithEndMarker();
    if (delayTime == 0){
        askForDelayTime();
        showNewNumber();
      }
    else {
        askForStart();
        mainFunction();
      }
   
}

// function for correct serial reading of characters
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
            //Serial.println(sizeof(receivedChars));
        }
    }
}

// function for converting received input into ledDelay time including conversion to milliseconds
void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        if (dataNumber == 0) {
          Serial.println("Invalid time, please give positive integer value");
          
        }
        delayTime = dataNumber * 1000; //converting seconds to milliseconds
        Serial.print("Led time set at (s):  ");    // new for this version
        Serial.println(delayTime/1000);     // new for this version
        Serial.println("");
        newData = false;
        askedForDelayTime = false;
    }
}

// Function to ask for delay time - only once and only if we don't have a time yet
void askForDelayTime() {
  if (askedForDelayTime == false && delayTime == 0) {
    Serial.println("Set time in sec for turning LED on and press enter:");   
    askedForDelayTime = true; 
  }
}

// Function to ask for start - only once and only if we have a delay time
void askForStart() {
  if (askedForStart == false && delayTime != 0) {
    Serial.println("Press s to start LED irradiation - stop at any time with any character ");   
    askedForStart = true; 
  }
}

// Function for the real action
void mainFunction() {
    if (newData == true) {
      Serial.println(receivedChars);

      // Two options, s character for start, any other character to stop
      char compie[numChars];  // compie is the compare string array, with equal size for correct string comparison
      compie[0] = 's';        // s is the start character
      compie[1] = '\0';       // end character
      if (strcmp(receivedChars , compie) == 0){
        // This ensures you cannot start the timer if it is already running
        if (ledDelay.isRunning()){  
          Serial.println("LED is already on");
        }
        else {
          ledDelay.start(delayTime);
          digitalWrite(led,HIGH);
          printDelay.start(printDelayTime);
        }
      }
      // Stop LED at request - set all variables back to initial
      else {
        if (ledDelay.isRunning()){
          digitalWrite(led,LOW);
          ledDelay.stop();
          Serial.println("Stopped LED irradiation");
          Serial.println("");          
          askedForDelayTime = false;
          askedForStart = false;
          delayTime = 0;
          printDelay.stop();

        }
        // If sequence hadn't started, reset time delay and start over
        else {
          Serial.println("Abort start - LED time delay");
          Serial.println("");
          askedForDelayTime = false;
          askedForStart = false;
          delayTime = 0;
          printDelay.stop();
          
          
        }
      }
      newData = false;  
    }
    // Once ledDelay is finished, turn down led - set all variables back to initial
    if (ledDelay.isFinished()){
      digitalWrite(led,LOW);
      Serial.println("Finished LED irradiation");
      Serial.println("");
      askedForDelayTime = false;
      askedForStart = false;
      delayTime = 0;
    }
    else {
      if (printDelay.isFinished() && ledDelay.isRunning()) {
        printDelay.repeat(); // for next print
        int c = 0;
        c = ledDelay.remaining() / 1000;
        Serial.print(" Time remaining (approximately):"); 
        Serial.print(c);
        Serial.println(" s");
      }
    }
}

