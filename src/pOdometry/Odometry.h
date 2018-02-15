/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Odometry_HEADER
#define Odometry_HEADER

//#include "MOOS/libMOOS/MOOSLib.h"
//changed for Appcasting
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
class Odometry : public AppCastingMOOSApp //CMOOSApp
{
 public:
   Odometry();
   ~Odometry();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   //included for Appcasting
   bool buildReport();
   //six member variables available to the Odometry Class
   bool   m_first_reading;
   double m_current_x;
   double m_current_y;
   double m_previous_x;
   double m_previous_y;
   double m_total_distance;
 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
