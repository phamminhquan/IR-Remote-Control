# IR-Remote-Control
SAMSUNG Protocol, Power Button Only

|||
|-|-|
| Processor | ATXMEGA 128A1u |
| Microcontroller | uPAD 1.4 |
| Accessories | IR-LED, IR Sensor for testing, Tactile Switch |
| Peripherals | Timer/Counter, Overflow Interrupts |

# Setup:

  PORTC[0] = IR-LED (high-true)
  
  PORTA[0] = Tactile Switch (low-true)
  
# Instruction:

  Everytime Tactile Switch is hit, IR-LED will lights up according to the coded sequence. 
  
  In this code, the coded sequence is based on standard SAMSUNG protocol.
  
  The sequence correspond to power button (on/off).
  

Have fun.
