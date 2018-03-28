/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"
//#include "GeomUtils.h"
#include "XYPoint.h"
#include "MOOS/libMOOS/MOOSLib.h"
using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  m_count = 0;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if( key == "VISIT_POINT")
	    {
	      m_visit_points_str_list.push_back(sval);
	    }





   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
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

bool PointAssign::Iterate()
{
  
    //setup visit point 1 and 2
    string visit_point_vehicle_1;
    string visit_point_vehicle_2 = "VISIT_POINT_";

    //size of the name parameter
    Notify("VNAME_LENGTH", m_vname_list.size());

    int vp_count = 1; 
  list<string>::iterator p_vp;
  for(p_vp=m_vname_list.begin(); p_vp!=m_vname_list.end(); ) {
     string str_vp = *p_vp;
     string full_result = str_vp.c_str();
     if(vp_count == 1)
       {
     m_vname_string_1 = "VISIT_POINT_" + full_result;
     Notify("VNAME_CONTENTS", m_vname_string_1);
       }

  if(vp_count == 2)
    {
      m_vname_string_2 = "VISIT_POINT_" + full_result;
    }

    p_vp = m_vname_list.erase(p_vp);
  vp_count = vp_count+1;
  }

  Notify("UTS_PAUSE", "toggle");
  //int count = 0;
  list<string>::iterator p;
  for(p=m_visit_points_str_list.begin(); p!=m_visit_points_str_list.end(); ) {
    string str = *p;
    string full_result = str.c_str();
    bool assign_by_region;
    setBooleanOnString(assign_by_region,m_assign_by_region);
    if( full_result == "lastpoint")
      {
	Notify(m_vname_string_2, full_result);
	Notify(m_vname_string_1, full_result);
      }
    else
      {
    if( assign_by_region == true)// && m_count != 0 && m_count !=101 )
      { 
	string x_val = tokStringParse(full_result, "x", ',', '=');
	double x_val_float = atof(x_val.c_str());

	string y_val = tokStringParse(full_result, "y", ',', '=');
	double y_val_float = atof(y_val.c_str());

	string label = tokStringParse(full_result, "id", ',', '=');


	if( x_val_float > 112)
	  {
	    Notify(m_vname_string_2, full_result);
	    postViewPoint(x_val_float, y_val_float, label, "yellow");
	  }
	else
	  {
	    Notify(m_vname_string_1, full_result);
	    postViewPoint(x_val_float, y_val_float, label, "red");
	  }
      }
     
     if( assign_by_region == false)
       { // if the count is even, assign to vehicle one
	 string x_val = tokStringParse(full_result, "x", ',', '=');
	 double x_val_float = atof(x_val.c_str());

	 string y_val = tokStringParse(full_result, "y", ',', '=');
	 double y_val_float = atof(y_val.c_str());

	 string label = tokStringParse(full_result, "id", ',', '=');

	 if( m_count%2 == 0)
	   {
	     Notify(m_vname_string_2, full_result);
	     postViewPoint(x_val_float, y_val_float, label, "yellow");
	   }
	 else
	   {
	     Notify(m_vname_string_1, full_result);
	     postViewPoint(x_val_float, y_val_float, label, "red");
	   }
       }
      }
      // count = count +1;
  p = m_visit_points_str_list.erase(p);
  m_count = m_count + 1;
  }



  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;

      if(param == "vname") {
	        m_vname_list.push_back(value);
		m_vname_string = value;
      }
      else if(param == "assign_by_region") {
        m_assign_by_region = value;
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PointAssign::RegisterVariables()
{
  Register("VISIT_POINT", 0);
}


void PointAssign::postViewPoint(double x, double y, string label, string color)
{
    XYPoint point(x, y);
  point.set_label(label);
  point.set_color("vertex", color);
  point.set_param("vertex_size", "2");
  string spec = point.get_spec();
  Notify("VIEW_POINT",spec);
}
