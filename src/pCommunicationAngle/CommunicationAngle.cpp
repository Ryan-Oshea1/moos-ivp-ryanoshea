/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle::CommunicationAngle()
{
  m_vehicle_name = 0;
  m_collaborator_name = 0;
  m_nav_x = 0;
  m_nav_y = 0;
  m_nav_depth = 0;
  m_nav_heading = 0;
  m_nav_speed = 0;
  m_c_nav_x = 0;
  m_c_nav_y = 0;
  m_c_nav_depth = 0;
  m_c_nav_heading = 0;
  m_c_nav_speed = 0;
  m_surface_sound_speed = 1480; //meter per second
  m_sound_speed_gradient = .0016; // meter per second per meter
  m_water_depth = 6000; // meters
  m_time_interval = 0;
}

//---------------------------------------------------------
// Destructor

CommunicationAngle::~CommunicationAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

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

  if( key == "VEHICLE_NAME" )  
   {
     m_vehicle_name = dval;
   }

  if( key == "COLLABORATOR_NAME" )  
   {
     m_collaborator_name = dval;
   }

  if( key == "NAV_X"  )
   {
     m_nav_x = dval;
   }

  if( key == "NAV_Y"  )
   {
     m_nav_y = dval;
   }

  if( key == "NAV_DEPTH"  )
   {
     m_nav_depth = dval;
   }


  if( key == "NAV_HEADING"  )
   {
     m_nav_heading = dval;
   }


  if( key == "NAV_SPEED"  )
   {
     m_nav_speed = dval;
   }

  if( key == "'collaborator'_NAV_X"  )
   {
     m_c_nav_x = dval;
   }

  if( key == "'collaborator'_NAV_Y"  )
   {
     m_c_nav_y = dval;
   }

  if( key == "'collaborator'_NAV_DEPTH"  )
   {
     m_c_nav_depth = dval;
   }

  if( key == "'collaborator'_NAV_HEADING"  )
   {
     m_c_nav_heading = dval;
   }

   if( key == "'collaborator'_NAV_SPEED" )  
   {
     m_c_nav_speed = dval;
   }

  }
 
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle::OnConnectToServer()
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

bool CommunicationAngle::Iterate()
{
  
  Notify("ACOUSTIC_PATH", "elev_angle= , transmission_loss= , id=oshea1@mit.edu");
  Notify("CONNECTIVITY_LOCATION", "x= , y= , depth= , id=oshea1@mit.edu");
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle::OnStartUp()
{
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

void CommunicationAngle::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("VEHICLE_NAME", 0);  
  Register("COLLABORATOR_NAME", 0);  
  Register("NAV_X", 0);  
  Register("NAV_Y", 0);  
  Register("NAV_DEPTH", 0);  
  Register("NAV_HEADING", 0);  
  Register("NAV_SPEED", 0);  
  Register("'collaborator'_NAV_X", 0);  
  Register("'collaborator'_NAV_Y", 0);  
  Register("'collaborator'_NAV_DEPTH", 0);  
  Register("'collaborator'_NAV_HEADING", 0);  
  Register("'collaborator'_NAV_SPEED", 0);  
}

