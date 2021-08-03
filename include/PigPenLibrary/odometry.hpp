
/*******************************************************************
 * @brief Odometry (Position Tracking) method declarations
 */
extern const int TICS_PER_REVOLUTION;

void odometryStartTask(bool reset = true);
void odometryStopTask();

void calculate_position(void *parameter);
double getX();
double getY();
double getTheta();
double getThetaRadians();
void resetOdometry();
void setTheta(int degrees);
void setCoordinates(int x, int y, int theta);
/******************************************************************/