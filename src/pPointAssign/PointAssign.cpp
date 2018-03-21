/************************************************************/
/*    NAME: Ryan OShea                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"
//#include "MBUtils.h"

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

  Notify("UTS_PAUSE", "toggle");
  //int count = 0;
  list<string>::iterator p;
  for(p=m_visit_points_str_list.begin(); p!=m_visit_points_str_list.end(); ) {
    string str = *p;
    string full_result = str.c_str();
    string assign_by_region = "true";
     if( assign_by_region == "true" && m_count != 0 && m_count !=101 )
      { 
	string x_val = tokStringParse(full_result, "x", ',', '=');
	float x_val_float = atof(x_val.c_str());
	if( x_val_float > 112)
	  {
	    Notify("VISIT_POINT_1", full_result);
	  }
	else
	  {
	    Notify("VISIT_POINT_2", full_result);
	  }
      }
     
     if( assign_by_region == "false" && m_count != 0 && m_count !=101)
       { // if the count is even, assign to vehicle one
	 if( m_count%2 == 0)
	   {
	     Notify("VISIT_POINT_2", full_result);
	   }
	 else
	   {
	     Notify("VISIT_POINT_1", full_result);
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

void PointAssign::RegisterVariables()
{
  Register("VISIT_POINT", 0);
}

