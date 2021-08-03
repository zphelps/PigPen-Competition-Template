/**
 * @file main.h
 * @author Zach Phelps (zach@zachphelps.com)
 * @brief 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#include "api.h"
#include "okapi/api.hpp"

/****************************************************************************
 * @brief PigPen Library #include statements
 */
#include "PigPenLibrary/odometry.hpp"
#include "PigPenLibrary/Configuration/sensorConfig.hpp"
#include "PigPenLibrary/Configuration/robotConfig.hpp"
#include "PigPenLibrary/drive.hpp"
#include "PigPenLibrary/PIDController.hpp"
#include "PigPenLibrary/utilities.hpp"
/***************************************************************************/

using namespace okapi;
extern int autonIndex;

#ifdef __cplusplus
extern "C"
{
#endif
    void autonomous(void);
    void initialize(void);
    void disabled(void);
    void competition_initialize(void);
    void opcontrol(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif // _PROS_MAIN_H_
