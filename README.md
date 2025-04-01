# DD Arduino Tabletop Game Turn Clock
### Overview
deliberator-depetrifier is an arduino project for a turn clock, intended for use with tabletop games with an arbitrary number of players. Still well in development.

### Current Features
- Multiple display functions: Can display in mm:ss format, or seconds and hundredths, with blinking variations to emphasize urgency. 
- Speaker connection: Outputs short clicks to a connected piezo transducer with increasing frequency as the timer approaches zero (eerily like a Geiger counter).

### Planned/Unfinished Features
- Pause switch: Simple switch to pause and resume the timer in the event of game disruption.
- Pass/Undo turn: A button to pass the turn to the next player, as well as an undo button for accidental passes.
- Setup Mode: Rudimentary menu capability to adjust display, timer and sound settings without needing to update code.
- Wiring diagrams/instructions
  - This project was developed using parts readily available to me, and not commonly available resources. Most notably, the LED display used was removed from a broken alarm clock, so modifications were necessary that may require more thorough explanations.
