#include "main.h"

void red_auton()
{
}

void blue_auton()
{
}

/**
 * @brief This method will run during the autonomous period
 * 
 * NOTE: It is best to call methods with each of your autonomous 
 *       programs rather than putting your code directly in this 
 *       method. If you are using the autonomous selector, then 
 *       include the switch statement in this method. 
 */
void autonomous()
{
    /**
     * @brief The cases correspond the values in autoNames[], found in initialize.cpp
     * 
     */
    switch (autonIndex)
    {
    case 0:
    {
        red_auton();
        break;
    }
    case 1:
    {
        blue_auton();
        break;
    }
    }
}