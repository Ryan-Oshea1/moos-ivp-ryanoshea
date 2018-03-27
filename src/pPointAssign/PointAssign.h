/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

using namespace std;

class PointAssign : public CMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables
   string m_visit_point;
   list<string> m_visit_points_str_list;
   int m_count;
   list<string> m_vname_list;
   string m_vname_string;
   string m_vname_string_1;
   string m_vname_string_2;
 private: // State variables
};

#endif 
