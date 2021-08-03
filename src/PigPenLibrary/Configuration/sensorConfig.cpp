#include "main.h"

/***************************************************************************
 * @brief Odometry (Position Tracking) 3-Wire Encoder Configuration
 * 
 * Make sure you have mounted all 3 tracking wheels on your chassis
 * Modify the ADIEncoder constructors below to match your robot's wiring
 * 
 * TROUBLESHOOTING:
 * 1. If your position tracking values are not correct, one or more of 
 *    your encoders is likely reversed. To fix this, flip the position
 *    of the wires on each encoder that is not working properly. 
 *    Alternatively, change the third parameter on the encoder not 
 *    working properly from false to true. 
 * 2. If an encoder value is not registering (i.e. the value is stuck at 0),
 *    the encoder is either broken or the encoder's two wires are not 
 *    plugged into adjacent ports. Ex. 'A', 'C' should be 'A', 'B'
 * 3. If you are getting a syntax error on your port assignments, make sure
 *    you are using '' instead of "".
 */

pros::ADIEncoder R('E', 'F', false); /* Right Side Encoder */
pros::ADIEncoder L('C', 'D', false); /* Left Side Encoder */
pros::ADIEncoder S('A', 'B', false); /* Back Side Encoder */
/***************************************************************************/

/***************************************************************************
 * @brief Autonomous Selector Limit Switch Constructors
 * 
 * Make sure you have mounted both limit switches on your robot
 * Modify the ADIDigitalIn constructors below to match your robot's wiring
 */
pros::ADIDigitalIn forwardSelectorLimit({21, 'C'});
pros::ADIDigitalIn backwardSelectorLimit({21, 'D'});
/***************************************************************************/
