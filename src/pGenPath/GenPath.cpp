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
	      }
	      }
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
      
      if(param == "foo") {
        //handled
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
}

