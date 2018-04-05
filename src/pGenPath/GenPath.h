/************************************************************/
/*    NAME: Ryan O'Shea                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" 

using namespace std;

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();
   
 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool  buildReport();
 protected:
   void RegisterVariables();

 private: // Configuration variables
   list<string> m_visit_points_str_list;
   list<string> m_visit_points_str_list_missed;
   string m_lastpoint_received;
   double m_visit_radius; //default of 5
   string m_genpath_regenerate;
   double m_nav_x;
   double m_nav_y;
   double  m_received_messages;
 private: // State variables
};

#endif 
