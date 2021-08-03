#include "main.h"

int autonIndex = 0;

/************************************************************
 * @brief Make sure the autoCount variable is set to 
 *        the number of autonomous Strings in autoNames[]
 */
const int autoCount = 7;
const char *autoNames[autoCount] = {
    "Red Autonomous",
    "Blue Autonomous"};
/***********************************************************/

void autonSelector(void *parameter)
{
    wait(500); //Fix Bug that starts index at 2
    pros::lcd::print(6, "%s", autoNames[autonIndex]);

    while (true)
    {

        if (forwardSelectorLimit.get_value())
        {
            autonIndex = autonIndex + 1;
            if (autonIndex == autoCount)
                autonIndex = 0;

            pros::lcd::print(6, "%s", autoNames[autonIndex]);
            wait(300);
        }
        else if (backwardSelectorLimit.get_value())
        {
            autonIndex = autonIndex - 1;
            if (autonIndex == autoCount)
                autonIndex = 0;
            pros::lcd::print(6, "%s", autoNames[autonIndex]);
            wait(300);
        }
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
    /* LCD Emulator Initilization */
    pros::lcd::initialize();

    /* Initialize the Odometry (Position Tracking) Task */
    odometryStartTask();

    /* Autonomous Selector Initialization */
    pros::Task lcd_task(autonSelector);
    pros::lcd::set_text(6, "<Select an Autonomous>");
    lcd_task.set_priority(LV_TASK_PRIO_LOW); //Task infinite loop bug fix
    if (pros::competition::is_autonomous())
    {
        lcd_task.remove();
    }
}