#include "main.h"

PIDController::PIDController(double inKP, double inKI, double inKD, int inMinSpeed)
{
    kP = inKP;
    kI = inKI;
    kD = inKD;
    minSpeed = inMinSpeed;

    //Stores initial values as defaults
    DEFAULT_KP = inKP;
    DEFAULT_KI = inKI;
    DEFAULT_KD = inKD;
    DEFAULT_MINSPEED = inMinSpeed;
}

PIDController::PIDController(double inKP, int inMinSpeed)
{
    kP = inKP;
    minSpeed = inMinSpeed;

    DEFAULT_KP = inKP;
}

int PIDController::getOutput(int target, int current)
{

    double prevError = 0;

    error = (target - current);

    int integral = integral + error;

    if (error == 0)
    {
        integral = 0;
    }

    if (abs(error) > 40)
    {
        integral = 0;
    }

    int derivative = error - prevError;

    prevError = error;

    //power output calculation
    int power = (error * kP + derivative * kI + integral * kD);

    //Power defaults to our minimum speed if power generates a value < minSpeed
    if (power <= minSpeed && power <= 0 && fabs(error) < 0.5)
    {
        return power;
    }
    else if (power >= -minSpeed && power <= 0 && fabs(error) < 0.5)
    {
        return power;
    }
    else if (power <= minSpeed && power >= 0)
    {
        power = minSpeed;
    }
    else if (power >= -minSpeed && power <= 0)
    {
        power = -minSpeed;
    }
    return power;
}

void PIDController::setGains(double inKP, double inKI, double inKD, int inMinSpeed)
{
    kP = inKP;
    kI = inKI;
    kD = inKD;
    minSpeed = inMinSpeed;
}

//Reset gains to stored defaults (see constructor)
void PIDController::resetGainsToDefaults()
{
    kP = DEFAULT_KP;
    kI = DEFAULT_KI;
    kD = DEFAULT_KD;
    minSpeed = DEFAULT_MINSPEED;
}

//Feedback
bool PIDController::gainsAreAtDefaults()
{
    if (kP == DEFAULT_KP && kI == DEFAULT_KI && kD == DEFAULT_KD && minSpeed == DEFAULT_MINSPEED)
    {
        return true;
    }
    return false;
}

//Feedback
double PIDController::getError()
{
    return error;
}
