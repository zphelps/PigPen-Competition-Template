#include "main.h"

/***************************************************************************
 * @brief Robot Chassis Motor Configuration
 * 
 * Make sure you have mounted 4 drive motors onto your robot's chassis
 * Modify the motor constructors below to match your motor configuration
 * 
 * NOTE: This configuration only works for tank drives. Additional
 *       modifications would need to be made for mecanum and x-drives. 
 * 
 * PARAMETERS:
 * There are 4 parameters for each motor constructor:
 * 1. The motor's port assignment (1-21)
 * 2. The motor's gearset:
 *      - pros::E_MOTOR_GEARSET_06: 600 RPM, blue cartridge
 *      - pros::E_MOTOR_GEARSET_18: 200 RPM, green cartridge
 *      - pros::E_MOTOR_GEARSET_36: 100 RPM, red cartridge
 * 3. Motor direction: You can reverse the direction the motor spins by 
 *    changing the boolean from true to false. 
 * 4. Motor position units: Do not modify this parameter. This library 
 *    requires all motors to use this configuration. 
 * 
 * IMPORTANT: 
 * These values will likely need to be changed as you tune and make
 * changes to your robot throughout the season. 
 * 
 * TROUBLESHOOTING:
 * 1. If one or more of your drive motors are not moving the correct direction, 
 *    try changing reversing the direction of that motor using the third 
 *    parameter boolean. 
 */
