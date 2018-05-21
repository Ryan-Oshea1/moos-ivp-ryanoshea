/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef CommunicationAngle_HEADER
#define CommunicationAngle_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" 

class CommunicationAngle : public AppCastingMOOSApp
{
 public:
   CommunicationAngle();
   ~CommunicationAngle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();
 protected:
   void RegisterVariables();
   double m_vehicle_name;
   double m_collaborator_name;
   double m_nav_x;
   double m_nav_y;
   double m_nav_depth;
   double m_nav_heading;
   double m_nav_speed;
   double m_c_nav_x;
   double m_c_nav_y;
   double m_c_nav_depth;
   double m_c_nav_heading;
   double m_c_nav_speed;
   double m_surface_sound_speed;
   double m_sound_speed_gradient;
   double m_water_depth;
   double m_time_interval;
   double m_elevation_angle;
   double m_radius_vehicle_c;
   double m_radius_vehicle_host;
   double m_z_circle;
   std::string acoustic_path_string;
   std::string connectivity_location_string ;
 private: // Configuration variables

 private: // State variables
};

#endif 
