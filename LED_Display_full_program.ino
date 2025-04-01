// declaring an array to convert pins of the display to the corresponding pin numbers for the arduino
const int sevSegPin[] = {99, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
const int soundPin = 13;
const int passButton = A0;

char mode = 't'; // setup mode 's' and timer mode 't'.

int LEDtime = 1; // period of time each LED anode stays on for
int blinkTime = 10; // period of time that the ceter colon or the whole display blinks for.
int hysteresis = 5; // period of time to ignore new button presses after the first rising edge

// button state variable always has the current value and the previous value for rising edge detection
bool passButtonState[] = {false, false};

// the program has to be able to start new timers on each pass, and undo accidental passes. It accomplishes this 
// by keeping the start time (relative to the runtime millis()) of the timer and displaying the difference between 
// this initial value and the current runtime.

// timers[] holds the five most recent start times. In a better programming language, they would be stored in a simple
// first-in-first-out queue, but modifiying C arrays is hell, so instead, every time the timer is passed, it places
// the new start time in the next index (timerNum) in the array, looping back to index 0 once it runs out of spaces.    
unsigned long timers[] = {0, 0, 0, 0, 0};
unsigned int timerNum = 0; 
unsigned long timerLen = 30; // length of timer in sec.


unsigned long displayTimer = 1;

// cathode pins for any number are declared as three binary numbers with 9 bits, all concatenated into one number.
// structure:
//  (declare binary) 0b |  000  000  000  | 000 000 010 | 000 000 000
//    
//          cathode pin:   4-6  7-9 10-12      ditto         ditto
//            anode pin:       pin 1           pin 2         pin 3       


// define each segment of the display as a 27-bit binary number. See the spreadsheet for segment notations. 
const unsigned long segA1 = 0b000000000000000001000000000;
const unsigned long segA2 = 0b000000001000000000000000000;
const unsigned long segB2 = 0b000000010000000000000000000;
const unsigned long segB3 = 0b000000000000000000000000001;
const unsigned long segD1 = 0b000001000000000000000000000;
const unsigned long segD2 = 0b000010000000000000000000000;
const unsigned long segD3 = 0b000000000000000000000000100;
const unsigned long segD4 = 0b000000000000000000000000010;
const unsigned long segD5 = 0b000000000000000010000000000;
const unsigned long segD6 = 0b000000100000000000000000000;
const unsigned long segD7 = 0b000000000000000100000000000;
const unsigned long segE2 = 0b000000000000010000000000000;
const unsigned long segF1 = 0b000100000000000000000000000;
const unsigned long segF2 = 0b001000000000000000000000000;
const unsigned long segF3 = 0b000000000000000000000100000;
const unsigned long segF4 = 0b000000000000000000000001000;
const unsigned long segF5 = 0b000000000000001000000000000;
const unsigned long segF6 = 0b000000000000000000000010000;
const unsigned long segF7 = 0b000000000000100000000000000;
const unsigned long segH1 = 0b100000000000000000000000000;
const unsigned long segH2 = 0b000000000100000000000000000;
const unsigned long segH3 = 0b000000000000000000010000000;
const unsigned long segH4 = 0b000000000000000000001000000;
const unsigned long segH5 = 0b000000000001000000000000000;
const unsigned long segH6 = 0b010000000000000000000000000;
const unsigned long segH7 = 0b000000000010000000000000000;
const unsigned long segI1 = 0b000000000000000000100000000;


// define each displayed number as the sum of its segments. See the spreadsheet for segment notations.
const unsigned long numB1 = segB2 + segB3;

const unsigned long numD1 = segD2 + segD3;
const unsigned long numD2 = segD1 + segD2 + segD4 + segD5 + segD7;
const unsigned long numD3 = segD1 + segD2 + segD3 + segD4 + segD7;
const unsigned long numD4 = segD2 + segD3 + segD6 + segD7;
const unsigned long numD5 = segD1 + segD3 + segD4 + segD6 + segD7;
const unsigned long numD6 = segD1 + segD3 + segD4 + segD5 + segD6 + segD7;
const unsigned long numD7 = segD1 + segD2 + segD3;
const unsigned long numD8 = segD1 + segD2 + segD3 + segD4 + segD5 + segD6 + segD7;
const unsigned long numD9 = segD1 + segD2 + segD3 + segD4 + segD6 + segD7;
const unsigned long numD0 = segD1 + segD2 + segD3 + segD4 + segD5 + segD6;

const unsigned long numF1 = segF2+segF3;
const unsigned long numF2 = segF1 + segF2 + segF4 + segF5 + segF7;
const unsigned long numF3 = segF1 + segF2 + segF3 + segF4 + segF7;
const unsigned long numF4 = segF2 + segF3 + segF6 + segF7;
const unsigned long numF5 = segF1 + segF3 + segF4 + segF6 + segF7;
const unsigned long numF6 = segF1 + segF3 + segF4 + segF5 + segF6 + segF7;
const unsigned long numF7 = segF1 + segF2 + segF3;
const unsigned long numF8 = segF1 + segF2 + segF3 + segF4 + segF5 + segF6 + segF7;
const unsigned long numF9 = segF1 + segF2 + segF3 + segF4 + segF6 + segF7;
const unsigned long numF0 = segF1 + segF2 + segF3 + segF4 + segF5 + segF6;

const unsigned long numH1 = segH2+segH3;
const unsigned long numH2 = segH1 + segH2 + segH4 + segH5 + segH7;
const unsigned long numH3 = segH1 + segH2 + segH3 + segH4 + segH7;
const unsigned long numH4 = segH2 + segH3 + segH6 + segH7;
const unsigned long numH5 = segH1 + segH3 + segH4 + segH6 + segH7;
const unsigned long numH6 = segH1 + segH3 + segH4 + segH5 + segH6 + segH7;
const unsigned long numH7 = segH1 + segH2 + segH3;
const unsigned long numH8 = segH1 + segH2 + segH3 + segH4 + segH5 + segH6 + segH7;
const unsigned long numH9 = segH1 + segH2 + segH3 + segH4 + segH6 + segH7;
const unsigned long numH0 = segH1 + segH2 + segH3 + segH4 + segH5 + segH6;

const unsigned long numB[] = {0, numB1};
const unsigned long numD[] = {numD0, numD1, numD2, numD3, numD4, numD5, numD6, numD7, numD8, numD9};
const unsigned long numF[] = {numF0, numF1, numF2, numF3, numF4, numF5, numF6, numF7, numF8, numF9};
const unsigned long numH[] = {numH0, numH1, numH2, numH3, numH4, numH5, numH6, numH7, numH8, numH9};

// reset the display to a blank slate
void clear() {
  digitalWrite(sevSegPin[1], LOW);
  digitalWrite(sevSegPin[2], LOW);
  digitalWrite(sevSegPin[3], LOW);
  digitalWrite(sevSegPin[4], HIGH);
  digitalWrite(sevSegPin[5], HIGH);
  digitalWrite(sevSegPin[6], HIGH);
  digitalWrite(sevSegPin[7], HIGH);
  digitalWrite(sevSegPin[8], HIGH);
  digitalWrite(sevSegPin[9], HIGH);
  digitalWrite(sevSegPin[10], HIGH);
  digitalWrite(sevSegPin[11], HIGH);
  digitalWrite(sevSegPin[12], HIGH); 
}

// function to light the whole display according to the specified binary code.
void pulseBin(long inputBin) {
  //Serial.println(inputBin, BIN);
  // first loop operates on common anode 3. loops through the first 9 bits.
  digitalWrite(sevSegPin[3], HIGH);
  for(int i = 0; i <= 8; i++){
    // checks which pins should be on using bitwise & along with a power of 2.
    if ((inputBin & bit(i)) != 0) {
      // writes to corresponding cathode, which will start at the highest number
      // since binary powers will read right-to-left.
      digitalWrite(sevSegPin[i+4], LOW);
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(bit(i), BIN);
    }    
  }
  delay(LEDtime);
  clear();
  
  // anode 2
  digitalWrite(sevSegPin[2], HIGH);
  for(int i = 9; i <= 17; i++){
    // checks which pins should be on using bitwise & along with a power of 2.
    if ((inputBin & bit(i)) != 0) {
      // writes to corresponding cathode.
      digitalWrite(sevSegPin[i-9+4], LOW);
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(bit(i), BIN);
    }    
  }
  delay(LEDtime);
  clear();

  // anode 1
  digitalWrite(sevSegPin[1], HIGH);
  for(int i = 18; i <= 26; i++){
    // checks which pins should be on using bitwise & along with a power of 2.
    if ((inputBin & bit(i) ) != 0) {
      // writes to corresponding cathode, which will start at the highest number
      // since binary powers will read right-to-left.
      digitalWrite(sevSegPin[i-18+4], LOW);
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(bit(i), BIN);
    }    
  }
  delay(LEDtime);
  clear();
}

// function to operate one segment at a time, primarily for troubleshooting.
void pulseOneSeg(int anode, int cathode) {
    digitalWrite(sevSegPin[anode], HIGH);
    digitalWrite(sevSegPin[cathode], LOW);
    digitalWrite(sevSegPin[cathode], HIGH);
    digitalWrite(sevSegPin[anode], LOW);
    clear();
}

//function to display a looping figure-eight motion around the segments.
void displayWiggle(int i) {
  switch (i) {
    case 0: 
      pulseOneSeg(1, 7);
      break;
    case 1:
      pulseOneSeg(1, 8);
      break;
    case 2:
      pulseOneSeg(2, 6);
      break;
    case 3:
      pulseOneSeg(2, 5);
      break;
    case 4:
      pulseOneSeg(3, 5);
      break;
    case 5:
      pulseOneSeg(3, 6);
      break;
    case 6:
      pulseOneSeg(2, 6);
      break;
    case 7:
      pulseOneSeg(1, 6);
      break;
  }
}

// function to convert between an input number and the binary code used to represent the the pins to power.
unsigned long convertToDispBin(unsigned long inputInt, char dispType) {
  unsigned long output = 0;
  unsigned long minutes;
  unsigned long seconds;
  switch (dispType) {
    case 'r': // raw output. performs no place correction.
      inputInt = inputInt % 2000;
      output += numB[inputInt/1000];
      output += numD[(inputInt/100) % 10];
      output += numF[(inputInt/10) % 10];
      output += numH[(inputInt) % 10];
      return output;
      break;
    case 'm': // minute output. takes ms input.
      inputInt = (inputInt/1000) % 1200;
      minutes = inputInt / 60;
      seconds = inputInt % 60;
      output += numB[minutes / 10];
      output += numD[minutes % 10];
      output += numF[seconds / 10];
      output += numH[seconds % 10];
      output += segE2;
      return output;
      break;
    case 'b': // blinking colon output, in minutes. takes ms input.
      inputInt = (inputInt/100) % 12000;
      minutes = (inputInt/10) / 60;
      seconds = (inputInt/10) % 60;
      output += numB[minutes / 10];
      output += numD[minutes % 10];
      output += numF[seconds / 10];
      output += numH[seconds % 10];
      if ((inputInt % blinkTime) >= (blinkTime/2)) {
        output += segE2;
      }
      return output;
      break;
    case 'h': // hurry output. less than 20s, shows hundredths. takes ms input
      inputInt = (inputInt/10) % 2000;
      output += numB[inputInt/1000];
      output += numD[(inputInt/100) % 10];
      output += numF[(inputInt/10) % 10];
      output += numH[(inputInt) % 10];
      return output;
      break;
    case 'p': // panic output. less than 20s, shows hundredths. takes ms input
      inputInt = (inputInt/10) % 2000;
      if ((inputInt % blinkTime) >= (blinkTime/2)) {
        output += numB[inputInt/1000];
        output += numD[(inputInt/100) % 10];
        output += numF[(inputInt/10) % 10];
        output += numH[(inputInt) % 10];
      }
      return output;
      break;
  }
}

void pause() {
  mode = "t";
}

void updateDisplayTimer() {
  displayTimer = (timerLen*1000 - ((millis() - timers[timerNum])));
  if (displayTimer > timerLen*1000) {
    pause();
    // save new start time (reset current timer)
    timers[timerNum] = millis();
  }
}

void updateLEDs() {
  if (displayTimer >= 20000) {
    pulseBin(convertToDispBin(displayTimer, 'b'));
  }
  else if (displayTimer >= 10000) {
    pulseBin(convertToDispBin(displayTimer, 'h'));
  }
  else {
    pulseBin(convertToDispBin(displayTimer, 'p'));
  }
}

// function to check the buttons and update any logic associated with them.
void updateButtons() {
  // update the previous states before gathering new states
  passButtonState[1] = passButtonState[0];

  // updates to true and false according to analog button reading.
  if (analogRead(passButton) > 800) {
    passButtonState[0] = true;

    // check for a rising edge, unless we are within the hysteresis window and a rising edge already happened recently (within hysteresis ms).
    if ((passButtonState[1] == false) && ((millis() - timers[timerNum]) > hysteresis)) {
      // increment timer index, roll back to zero if out of range, save new start time (restart timer).
      timerNum++;
      timerNum = timerNum % 5; 
      timers[timerNum] = millis();
    }
  }
  else if (analogRead(passButton) < 200) {
    passButtonState[0] = false;
  }
}

void soundGieger() {
  if (random( 1 + pow(7, (double(displayTimer)/10000) ) ) == 0) {
    tone(soundPin, 200, 3);
  } 
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 1; i <= 12; i++) {
    pinMode(sevSegPin[i], OUTPUT);
  }

  pinMode(soundPin, OUTPUT);
}

void loop() {

  updateDisplayTimer();

  updateButtons();

  updateLEDs();

  if (mode == "t") {
    soundGieger();
  }
}
