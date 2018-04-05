/************************************************************/
/*    NAME: Ryan O'Shea                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include "XYSegList.h"
using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_visit_radius = 5; //default of f5
  m_genpath_regenerate = "false";
  m_nav_x = 0;
  m_nav_y = 0;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
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
    
    if(key == "VISIT_POINT")
      {
	if(sval == "lastpoint")
	  {
	    m_lastpoint_received = "true";
	    Notify("LAST_POINT_RECEIVED","TRUE");
	  }
	else
	  {
	    if( sval == "firstpoint")
	      {}
	    else
	      {
		m_visit_points_str_list.push_back(sval);
		m_visit_points_str_list_missed.push_back(sval);
	      }
	  }
      }
    if(key == "GENPATH_REGENERATE")
      {
	m_genpath_regenerate = sval;
      }
    //set the NAV-X and NAV-Y
    if( key == "NAV_X")
      {
	m_nav_x = dval;
      }
    if(key == "NAV_Y")
      {
	m_nav_y = dval;
      }

  }
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
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

bool GenPath::Iterate()
{
  /*
  list<string>::iterator p;
  for(p=m_visit_points_str_list.begin(); p!=m_visit_points_str_list.end(); ) {
    string str = *p;
    string full_result = str.c_str();
    
    

    p = m_visit_points_str_list.erase(p);
    }*/

  // NOTIFY Console of visit radius
  Notify("VISIT_RADIUS",m_visit_radius);


  //we need to save all of the points that were missed in a new buffer
  //if nav_x/y result in larger difference than the visit_radius, then we should add the point to the list, or we can just remove the points that are, sort of the opposite of what is going on in genpath
  //m_visit_points_str_list_missed
    if(m_visit_points_str_list_missed.size()>0)
    {
      list<string>::iterator p;
      list<string>::iterator remove_point;

      double f_min_x_loc = 10;
      double f_min_y_loc = 20;
      //     p=m_visit_points_str_list.begin();
      for(p=m_visit_points_str_list_missed.begin(); p!=m_visit_points_str_list_missed.end(); )
	{

	  //get out the data for this point                                                                                                                                                                                                                 
	  string str = *p;
	  string full_result = str.c_str();

	  string x_val = tokStringParse(full_result, "x", ',', '=');
	  double x_val_float = atof(x_val.c_str());

	  string y_val = tokStringParse(full_result, "y", ',', '=');
	  double y_val_float = atof(y_val.c_str());

	  string label = tokStringParse(full_result, "id", ',', '=');

	  //if the difference between the point and the current x/y position is below the threshold, remove it from the missed list
	  //assign first distance to minimum distance                                                                                                                                                                                                       
	    
	  
	    
	    double f_diff_x = m_nav_x - x_val_float;
	    double f_diff_y = m_nav_y - y_val_float;
	    double f_current_distance_missed = sqrt( pow(f_diff_x,2) + pow(f_diff_y,2));
	    //remove the point from the list if it is less than the visit radius
	    if(f_current_distance_missed < m_visit_radius)
	      {
		remove_point = p;
			p=m_visit_points_str_list_missed.erase(remove_point);
	      }
	    else
	      {
		p++;
	      }
	  
	  
	}
    }
  
  


  //if genpath has been set to true, then we should regenerate the path using the list of missed points
  if(m_genpath_regenerate == "true")
    {
      //reassign all of the values in the visit_points str list
      //then let the program proceed as normal
      //m_visit_points_str_list_missed
      if(m_visit_points_str_list_missed.size()>0)
       {
	 m_visit_points_str_list = m_visit_points_str_list_missed;
	 m_visit_points_str_list.assign(m_visit_points_str_list_missed.begin(),m_visit_points_str_list_missed.end());
	 Notify("VISIT_STR_LIST_SIZE", m_visit_points_str_list_missed.size());
      //else
       //{
       // Notify("STATION_KEEP","true");
       //}
      //set m_lastpoint_received == "true", so that the path planner gets run again
      m_lastpoint_received = "true";
      //set m_genpath_regenerate == "false", so that this doesnt get called again
      m_genpath_regenerate = "false";
       }
      else
	{
	  Notify("WAYPOINT", "false");
	}


   }
  
  
  //if the last point in the list was "lastpoint"
  //then we know the list is complete and we can begin building the waypoints
  if(m_lastpoint_received == "true")
    {
      //double for distance minimum
      double f_minimum_distance = 10000000;
      double f_current_x = 0;
      double f_current_y = 0;
      double f_current_distance;



  //iterate through the entire list, and assign the x and y points of the minimum distance from the current location, every time the distane is less than the current distance
      list<string>::iterator p;
      list<string>::iterator remove_point;
      list<string>::iterator p_outer;
	  XYSegList my_seglist;
	  Notify("STR_LIST_SIZE",m_visit_points_str_list.size());
	    while(m_visit_points_str_list.size()>0)
	  {
	  double f_min_x_loc = 10;
	  double f_min_y_loc = 20;
	  p=m_visit_points_str_list.begin();
	    for(p=m_visit_points_str_list.begin(); p!=m_visit_points_str_list.end(); ++p)
	      {

	    //get out the data for this point
	    string str = *p;
	    string full_result = str.c_str();

	    string x_val = tokStringParse(full_result, "x", ',', '=');
	    double x_val_float = atof(x_val.c_str());

	    string y_val = tokStringParse(full_result, "y", ',', '=');
	    double y_val_float = atof(y_val.c_str());

	    string label = tokStringParse(full_result, "id", ',', '=');

	    
	    //assign first distance to minimum distance
	    if(p == m_visit_points_str_list.begin())
	      {
		Notify("FIRST_STR", full_result);
		double f_diff_x = f_current_x - x_val_float;
		double f_diff_y = f_current_y - y_val_float;
		//set min and x/y loc
		f_minimum_distance = sqrt( pow(f_diff_x,2) + pow(f_diff_y,2)); 
		f_min_x_loc = x_val_float;
		f_min_y_loc = y_val_float;
		remove_point = p;
	      }
	    
	    else
	      {
		double f_diff_x = f_current_x - x_val_float;
		double f_diff_y = f_current_y - y_val_float;
		f_current_distance = sqrt( pow(f_diff_x,2) + pow(f_diff_y,2));
		  
		  if(f_current_distance < f_minimum_distance)
		    {
		      f_minimum_distance = f_current_distance;
		      f_min_x_loc = x_val_float;
		      f_min_y_loc = y_val_float;
		      remove_point = p;
		    }
	      }

	      }

	//after iterating through all of the points, remove the point that had the smallest distance from the overall list
	m_visit_points_str_list.erase(remove_point);
	   

      //and add that point to the waypoint list 
	my_seglist.add_vertex(f_min_x_loc, f_min_y_loc);

	//then update current x/y location
	f_current_x = f_min_x_loc;
	f_current_y = f_min_y_loc;
	  }
	  
      //Send the notification with the waypoint list
      string update_str = "points = ";
      //my_seglist.add_vertex(110, 120);

      // XYSegList my_seglist2;
      //double three = 3;
      //double eight = 8;
      //my_seglist2.add_vertex(three, eight);
      update_str      += my_seglist.get_spec();
      Notify("UPDATES_VAR", update_str);  // UPDATES_VAR depends on your config

      //so it doesnt redo the path plan  again
      m_lastpoint_received = "false";
    
    }


  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "visit_radius") {
        //handles what to do with the new value
	m_visit_radius = atof(value.c_str());
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{
   Register("VISIT_POINT", 0);
   Register("NAV_X", 0);
   Register("NAV_Y", 0);
   Register("GENPATH_REGENERATE", 0);

}

