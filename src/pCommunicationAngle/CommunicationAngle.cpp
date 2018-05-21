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
  //set the constants and calculate some variables that reside in the function
  //basic plan is to check if the angle is within the circle

  uint64_t f_launch_angle = 1; //deg to rad
  double f_local_angle = f_launch_angle; 
  double f_sound_speed_depth = m_surface_sound_speed + m_sound_speed_gradient * m_nav_depth;
  double f_R = f_sound_speed_depth /(m_sound_speed_gradient * cos(f_launch_angle));
  double f_s = f_R * (f_launch_angle - f_local_angle) ;
  m_z_circle = (-1) * m_surface_sound_speed/m_sound_speed_gradient;

  //sqrt of the difference in distances between vehicles
  double f_distance_between_3_D = sqrt(pow((m_nav_x - m_c_nav_x),2) + pow((m_nav_y - m_c_nav_y),2) + pow((m_nav_depth-m_c_nav_depth),2)); 

            //calculate the vectors to the vehicle
	    double f_x_difference_2_D = m_c_nav_x - m_nav_x;
	    double f_y_difference_2_D = m_c_nav_y - m_nav_y;
	    double f_z_difference_2_D = m_c_nav_depth - m_nav_depth;

	    double f_x_unit_vector = f_x_difference_2_D/f_distance_between_3_D;
	    double f_y_unit_vector = f_y_difference_2_D/f_distance_between_3_D;
	    double f_z_unit_vector = f_z_difference_2_D/f_distance_between_3_D;

	    //find the first bisector, between the two vehicles
	    double f_vehicle_bisector_x = f_x_difference_2_D/2 + m_nav_x;
	    double f_vehicle_bisector_y = f_y_difference_2_D/2 + m_nav_y;
	    double f_vehicle_bisector_z = f_y_difference_2_D/2 + m_nav_depth;

	    //find the perpendicular line from there to the center of the circle, at some known height m_z_circle
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

	    //calculate the radius from the circle to the host vehicle and collaborator
	    m_radius_vehicle_host = sqrt(pow((m_nav_x - f_circle_x_loc),2) + pow((m_nav_y - f_circle_y_loc),2) + pow((m_nav_depth - f_circle_z_loc),2));
	    m_radius_vehicle_c = sqrt(pow((m_c_nav_x- f_circle_x_loc),2) + pow((m_c_nav_y - f_circle_y_loc),2) + pow((m_c_nav_depth - f_circle_z_loc),2));
			 
	    //if there is little difference between the two calculate the angle and report acoustic path and connectivity location
	    m_elevation_angle = -1*acos(m_surface_sound_speed/m_sound_speed_gradient/m_radius_vehicle_host);

	    //calculate Transmission Loss parameters
	    double f_theta = 0; //need to update this value for accurate assesment
	    double f_z_s = m_radius_vehicle_host * cos(m_elevation_angle) - m_surface_sound_speed/m_sound_speed_gradient;
            double f_r_s = m_radius_vehicle_host * (sin(m_elevation_angle) - sin(f_theta));

	    double f_J_s = f_r_s/sin(f_theta); //need to update

	    double f_sound_speed_depth_c = m_surface_sound_speed + m_sound_speed_gradient * m_c_nav_depth;

	    double f_pressure_field_s = 1/(4*3.1415) * sqrt(abs(f_sound_speed_depth_c*cos(m_elevation_angle)/(m_surface_sound_speed +f_J_s)));

	    double f_transmission_loss = -20*log( f_pressure_field_s/(1/(4*3.1415)));


	    //NOTIFY a bunch of variables for testing
            Notify("F_SOUND_SPEED_DEPTH",(f_sound_speed_depth)); 
            Notify("M_SOUND_SPEED_GRADIENT",(m_sound_speed_gradient)); 
            Notify("F_LAUNCH_ANGLE",(f_launch_angle)); 
	    Notify("f_R",(f_R));
	    Notify("R_V_H",(m_radius_vehicle_host));
            Notify("R_V_C",(m_radius_vehicle_c));
	    Notify("E_A",m_elevation_angle);
	    Notify("F_Z_C",m_z_circle);

	    //create the string which we then NOTIFY
	    acoustic_path_string = "elev_angle= " + to_string(m_elevation_angle* 57.3) + " transmission_loss= " + to_string(f_transmission_loss) + ", id=oshea1@mit.edu";
  Notify("ACOUSTIC_PATH", acoustic_path_string);
  
  //if there is an angle that works, notify this location
  connectivity_location_string = "x=" + to_string(m_nav_x) + ", y= " +to_string(m_nav_y) + ", depth= " + to_string(m_nav_depth) + ", id=oshea1@mit.edu";
  Notify("CONNECTIVITY_LOCATION", connectivity_location_string);
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
      m_msgs << "Acousting Path  " << acoustic_path_string << endl;
      m_msgs << "Connectivity Location   " << connectivity_location_string << endl;
  return(true);
 }
