/* This sketch uses a momentary contact E-Stop button (ASIN B07DYKQDJK) and the keyboard emulation 
capabilities of the Arduino Micro, to send keystrokes to leave a Zoom meeting.  The E-Stop button 
is set up so that actuation closes one circuit to ground (normally open, NO) and opens another 
(normally closed, NC) also to ground.  We use A0 to monitor the voltage on the NO line and A1 to 
monitor the voltage on the NC line. The values on AO are held HIGH (1023 counts) via the internal 
PULL_UP and driven LOW when the NO switch is closed. Conversely, A1 values are held LOW by the NC 
switch and pulled HIGH by the PULL_UP when the switch is opened.  

The keyboard commands are OS specific, so you'll need to set the right flag.*/

#include<Keyboard.h>

#define WINDOWS // Change this flag to match your OS, either LINUX, OSX or WINDOWS
#define NORMAL // define DEBUG for development or NORMAL for production

constexpr int threshold = 1023 / 2; // Set at half of full scale for analog inputs
int NO_raw_value = 0;               //Normally OPEN side of the E-Stop Button
int NC_raw_value = 0;               //Normally CLOSED side of the E-Stop Button
bool is_button_pressed = false;
bool keystroke_sent = false;

void check_state_and_send_shortcut(){
  if (is_button_pressed && !keystroke_sent){
    //kill screenshare in case it was open
    #ifdef OSX
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.write('S');
      delay(150);
    #endif
    #ifdef WINDOWS
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write('s');
      delay(150);
    #endif
    Keyboard.releaseAll();
    // send escape key to exit screenshare select window (if screenshare was off)
    Keyboard.write(KEY_ESC);
    delay(150);
    // send leave meeting command
    #ifdef OSX
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.write('w');
      delay(150);
    #endif
    #ifdef WINDOWS
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(KEY_F4);
      delay(150);
    #endif
    Keyboard.releaseAll();
    // press and release enter key to confirm we want to exit
    Keyboard.write(KEY_RETURN);
    keystroke_sent = true;
  }
  Keyboard.end();
}

void turn_on_light(){
  digitalWrite(13, HIGH);
}

void turn_off_light(){
  digitalWrite(13, LOW);
}

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);  //Start the Serial Monotor 
  #endif
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT_PULLUP);  //Floats HIGH when there is no contact
  pinMode(A1, INPUT_PULLUP);  //will yield 1023 counts
  Keyboard.begin();
}

void loop() {
  NO_raw_value = analogRead(A0); // Read these counts
  NC_raw_value = analogRead(A1);
  #ifdef DEBUG
    Serial.print("NO_raw value is: ");
    Serial.print(NO_raw_value);
    Serial.print(" | NC_raw value is: ");
    Serial.print(NC_raw_value);
    Serial.println();
  #endif
  if ( NO_raw_value < threshold && NC_raw_value > threshold ){
    is_button_pressed = true;
    turn_on_light();
    #ifdef DEBUG
     Serial.print("Button Pressed - - ");
    #endif
  }
  else {
    is_button_pressed = false;
    keystroke_sent = false;
    turn_off_light();
  }
  #ifdef NORMAL
    check_state_and_send_shortcut();
  #endif
}
