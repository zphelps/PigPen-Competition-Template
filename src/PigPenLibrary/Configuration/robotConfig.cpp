#include "main.h"

/***************************************************************************
 * @brief V5 Controller Constructor
 * 
 * @return pros::Controller 
 */
pros::Controller master(pros::E_CONTROLLER_MASTER);
/***************************************************************************/

/***************************************************************************
 * @brief Odometry (Position Tracking) Robot Configuration
 * 
 * Make sure you have mounted all 3 tracking wheels on your chassis
 * Modify the constants below to match your robot's configuration
 * 
 * OFFETS:
 * The offsets for each tracking wheel is measured from the center of the 
 * wheels to the center of the robot's rotation. Note: The center of rotation
 * is not always the physical center of the robot when using omni-directional
 * wheels. 
 * 
 * IMPORTANT: 
 * These values will likely need to be changed as you tune and make
 * changes to your robot throughout the season. 
 * 
 * TROUBLESHOOTING:
 * 1. If your robot's position or angle is still not tracking properly after 
 *    measuring the physical offsets and wheel diameter, then you can modify 
 *    these values until the tracking is more accurate. 
 * 2. Not all VEX wheels have the same diameter as advertised. Many wheels are
 *    at least an 1/8 of an inch larger. Ex. 4" wheels are actually 4.125
 */
const double WHEEL_DIAMETER = 2.75;
const double LEFT_OFFSET = 5.95;
const double RIGHT_OFFSET = 5.95;
const double REAR_OFFSET = 5.75;
/***************************************************************************/
