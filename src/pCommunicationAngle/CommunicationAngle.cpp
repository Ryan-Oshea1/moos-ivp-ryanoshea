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

  if( key == "collaborator_NAV_X"  )
   {
     m_c_nav_x = dval;
   }

  if( key == "collaborator_NAV_Y"  )
   {
     m_c_nav_y = dval;
   }

  if( key == "collaborator_NAV_DEPTH"  )
   {
     m_c_nav_depth = dval;
   }

  if( key == "collaborator_NAV_HEADING"  )
   {
     m_c_nav_heading = dval;
   }

   if( key == "collaborator_NAV_SPEED" )  
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

  //  m_surface_sound_speed = 1480; //meter per second                                                                             
  // m_sound_speed_gradient = .0016; // meter per second per meter  
  uint64_t f_launch_angle = 1; //deg to rad
  double f_local_angle = f_launch_angle; 
  double f_sound_speed_depth = m_surface_sound_speed + m_sound_speed_gradient * m_nav_depth;
  double f_R = f_sound_speed_depth /(m_sound_speed_gradient * cos(f_launch_angle));
  double f_s = f_R * (f_launch_angle - f_local_angle) ;       
  //  double f_r_s = f_R * [sin(f_launch_angle) + sin(f_s/f_R - f_launch_angle)];
  //double f_z_s = f_R * cos(f_s/f_R) - m_surface_sound_speed/m_sound_speed_gradient;
  double f_distance_between_2_D = pow((m_nav_x - m_c_nav_x),2) + pow((m_nav_y - m_c_nav_y),2);
  //  Notify("PATH_EXISTS", "iterate");
  //double i = 1;
  for(double i=1;i<90; i++)
	{
	  //      f_s = f_R * (f_launch_angle - f_local_angle) ;                                                                   
	  //f_r_s = f_R * [sin(f_launch_angle) + sin(f_s/f_R - f_launch_angle)];                                                   
	  //f_z_s = f_R * cos(f_s/f_R) - m_surface_sound_speed/m_sound_speed_gradient;                                           
	  //  Notify("PATH_EXISTS", "in for loop");
  
	  f_R = f_sound_speed_depth /(m_sound_speed_gradient * cos(f_launch_angle));
	  f_launch_angle = f_launch_angle + i;
	  // if the collaborator is above you (less depth) or on the same plane
	  if(m_nav_depth >= m_c_nav_depth )
	    {
	      //		  Notify("PATH_EXISTS", "in above loop");
	      //if the difference between is less than 5 meters
	      if( (f_R*2 - f_distance_between_2_D) < 5 ) 
		{
		  Notify("PATH_EXISTS", "true above");
		}
	    }
	  // if the collaborator is below you 
	if(m_nav_depth < m_c_nav_depth )
	  {
	    //		  Notify("PATH_EXISTS", "in below loop");	    
	    double f_x_difference_2_D = m_c_nav_x - m_nav_x;
	    double f_y_difference_2_D = m_c_nav_y - m_nav_x;
	    
	    double f_x_unit_vector = f_x_difference_2_D/f_distance_between_2_D;
	    double f_y_unit_vector = f_y_difference_2_D/f_distance_between_2_D;
	    
	    double f_circle_center_x = m_nav_x + f_x_unit_vector*f_R;
	    double f_circle_center_y = m_nav_y + f_y_unit_vector*f_R;
	    double f_depth_difference = m_c_nav_depth - m_nav_depth;
 
	    double f_distance_from_circle_center_to_c = sqrt(pow((f_circle_center_x - m_c_nav_x),2) + pow((f_circle_center_y - m_c_nav_y),2) + pow((f_depth_difference),2));
	    Notify("DIST_DIFF",(f_R) - (f_distance_from_circle_center_to_c)); 
            Notify("F_SOUND_SPEED_DEPTH",(f_sound_speed_depth)); 

            Notify("M_SOUND_SPEED_GRADIENT",(m_sound_speed_gradient)); 
            Notify("F_LAUNCH_ANGLE",(f_launch_angle)); 
 
	    Notify("f_R",(f_R));
            Notify("center_to_center_dist",(f_distance_from_circle_center_to_c));

	    if( (f_R) - (f_distance_from_circle_center_to_c) < 5 )
	      {
		Notify("PATH_EXISTS", "true below");
	      }
	    
	  }
	}


      //  Notify("ACOUSTIC_PATH", "elev_angle= , transmission_loss= , id=oshea1@mit.edu");
  Notify("ACOUSTIC_PATH", m_c_nav_x);
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
  Register("collaborator_NAV_X", 0);  
  Register("collaborator_NAV_Y", 0);  
  Register("collaborator_NAV_DEPTH", 0);  
  Register("collaborator_NAV_HEADING", 0);  
  Register("collaborator_NAV_SPEED", 0);  
}

