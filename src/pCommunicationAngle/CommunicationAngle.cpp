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
  AppCastingMOOSApp::OnNewMail(NewMail);
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
  AppCastingMOOSApp::Iterate();
  //we assume the linear profile for simplicity

  //  m_surface_sound_speed = 1480; //meter per second                                                                             
  // m_sound_speed_gradient = .0016; // meter per second per meter  
  uint64_t f_launch_angle = 1; //deg to rad
  double f_local_angle = f_launch_angle; 
  double f_sound_speed_depth = m_surface_sound_speed + m_sound_speed_gradient * m_nav_depth;
  double f_R = f_sound_speed_depth /(m_sound_speed_gradient * cos(f_launch_angle));
  double f_s = f_R * (f_launch_angle - f_local_angle) ;
  m_z_circle = (-1) * m_surface_sound_speed/m_sound_speed_gradient;
  //  double f_r_s = f_R * [sin(f_launch_angle) + sin(f_s/f_R - f_launch_angle)];
  //double f_z_s = f_R * cos(f_s/f_R) - m_surface_sound_speed/m_sound_speed_gradient;
  //sqrt of the difference in distances
  double f_distance_between_3_D = sqrt(pow((m_nav_x - m_c_nav_x),2) + pow((m_nav_y - m_c_nav_y),2) + pow((m_nav_depth-m_c_nav_depth),2)); 
  //  Notify("PATH_EXISTS", "iterate");
  //double i = 1;
  //loop through from 1 degree to 90 degreees
  // for(double i=1;i<90; i++)
  //{
	  //      f_s = f_R * (f_launch_angle - f_local_angle) ;                                                                   
	  //f_r_s = f_R * [sin(f_launch_angle) + sin(f_s/f_R - f_launch_angle)];                                                   
	  //f_z_s = f_R * cos(f_s/f_R) - m_surface_sound_speed/m_sound_speed_gradient;                                           
	  //  Notify("PATH_EXISTS", "in for loop");
  
	  //	  f_R = f_sound_speed_depth /(m_sound_speed_gradient * cos(f_launch_angle));
	  //f_launch_angle = f_launch_angle + i;
	  //Either the collaborator is above you, so you can trace a circle arc to them as in figure 4
	  // or the collaborator is below you, so they lie on the arc of the circle
	  // if the collaborator is above you (less depth) or on the same plane
	  //if(m_nav_depth >= m_c_nav_depth )
	  //{
	      //		  Notify("PATH_EXISTS", "in above loop");
	      //if the difference between is less than 5 meters
	  //  if( (f_R*2 - f_distance_between_3_D) < 5 ) 
	  //{
	  //	  Notify("PATH_EXISTS", "true above");
	  //	}
	  // }
	  // if the collaborator is below you 
	  //if(m_nav_depth < m_c_nav_depth )
	  //	  {
	    //		  Notify("PATH_EXISTS", "in below loop");
            //calculate the vectors
	    double f_x_difference_2_D = m_c_nav_x - m_nav_x;
	    double f_y_difference_2_D = m_c_nav_y - m_nav_y;
	    double f_z_difference_2_D = m_c_nav_depth - m_nav_depth;

	    double f_x_unit_vector = f_x_difference_2_D/f_distance_between_3_D;
	    double f_y_unit_vector = f_y_difference_2_D/f_distance_between_3_D;
	    double f_z_unit_vector = f_z_difference_2_D/f_distance_between_3_D;

	    //	    double f_circle_center_x = m_nav_x + f_x_unit_vector*f_R;
	    //double f_circle_center_y = m_nav_y + f_y_unit_vector*f_R;
	    
	    //find the first bisector
	    double f_vehicle_bisector_x = f_x_difference_2_D/2 + m_nav_x;
	    double f_vehicle_bisector_y = f_y_difference_2_D/2 + m_nav_y;
	    double f_vehicle_bisector_z = f_y_difference_2_D/2 + m_nav_depth;

	    //find the perpendicular line from there to the center of the circle
	    //define a new vector w that is the sum of these two vectors
	    double f_w_magnitude = sqrt(pow(f_x_difference_2_D,2) + pow(f_y_difference_2_D,2));
	    
	    //define the perpendicular vector
	    double f_second_perp_x = f_x_difference_2_D/f_w_magnitude * f_z_difference_2_D * (-1); 
	    double f_second_perp_y = f_y_difference_2_D/f_w_magnitude * f_z_difference_2_D * (-1);
	    double f_second_perp_z = f_w_magnitude;
	    
	    double f_second_perp_magnitude = sqrt(pow((f_second_perp_x),2) + pow((f_second_perp_y),2) + pow((f_second_perp_z),2));
	    double f_second_perp_z_unit = f_second_perp_z/f_second_perp_magnitude;
	    
	    double f_intersect_z_dist = (-1)* (m_z_circle - f_vehicle_bisector_z)/f_second_perp_z_unit; 
	    double f_circle_x_loc = f_second_perp_x/f_second_perp_magnitude * f_intersect_z_dist + f_vehicle_bisector_x;
	    double f_circle_y_loc = f_second_perp_y/f_second_perp_magnitude * f_intersect_z_dist + f_vehicle_bisector_y;
	    double f_circle_z_loc = m_z_circle;

	    m_radius_vehicle_host = sqrt(pow((m_nav_x - f_circle_x_loc),2) + pow((m_nav_y - f_circle_y_loc),2) + pow((m_nav_depth - f_circle_z_loc),2));
	    m_radius_vehicle_c = sqrt(pow((m_c_nav_x- f_circle_x_loc),2) + pow((m_c_nav_y - f_circle_y_loc),2) + pow((m_c_nav_depth - f_circle_z_loc),2));
	    //   double f_depth_difference = m_c_nav_depth - m_nav_depth;
 
	    //double f_distance_from_circle_center_to_c = sqrt(pow((f_circle_center_x - m_c_nav_x),2) + pow((f_circle_center_y - m_c_nav_y),2) + pow((f_depth_difference),2));
	 
	    m_elevation_angle = acos(m_surface_sound_speed/m_sound_speed_gradient/m_radius_vehicle_host);


	    //Notify("DIST_DIFF",(f_R) - (f_distance_from_circle_center_to_c)); 
            Notify("F_SOUND_SPEED_DEPTH",(f_sound_speed_depth)); 

            Notify("M_SOUND_SPEED_GRADIENT",(m_sound_speed_gradient)); 
            Notify("F_LAUNCH_ANGLE",(f_launch_angle)); 
 
	    Notify("f_R",(f_R));
	    //            Notify("center_to_center_dist",(f_distance_from_circle_center_to_c));

	    Notify("R_V_H",(m_radius_vehicle_host));
            Notify("R_V_C",(m_radius_vehicle_c));
	    Notify("E_A",m_elevation_angle);
	    Notify("F_Z_C",m_z_circle);
	    //            Notify("M_SOUND_SPEED_GRADIENT",(m_sound_speed_gradient));
	    // Notify("F_LAUNCH_ANGLE",(f_launch_angle));
	    //}
	    //}


  Notify("ACOUSTIC_PATH", "elev_angle= , transmission_loss= , id=oshea1@mit.edu");
  //Notify("ACOUSTIC_PATH", m_c_nav_x);
  Notify("CONNECTIVITY_LOCATION", "x= , y= , depth= , id=oshea1@mit.edu");
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle::OnStartUp()
{
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

void CommunicationAngle::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
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

bool CommunicationAngle::buildReport()
 {
      m_msgs << "Calculated Elevation angle (rad): " << m_elevation_angle << endl;     
      m_msgs << "Calculated Elevation angle (deg): " << m_elevation_angle*57.3 << endl;
      m_msgs << "Radius calculated from host: " << m_radius_vehicle_host  << endl;  
      m_msgs << "Radius calculated from collaborator: " << m_radius_vehicle_c  << endl;
      m_msgs << "Height of the circle:  " << m_z_circle << endl;

      m_msgs << "Host Depth: " << m_nav_depth << endl;
      m_msgs << "Host X:  " << m_nav_x << endl;
      m_msgs << "Host Y: " << m_nav_y  << endl;
      m_msgs << "Col Depth: " << m_c_nav_depth  << endl;
      m_msgs << "Col X  " << m_c_nav_x << endl;
      m_msgs << "Col Y  " << m_c_nav_y << endl;
  return(true);
 }
