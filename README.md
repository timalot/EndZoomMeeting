# EndZoomMeeting
Arduino Micro sketch for a physical E-Stop button to leave your Zoom meeting as seen on Reddit

This sketch uses a momentary contact E-Stop button (ASIN B07DYKQDJK) and the keyboard emulation 
capabilities of the Arduino Micro, to send keystrokes to leave a Zoom meeting.  The E-Stop button 
is set up so that actuation closes one circuit to ground (normally open, NO) and opens another 
(normally closed, NC) also to ground.  We use A0 to monitor the voltage on the NO line and A1 to 
monitor the voltage on the NC line. The values on AO are held HIGH (1023 counts) via the internal 
PULL_UP and driven LOW when the NO switch is closed. Conversely, A1 values are held LOW by the NC 
switch and pulled HIGH by the PULL_UP when the switch is opened.  

The keyboard commands are OS specific, so you'll need to set the right flag.