pros::Motor leftFront(9, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor leftBack(2, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightFront(5, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rightBack(1, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
/***************************************************************************/

/***************************************************************************
 * @brief Chassis Default PID Controller Constructors
 * 
 * IMPORTANT:
 * These PIDController constructors are the default controllers that will
 * be used for all robot movements. It is recommended that you tune these 
 * values through trial/error to get the best results. It is expected that 
 * these values will not work perfectly for every movement in your 
 * autonomous or programming skills, so you can you the override methods in
 * the drive class to adjust the gains for specific movements. Do not alter
 * these gains to improve the consistency of an individual movement. 
 */
PIDController movePID(0.15, 0, 0, 15);
PIDController turnPID(1.25, 0, 0, 15);
PIDController sweepTurnWithThreshholdPID(1.75, 0, 0, 80);
/***************************************************************************/

Drive drive;
MoveTargets moveTargets;
TurnTargets turnTargets;

/* Move and Turn Tasks */
pros::Task *move_task = nullptr;
pros::Task *turn_task = nullptr;

/* Acceleration Step Variables */
int rightSideSpeed = 0;
int leftSideSpeed = 0;

// //Drive PIDControllers
// PIDController movePID(0.15, 0, 0, 15);
// PIDController turnPID(1.25, 0, 0, 15);

bool moveComplete = false;
bool turnComplete = false;

double correctionMultiplier = 0.2;

//Move task helper methods
void Drive::moveStartTask()
{
    move_task = new pros::Task(moveTask);
}

void Drive::moveStopTask()
{
    if (move_task != nullptr)
    {
        move_task->remove();
        delete move_task;
        move_task = nullptr;
    }
}

//turn task helper methods
void Drive::turnStartTask()
{
    turn_task = new pros::Task(turnTask);
}

void Drive::turnStopTask()
{
    if (turn_task != nullptr)
    {
        turn_task->remove();
        delete turn_task;
        turn_task = nullptr;
    }
}

void Drive::left(int l)
{
    leftFront.move(l);
    leftBack.move(l);
}

void Drive::right(int r)
{
    rightFront.move(r);
    rightBack.move(r);
}

void Drive::slewRight(int speed, int accelStep)
{
    if (speed > rightSideSpeed)
    {
        rightSideSpeed++;
        right(rightSideSpeed);
    }
    else
    {
        right(speed);
    }
    wait(accelStep);
}

void Drive::slewLeft(int speed, int accelStep)
{
    if (speed > leftSideSpeed)
    {
        leftSideSpeed++;
        left(leftSideSpeed);
    }
    else
    {
        left(speed);
    }
    wait(accelStep);
}

void Drive::slewRightBack(int speed, int accelStep)
{
    if (speed < rightSideSpeed)
    {
        rightSideSpeed--;
        right(rightSideSpeed);
    }
    else
    {
        right(speed);
    }
    wait(accelStep);
}

void Drive::slewLeftBack(int speed, int accelStep)
{
    if (speed < leftSideSpeed)
    {
        leftSideSpeed--;
        left(leftSideSpeed);
    }
    else
    {
        left(speed);
    }
    wait(accelStep);
}

void Drive::drivePower(int l, int r)
{
    leftFront.move(l);
    leftBack.move(l);
    rightFront.move(r);
    rightBack.move(r);
}

void Drive::timedDrive(int time, int l, int r)
{
    drivePower(l, r);
    wait(time);
    drivePower(0, 0);
}

void Drive::brake()
{
    //set motor brake modes to HOLD
    rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void Drive::brakeLeftSide()
{
    //set motor brake modes to HOLD
    leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void Drive::brakeRightSide()
{
    //set motor brake modes to HOLD
    rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

void Drive::coast()
{
    //set motor brake modes to COAST
    rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

/********************************************************************
 * @brief Chassis Operator Control Code
 */
void Drive::driveOP()
{
    right(master.get_analog(ANALOG_RIGHT_Y));
    left(master.get_analog(ANALOG_LEFT_Y));
}
/*******************************************************************/

/**
 * @brief Helper method that maintains the drive's target heading
 * 
 * @param heading 
 * @param correctionMultiplier 
 * @param PIDSpeed 
 * @param accelStep 
 * @param backward 
 */
void Drive::moveHeadingCorrection(int heading, double correctionMultiplier, double PIDSpeed, int accelStep, bool backward)
{
    if (backward == false)
    {
        if ((heading - getTheta() >= 3 || heading - getTheta() <= -3))
        {
            if (getTheta() < heading)
            {
                left(PIDSpeed);
                right(correctionMultiplier * PIDSpeed);
            }
            if (getTheta() > heading)
            {
                left(correctionMultiplier * PIDSpeed);
                right(PIDSpeed);
            }
        }
        else
        {
            if (getTheta() < heading)
            {
                left(PIDSpeed);
                right(PIDSpeed * 0.6);
            }
            if (getTheta() > heading)
            {
                left(PIDSpeed * 0.6);
                right(PIDSpeed);
            }
        }
    }
    else if (backward)
    {
        if ((heading - getTheta() >= 5 || heading - getTheta() <= -5))
        {
            if (getTheta() < heading)
            {
                left(correctionMultiplier * PIDSpeed);
                right(PIDSpeed);
            }
            if (getTheta() > heading)
            {
                left(PIDSpeed);
                right(correctionMultiplier * PIDSpeed);
            }
        }
        else
        {
            if (getTheta() < heading)
            {
                left(PIDSpeed * 0.6);
                right(PIDSpeed);
            }
            if (getTheta() > heading)
            {
                left(PIDSpeed);
                right(PIDSpeed * 0.6);
            }
        }
    }
}

/**
 * @brief Temporarily change correctionMultiplier on any movement in moveTask()
 * 
 * @param cM 
 * @return Drive& 
 */
Drive &Drive::withCorrection(double cM)
{
    correctionMultiplier = cM;

    return *this;
}

/**
 * @brief Temporarily change the gains on any movement in moveTask()
 * 
 * @param kP 
 * @param kI 
 * @param kD 
 * @param minSpeed 
 * @return Drive& 
 */
Drive &Drive::withGains(double kP, double kI, double kD, int minSpeed)
{
    movePID.setGains(kP, kI, kD, minSpeed);
    return *this;
}

/**
 * @brief Temporarily change the gains on any turn in turnTask()
 * 
 * @param kP 
 * @param kI 
 * @param kD 
 * @param minSpeed 
 * @return Drive& 
 */
Drive &Drive::withTurnGains(double kP, double kI, double kD, int minSpeed)
{
    turnPID.setGains(kP, kI, kD, minSpeed);
    return *this;
}

/**
 * @brief drive for distance in inches
 * 
 * @param distance 
 * @param heading 
 * @param accelStep 
 * @param async 
 * @param fluid 
 * @return Drive& 
 */
Drive &Drive::move(int distance, int heading, int accelStep, bool async, bool fluid)
{
    if (!moveComplete)
    {
        moveStopTask();
    }
    if (move_task != nullptr)
    {
        move_task = nullptr;
    }
    //set move targets
    moveTargets = {distance, heading, accelStep, fluid, MOVE_FOR_DISTANCE};
    moveComplete = false;
    if (async)
    {
        //call as task
        moveStartTask();
    }
    else
    {
        //call as method (syncronous)
        moveTask(nullptr);
    }

    return *this;
}

/**
 * @brief move forward to Y coordinate
 * 
 * @param distance 
 * @param heading 
 * @param accelStep 
 * @param async 
 * @param fluid 
 * @return Drive& 
 */
Drive &Drive::moveToYCoord(int distance, int heading, int accelStep, bool async, bool fluid)
{
    if (!moveComplete)
    {
        moveStopTask();
    }
    if (move_task != nullptr)
    {
        move_task = nullptr;
    }
    //set move targets
    moveTargets = {distance, heading, accelStep, fluid, MOVE_TO_Y_COORD};
    moveComplete = false;
    if (async)
    {
        moveStartTask();
    }
    else
    {
        moveTask(nullptr);
    }

    return *this;
}

/**
 * @brief move forward to X coordinate
 * 
 * @param distance 
 * @param heading 
 * @param accelStep 
 * @param async 
 * @param fluid 
 * @return Drive& 
 */
Drive &Drive::moveToXCoord(int distance, int heading, int accelStep, bool async, bool fluid)
{
    if (!moveComplete)
    {
        moveStopTask();
    }
    if (move_task != nullptr)
    {
        move_task = nullptr;
    }
    //set move targets
    moveTargets = {distance, heading, accelStep, fluid, MOVE_TO_X_COORD};
    moveComplete = false;
    if (async)
    {
        moveStartTask();
    }
    else
    {
        moveTask(nullptr);
    }

    return *this;
}

/**
 * @brief move back to Y coordinate
 * 
 * @param distance 
 * @param heading 
 * @param accelStep 
 * @param async 
 * @param fluid 
 * @return Drive& 
 */
Drive &Drive::moveBackToYCoord(int distance, int heading, int accelStep, bool async, bool fluid)
{
    if (!moveComplete)
    {
        moveStopTask();
    }
    if (move_task != nullptr)
    {
        move_task = nullptr;
    }
    //set move targets
    moveTargets = {distance, heading, accelStep, fluid, MOVE_BACK_TO_Y_COORD};
    moveComplete = false;
    if (async)
    {
        moveStartTask();
    }
    else
    {
        moveTask(nullptr);
    }

    return *this;
}

/**
 * @brief move back to X coordinate
 * 
 * @param distance 
 * @param heading 
 * @param accelStep 
 * @param async 
 * @param fluid 
 * @return Drive& 
 */
Drive &Drive::moveBackToXCoord(int distance, int heading, int accelStep, bool async, bool fluid)
{
    if (!moveComplete)
    {
        moveStopTask();
    }
    if (move_task != nullptr)
    {
        move_task = nullptr;
    }
    //set move targets
    moveTargets = {distance, heading, accelStep, fluid, MOVE_BACK_TO_X_COORD};
    moveComplete = false;
    if (async)
    {
        moveStartTask();
    }
    else
    {
        moveTask(nullptr);
    }

    return *this;
}

void Drive::moveTask(void *parameter)
{
    switch (moveTargets.moveType)
    {
    case MOVE_FOR_DISTANCE:
    {
        if (moveTargets.fluid && movePID.gainsAreAtDefaults())
        {
            movePID.setGains(0.15, 0, 0, 75);
        }

        if (moveTargets.targetDistance < 0)
        {
            //Calculate target in inches
            double target = (L.get_value()) - TICS_PER_REVOLUTION * (abs(moveTargets.targetDistance) / (WHEEL_DIAMETER * PI));

            while (L.get_value() > target)
            {
                double PIDSpeed = (movePID.getOutput(target, L.get_value()));
                //Move straight at heading
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, true);
            }
        }
        else
        {
            double target = (L.get_value()) + TICS_PER_REVOLUTION * (abs(moveTargets.targetDistance) / (WHEEL_DIAMETER * PI));

            while (L.get_value() < target)
            {
                double PIDSpeed = movePID.getOutput(target, L.get_value());
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, false);
            }
        }

        if (!moveTargets.fluid)
        {
            drivePower(0, 0);
        }

        //set all modifiers back to defaults
        leftSideSpeed = 0;
        rightSideSpeed = 0;
        correctionMultiplier = 0.2;
        movePID.resetGainsToDefaults();
        moveComplete = true;
        moveStopTask();
        break;
    }
    case MOVE_TO_X_COORD:
    {
        if (moveTargets.fluid && movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 75);
        }
        else if (movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 30);
        }

        double target = moveTargets.targetDistance;

        if (getX() < target)
        {
            while (getX() < target)
            {
                double PIDSpeed = (movePID.getOutput(target, getX()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, false);
            }
        }
        else if (getX() > target)
        {
            while (getX() > target)
            {
                double PIDSpeed = (-movePID.getOutput(target, getX()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, false);
            }
        }
        if (!moveTargets.fluid)
        {
            drivePower(0, 0);
        }

        //set all modifiers back to defaults
        leftSideSpeed = 0;
        rightSideSpeed = 0;
        correctionMultiplier = 0.2;
        movePID.resetGainsToDefaults();
        moveComplete = true;
        moveStopTask();

        break;
    }
    case MOVE_BACK_TO_X_COORD:
    {
        if (moveTargets.fluid && movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 75);
        }
        else if (movePID.gainsAreAtDefaults())
        {
            movePID.setGains(5, 0, 0, 30);
        }

        double target = moveTargets.targetDistance;

        if (getX() < target)
        {
            while (getX() < target)
            {
                double PIDSpeed = (-movePID.getOutput(target, getX()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, true);
            }
        }
        else if (getX() > target)
        {
            while (getX() > target)
            {
                double PIDSpeed = (movePID.getOutput(target, getX()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, true);
            }
        }
        if (!moveTargets.fluid)
        {
            drivePower(0, 0);
        }

        //set all modifiers back to defaults
        leftSideSpeed = 0;
        rightSideSpeed = 0;
        correctionMultiplier = 0.2;
        movePID.resetGainsToDefaults();
        moveComplete = true;
        moveStopTask();

        break;
    }
    case MOVE_TO_Y_COORD:
    {
        if (moveTargets.fluid && movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 75);
        }
        else if (movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 30);
        }

        double target = moveTargets.targetDistance;

        if (getY() < target)
        {
            while (getY() < target)
            {
                double PIDSpeed = (movePID.getOutput(target, getY()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, false);
            }
        }
        else if (getY() > target)
        {
            while (getY() > target)
            {
                double PIDSpeed = (-movePID.getOutput(target, getY()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, false);
            }
        }
        if (!moveTargets.fluid)
        {
            drivePower(0, 0);
        }

        //set all modifiers back to defaults
        leftSideSpeed = 0;
        rightSideSpeed = 0;
        correctionMultiplier = 0.2;
        movePID.resetGainsToDefaults();
        moveComplete = true;
        moveStopTask();

        break;
    }
    case MOVE_BACK_TO_Y_COORD:
    {
        if (moveTargets.fluid && movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 75);
        }
        else if (movePID.gainsAreAtDefaults())
        {
            movePID.setGains(6, 0, 0, 30);
        }

        double target = moveTargets.targetDistance;

        if (getY() < target)
        {
            while (getY() < target)
            {
                double PIDSpeed = (-movePID.getOutput(target, getY()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, true);
            }
        }
        else if (getY() > target)
        {
            while (getY() > target)
            {
                double PIDSpeed = (movePID.getOutput(target, getY()));
                moveHeadingCorrection(moveTargets.targetHeading, correctionMultiplier, PIDSpeed, moveTargets.accelStep, true);
            }
        }
        if (!moveTargets.fluid)
        {
            drivePower(0, 0);
        }

        //set all modifiers back to defaults
        leftSideSpeed = 0;
        rightSideSpeed = 0;
        correctionMultiplier = 0.2;
        movePID.resetGainsToDefaults();
        moveComplete = true;
        moveStopTask();

        break;
    }
    }
}

//******************************************************************************
//**************************Turn Functions**************************************

/**
 * @brief turn for degrees
 * 
 * @param degrees 
 * @return Drive& 
 */
Drive &Drive::turn(int degrees)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, 0, 0, 0, TURN};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep right
 * 
 * @param degrees 
 * @param rightSideSpeed 
 * @return Drive& 
 */
Drive &Drive::sweepRight(int degrees, int rightSideSpeed)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, 0, rightSideSpeed, 0, SWEEP_RIGHT};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep right fluidly into another movement
 * 
 * @param degrees 
 * @param rightSideSpeed 
 * @param errorThreshhold 
 * @return Drive& 
 */
Drive &Drive::sweepRight(int degrees, int rightSideSpeed, int errorThreshhold)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, 0, rightSideSpeed, errorThreshhold, SWEEP_RIGHT_WITH_THRESHHOLD};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep left
 * 
 * @param degrees 
 * @param leftSideSpeed 
 * @return Drive& 
 */
Drive &Drive::sweepLeft(int degrees, int leftSideSpeed)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, leftSideSpeed, 0, 0, SWEEP_LEFT};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep left fluidly into another movement
 * 
 * @param degrees 
 * @param leftSideSpeed 
 * @param errorThreshhold 
 * @return Drive& 
 */
Drive &Drive::sweepLeft(int degrees, int leftSideSpeed, int errorThreshhold)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, leftSideSpeed, 0, errorThreshhold, SWEEP_LEFT_WITH_THRESHHOLD};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep right backwards
 * 
 * @param degrees 
 * @param leftSideSpeed 
 * @return Drive& 
 */
Drive &Drive::sweepRightBack(int degrees, int leftSideSpeed)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, leftSideSpeed, 0, 0, SWEEP_RIGHT_BACK_WITH_THRESHHOLD};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep right backwards fluidly
 * 
 * @param degrees 
 * @param leftSideSpeed 
 * @param errorThreshhold 
 * @return Drive& 
 */
Drive &Drive::sweepRightBack(int degrees, int leftSideSpeed, int errorThreshhold)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, leftSideSpeed, 0, errorThreshhold, SWEEP_RIGHT_BACK_WITH_THRESHHOLD};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep left backwards fluidly
 * 
 * @param degrees 
 * @param rightSideSpeed 
 * @return Drive& 
 */
