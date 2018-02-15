/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
  bool   m_first_reading = false;
  double m_current_x = 0;
  double m_current_y = 0;
  double m_previous_x = 0;
  double m_previous_y = 0;
  double m_total_distance = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  //included for AppCasting
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    Notify("NewMailCount", NewMail.size());
    

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

            
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();

    //if the message is from the NAV_X file then we iterate x current
    if (key == "NAV_X")
		{
		  if(p==NewMail.begin())
		    {m_previous_x = m_current_x;}
		  m_current_x = dval;
		}
    

    //if the message is from the NAV_Y file then we iterate y current
    if (key == "NAV_Y")
		{
		  if(p==NewMail.begin())
		    {m_previous_y = m_current_y;}
		  m_current_y = dval;
		}

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  //added for AppCasting
  AppCastingMOOSApp::Iterate();

  //f prefix for local to function
  double f_x_difference = m_current_x - m_previous_x;
  double f_y_difference = m_current_y - m_previous_y;

  //if iterate gets called again before on new mail, then we want it to not update with more data
  // so we set the previous and current to be the same
  m_previous_x = m_current_x;
  m_previous_y = m_current_y;

  //total is equal to the total from last time plus the new euclidian distance
  m_total_distance = m_total_distance + sqrt(pow(f_x_difference,2) + pow(f_y_difference,2));
  
  //update/notify the server with Notify(varname,varval)
  Notify("ODOMETRY_DIST", m_total_distance);

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  //included for appcasting
  AppCastingMOOSApp::OnStartUp();

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  // Register("FOOBAR", 0);

  //included for AppCasting
  AppCastingMOOSApp::RegisterVariables();

  //register for NAV_X and NAV_y
   Register("NAV_X", 0);
   Register("NAV_Y", 0); //the 0 registers for all change notifications on the variable
}

bool Odometry::buildReport()
{
  m_msgs << "Total Distance Traveled: " << m_total_distance << endl;
  return(true);
}