Drive &Drive::sweepLeftBack(int degrees, int rightSideSpeed)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, 0, rightSideSpeed, 0, SWEEP_LEFT_BACK};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

/**
 * @brief sweep left backwards fluidly
 * 
 * @param degrees 
 * @param rightSideSpeed 
 * @param errorThreshhold 
 * @return Drive& 
 */
Drive &Drive::sweepLeftBack(int degrees, int rightSideSpeed, int errorThreshhold)
{
    if (!turnComplete)
    {
        moveStopTask();
    }
    if (turn_task != nullptr)
    {
        turn_task = nullptr;
    }
    turnTargets = {degrees, 0, rightSideSpeed, errorThreshhold, SWEEP_LEFT_BACK_WITH_THRESHHOLD};
    turnComplete = false;

    turnTask(nullptr);

    return *this;
}

void Drive::turnTask(void *parameter)
{
    switch (turnTargets.turnType)
    {
    case TURN:
    {
        int time = 0;

        while (time < 50) //50
        {
            double PIDSpeed = turnPID.getOutput(turnTargets.degrees, getTheta());
            drivePower(PIDSpeed, -PIDSpeed);

            if (fabs(turnPID.getError()) < 2.5)
            {
                time++;
                wait(2);
            }
        }
        drivePower(0, 0);
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_RIGHT:
    {
        int time = 0;
        while (time < 25)
        {
            drivePower(turnPID.getOutput(turnTargets.degrees, getTheta()), turnTargets.rightSideSpeed);

            if (abs(turnPID.getError()) < 2.5)
            {
                time++;
                wait(2);
            }
            wait(5);
        }
        drivePower(0, 0);
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_RIGHT_WITH_THRESHHOLD:
    {
        // if (turnPID.gainsAreAtDefaults())
        // {
        //     turnPID.setGains(1.75, 0, 0, 80);
        // }

        while (getTheta() < turnTargets.degrees - turnTargets.errorThreshhold)
        {
            drivePower(sweepTurnWithThreshholdPID.getOutput(turnTargets.degrees, getTheta()), turnTargets.rightSideSpeed);
        }
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_LEFT:
    {
        int time = 0;
        while (time < 25)
        {
            drivePower(turnTargets.leftSideSpeed, -turnPID.getOutput(turnTargets.degrees, getTheta()));

            if (abs(turnPID.getError()) < 2.5)
            {
                time++;
                wait(2);
            }
            wait(5);
        }
        drivePower(0, 0);
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_LEFT_WITH_THRESHHOLD:
    {
        // if (turnPID.gainsAreAtDefaults())
        // {
        //     turnPID.setGains(1.75, 0, 0, 80);
        // }
        while (getTheta() > turnTargets.degrees + turnTargets.errorThreshhold)
        {
            drivePower(turnTargets.leftSideSpeed, -sweepTurnWithThreshholdPID.getOutput(turnTargets.degrees, getTheta()));
        }
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_RIGHT_BACK:
    {
        int time = 0;
        while (time < 25)
        {
            drivePower(turnTargets.leftSideSpeed, -turnPID.getOutput(turnTargets.degrees, getTheta()));

            if (abs(turnPID.getError()) < 2.5)
            {
                time++;
                wait(2);
            }
            wait(5);
        }
        drivePower(0, 0);
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_RIGHT_BACK_WITH_THRESHHOLD:
    {
        // if (turnPID.gainsAreAtDefaults())
        // {
        //     turnPID.setGains(1.75, 0, 0, 80);
        // }
        while (getTheta() < turnTargets.degrees - turnTargets.errorThreshhold)
        {
            drivePower(turnTargets.leftSideSpeed, -sweepTurnWithThreshholdPID.getOutput(turnTargets.degrees, getTheta()));
        }
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_LEFT_BACK:
    {
        int time = 0;
        while (time < 25)
        {
            drivePower(turnPID.getOutput(turnTargets.degrees, getTheta()), turnTargets.rightSideSpeed);
            if (abs(turnPID.getError()) < 2.5)
            {
                time++;
                wait(2);
            }
            wait(5);
        }
        drivePower(0, 0);
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    case SWEEP_LEFT_BACK_WITH_THRESHHOLD:
    {
        // if (turnPID.gainsAreAtDefaults())
        // {
        //     turnPID.setGains(1.75, 0, 0, 80);
        // }
        while (getTheta() > turnTargets.degrees + turnTargets.errorThreshhold)
        {
            drivePower(sweepTurnWithThreshholdPID.getOutput(turnTargets.degrees, getTheta()), turnTargets.rightSideSpeed);
        }
        turnPID.resetGainsToDefaults();
        turnComplete = true;
        turnStopTask();
        break;
    }
    }
}

//******************************************************************************
//*************************Sweep Functions**************************************
/**
 * @brief helper method to block code until asynchronous movements are complete
 * 
 */
void Drive::waitForComplete()
{
    while (!moveComplete)
        wait(1);
}